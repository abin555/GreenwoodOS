#include "hda.h"
#include "stddef.h"

#define HDA_WIDGET_AUDIO_OUTPUT 0x0
#define HDA_WIDGET_AUDIO_INPUT 0x1
#define HDA_WIDGET_AUDIO_MIXER 0x2
#define HDA_WIDGET_AUDIO_SELECTOR 0x3
#define HDA_WIDGET_PIN_COMPLEX 0x4
#define HDA_WIDGET_POWER_WIDGET 0x5
#define HDA_WIDGET_VOLUME_KNOB 0x6
#define HDA_WIDGET_BEEP_GENERATOR 0x7
#define HDA_WIDGET_VENDOR_DEFINED 0xF

#define HDA_PIN_LINE_OUT 0x0
#define HDA_PIN_SPEAKER 0x1
#define HDA_PIN_HEADPHONE_OUT 0x2
#define HDA_PIN_CD 0x3
#define HDA_PIN_SPDIF_OUT 0x4
#define HDA_PIN_DIGITAL_OTHER_OUT 0x5
#define HDA_PIN_MODEM_LINE_SIDE 0x6
#define HDA_PIN_MODEM_HANDSET_SIDE 0x7
#define HDA_PIN_LINE_IN 0x8
#define HDA_PIN_AUX 0x9
#define HDA_PIN_MIC_IN 0xA
#define HDA_PIN_TELEPHONY 0xB
#define HDA_PIN_SPDIF_IN 0xC
#define HDA_PIN_DIGITAL_OTHER_IN 0xD
#define HDA_PIN_RESERVED 0xE
#define HDA_PIN_OTHER 0xF

#define HDA_OUTPUT_NODE 0x1
#define HDA_INPUT_NODE 0x2


struct IntelHDA_Driver hdaDriver;

uint32_t output_buffer_region[16] __attribute__((aligned(0x400)));
uint32_t corb_mem[512] __attribute__((aligned(0x400)));
uint32_t rirb_mem[512] __attribute__((aligned(0x400)));


void hda_init(struct PCI_driver *driver){
	print_serial("[INTEL HDA] Init\n");
	if(driver == NULL){
		print_serial("[INTEL HDA] PCI Driver is NULL\n");
		return;
	}
	
	MEM_reserveRegion((uint32_t) driver->BAR[0], (uint32_t) driver->BAR[0], DRIVER);
	MEM_reserveRegion((uint32_t) driver->BAR[0]+0x400000, (uint32_t) driver->BAR[0]+0x400000, DRIVER);
	return;

	hdaDriver.base = driver->BAR[0];
	hdaDriver.commType = HDA_UNINITALIZED;
	hdaDriver.initialized = false;
	uint32_t ticks;


	//reset card and set operational state
	mmio_outd(hdaDriver.base + 0x08, 0x0);
	ticks = timer_ticks + 10;
	while(timer_ticks < ticks) {
		asm("nop");
		if((mmio_ind(hdaDriver.base + 0x08) & 0x1)==0x0) {
			break;
		}
	}
	mmio_outd(hdaDriver.base + 0x08, 0x1);
	ticks = timer_ticks + 10;
	while(timer_ticks < ticks) {
		asm("nop");
		if((mmio_ind(hdaDriver.base + 0x08) & 0x1)==0x1) {
			break;
		}
	}
	if((mmio_ind(hdaDriver.base + 0x08) & 0x1)==0x0) {
		print_serial("HDA ERROR: card can not be set to operational state\n");
		return;
	}

	//read capabilities
	print_serial("Sound card High Definition Audio %x.%x\n", mmio_inb(hdaDriver.base + 0x03), mmio_inb(hdaDriver.base + 0x02));
	hdaDriver.input_stream_base = (hdaDriver.base + 0x80);
	hdaDriver.output_stream_base = (hdaDriver.base + 0x80 + (0x20*((mmio_inw(hdaDriver.base + 0x00)>>8) & 0xF))); //skip input streams ports
	hdaDriver.output_buffer_list = (uint32_t *) output_buffer_region;
	//disable interrupts
	mmio_outd(hdaDriver.base + 0x20, 0);
	
	//turn off dma position transfer
	mmio_outd(hdaDriver.base + 0x70, 0);
	mmio_outd(hdaDriver.base + 0x74, 0);
	
	//disable synchronization
	mmio_outd(hdaDriver.base + 0x34, 0);
	mmio_outd(hdaDriver.base + 0x38, 0);

	//stop CORB and RIRB
	mmio_outb(hdaDriver.base + 0x4C, 0x0);
	mmio_outb(hdaDriver.base + 0x5C, 0x0);

	//configure CORB
	hdaDriver.corb_mem = (uint32_t *) corb_mem;
	print_serial("HDA: CORB ADDR: 0x%x\n", hdaDriver.corb_mem);
	mmio_outd(hdaDriver.base + 0x40, (uint32_t)hdaDriver.corb_mem); //CORB lower memory
	mmio_outd(hdaDriver.base + 0x44, 0); //CORB upper memory
	if((mmio_inb(hdaDriver.base + 0x4E) & 0x40)==0x40) {
		hdaDriver.corb_num_entries = 256;
		mmio_outb(hdaDriver.base + 0x4E, 0x2); //256 entries
		print_serial("CORB: 256 entries\n");
	}
	else if((mmio_inb(hdaDriver.base + 0x4E) & 0x20)==0x20) {
		hdaDriver.corb_num_entries = 16;
		mmio_outb(hdaDriver.base + 0x4E, 0x1); //16 entries
		print_serial("CORB: 16 entries\n");
	}
	else if((mmio_inb(hdaDriver.base + 0x4E) & 0x10)==0x10) {
		hdaDriver.corb_num_entries = 2;
		mmio_outb(hdaDriver.base + 0x4E, 0x0); //2 entries
		print_serial("CORB: 2 entries\n");
	}
	else { //CORB/RIRB is not supported
		print_serial("CORB: no size allowed\n");
		goto hda_use_pio_interface;
	}
	mmio_outw(hdaDriver.base + 0x4A, 0x8000); //reset read pointer
	ticks = timer_ticks + 5;
	while(timer_ticks < ticks) {
		asm("nop");
		if((mmio_inw(hdaDriver.base + 0x4A) & 0x8000)==0x8000) { //wait until reset is complete
			break;
		}
	}
	if((mmio_inw(hdaDriver.base + 0x4A) & 0x8000)==0x0000) { //CORB read pointer was not reseted
		print_serial("HDA: CORB pointer can not be put to reset state\n");
		goto hda_use_pio_interface;
	}
	mmio_outw(hdaDriver.base + 0x4A, 0x0000); //go back to normal state
	ticks = timer_ticks + 5;
	while(timer_ticks < ticks) {
		asm("nop");
		if((mmio_inw(hdaDriver.base + 0x4A) & 0x8000)==0x0000) { //wait until is CORB read pointer in normal state
			break;
		}
	}
	if((mmio_inw(hdaDriver.base + 0x4A) & 0x8000)==0x8000) { //CORB read pointer is still in reset
		print_serial("HDA: CORB pointer can not be put from reset state\n");
		goto hda_use_pio_interface;
	}
	mmio_outw(hdaDriver.base + 0x48, 0); //set write pointer
	hdaDriver.corb_pointer = 1;

	//configure RIRB
	hdaDriver.rirb_mem = (uint32_t *) rirb_mem;
	print_serial("HDA: RIRB ADDR: 0x%x\n", hdaDriver.rirb_mem);
	mmio_outd(hdaDriver.base + 0x50, (uint32_t)hdaDriver.rirb_mem); //RIRB lower memory
	mmio_outd(hdaDriver.base + 0x54, 0); //RIRB upper memory
	if((mmio_inb(hdaDriver.base + 0x5E) & 0x40)==0x40) {
		hdaDriver.rirb_num_entries = 256;
		mmio_outb(hdaDriver.base + 0x5E, 0x2); //256 entries
		print_serial("RIRB: 256 entries\n");
	}
	else if((mmio_inb(hdaDriver.base + 0x5E) & 0x20)==0x20) {
		hdaDriver.rirb_num_entries = 16;
		mmio_outb(hdaDriver.base + 0x5E, 0x1); //16 entries
		print_serial("RIRB: 16 entries\n");
	}
	else if((mmio_inb(hdaDriver.base + 0x5E) & 0x10)==0x10) {
		hdaDriver.rirb_num_entries = 2;
		mmio_outb(hdaDriver.base + 0x5E, 0x0); //2 entries
		print_serial("RIRB: 2 entries\n");
	}
	else { //CORB/RIRB is not supported
		print_serial("RIRB: no size allowed\n");
		goto hda_use_pio_interface;
	}
	mmio_outw(hdaDriver.base + 0x58, 0x8000); //reset write pointer
	uint32_t sticks = timer_ticks;
	while(timer_ticks < sticks + 10){}
	mmio_outw(hdaDriver.base + 0x5A, 0); //disable interrupts
	hdaDriver.rirb_pointer = 1;

	print_serial("HDA: Starting CORB & RIRB\n");

	//start CORB and RIRB
	mmio_outb(hdaDriver.base + 0x4C, 0x2);
	mmio_outb(hdaDriver.base + 0x5C, 0x2);

	//find codec and working communication interface
	//TODO: find more codecs
	for(uint32_t codec_number=0, codec_id=0; codec_number<16; codec_number++) {
		hdaDriver.commType = HDA_CORB_RIRB;
		print_serial("HDA: Finding Codec %d\n", codec_number);
		codec_id = hda_send_verb(codec_number, 0, 0xF00, 0);
		

		if(codec_id!=0) {
			print_serial("HDA: CORB/RIRB communication interface\n");
			//hda_initalize_codec(sound_card_number, codec_number);
			return; //initalization is complete
		}
	}

	hda_use_pio_interface:
	print_serial("HDA: PIO Interface\n");
	//stop CORB and RIRB
	mmio_outb(hdaDriver.base + 0x4C, 0x0);
	mmio_outb(hdaDriver.base + 0x5C, 0x0);

	for(uint32_t codec_number=0, codec_id=0; codec_number<16; codec_number++) {
		hdaDriver.commType = HDA_PIO;
		codec_id = hda_send_verb(codec_number, 0, 0xF00, 0);

		if(codec_id!=0) {
			print_serial("HDA: PIO communication interface\n");
			//hda_initalize_codec(sound_card_number, codec_number);
			return; //initalization is complete
		}
	}
	print_serial("HDA: HMMM\n");
	return;
}

int hda_send_verb(uint32_t codec, uint32_t node, uint32_t verb, uint32_t command){
	uint32_t value = ((codec<<28) | (node<<20) | (verb<<8) | (command));

	if(hdaDriver.commType==HDA_CORB_RIRB) { //CORB/RIRB interface
		//write verb
		hdaDriver.corb_mem[hdaDriver.corb_pointer] = value;

		//move write pointer
		mmio_outw(hdaDriver.base + 0x48, hdaDriver.corb_pointer);

		//wait for response
		uint32_t ticks = timer_ticks + 100;
		while(timer_ticks < ticks) {
			asm("nop");
			if(mmio_inw(hdaDriver.base + 0x58)==hdaDriver.corb_pointer) {
				break;
			}
		}
		if(mmio_inw(hdaDriver.base + 0x58)!=hdaDriver.corb_pointer) {
			print_serial("HDA ERROR: no response\n");
			hdaDriver.commType = HDA_UNINITALIZED;
			return 0;
		}

		//read response
		value = hdaDriver.rirb_mem[hdaDriver.rirb_pointer*2];

		//move pointers
		hdaDriver.corb_pointer++;
		if(hdaDriver.corb_pointer==hdaDriver.corb_num_entries) {
		hdaDriver.corb_pointer = 0;
		}
		hdaDriver.rirb_pointer++;
		if(hdaDriver.rirb_pointer==hdaDriver.rirb_num_entries) {
		hdaDriver.rirb_pointer = 0;
		}

		//return response
		return value;
	}
	else if(hdaDriver.commType==HDA_PIO) { //PIO interface
		//clear Immediate Result Valid bit
		mmio_outw(hdaDriver.base + 0x68, 0x2);

		//write verb
		mmio_outd(hdaDriver.base + 0x60, value);

		//start verb transfer
		mmio_outw(hdaDriver.base + 0x68, 0x1);

		//pool for response
		uint32_t ticks = timer_ticks + 3;
		while(timer_ticks < ticks) {
			asm("nop");

			//wait for Immediate Result Valid bit = set and Immediate Command Busy bit = clear
			if((mmio_inw(hdaDriver.base + 0x68) & 0x3)==0x2) {
			//clear Immediate Result Valid bit
			mmio_outw(hdaDriver.base + 0x68, 0x2);

			//return response
			return mmio_ind(hdaDriver.base + 0x64);
			}
		}

		//there was no response after 6 ms
		print_serial("HDA ERROR: no response\n");
		hdaDriver.commType = HDA_UNINITALIZED;
		return 0;
	}

	return 0;
}