//REFERENCED: https://github.com/VendelinSlezak/BleskOS/blob/master/source/drivers/sound/ac97.c

#include "ac97.h"
#include "timer.h"

extern void wait(int ticks);
void ac97_check_headphone_connection_change();

struct audio_driver *ac97_init(struct PCI_driver *driver){
	print_serial("[AC97] Init: \"%s\"\n", driver->name);
	struct audio_driver *audio = malloc(sizeof(struct audio_driver));

    pci_enable_io_busmastering(driver->device->bus, driver->device->slot, driver->device->device);

	audio->pci = driver;
	audio->int_number = driver->interrupt;
	audio->deviceType = AUDIO_AC97;

	if(driver->BAR[0] & 0x1){
        audio->bar_type = 1;
    }
    else{
        audio->bar_type = 0;
    }
    print_serial("[AC97] BAR Type: %d\n", audio->bar_type);

	audio->nam_base = driver->BAR[0] & (~0x3);
	audio->nabm_base = driver->BAR[1] & (~0x3);
    audio->mem_base = driver->BAR[0] & (~0xf);

    struct AC97_driver *ac97 = malloc(sizeof(struct AC97_driver));
    audio->device.ac97 = ac97;

    ac97->private_page = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(0x40000), 0x40000, 0, DRIVER));
    ac97->page_head = 0;
    //MEM_reserveRegion((uint32_t) ether->mem_base, (uint32_t) ether->mem_base, DRIVER);

    print_serial("[AC97] Use %s based access (base: 0x%x)\n", (audio->bar_type) ? "mem" : "port", (audio->bar_type != 0) ? audio->nam_base : audio->mem_base);
    
    //Resume
    mmio_outb(audio->nabm_base + AC97_NABM_IO_GLOBAL_CONTROL, (0b00<<22) | (0b00<<20) | (0<<2) | (1<<1));
    wait(20);

    //reset all streams
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_INPUT_CONTROL, 0x2);
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x2);
    mmio_outb(audio->nabm_base + AC97_NABM_IO_MICROPHONE_INPUT_CONTROL, 0x2);
    wait(20);

    //reset all NAM registers
    mmio_outw(audio->nam_base + AC97_NAM_IO_RESET, 0xFF);
    
    ac97->headphone_output_present = 0;
    if((mmio_inw(audio->nam_base + AC97_NAM_IO_CAPABILITES) & AC97_CAPABILITY_HEADPHONE_OUTPUT)==AC97_CAPABILITY_HEADPHONE_OUTPUT && mmio_inw(audio->nam_base + AC97_NAM_IO_AUX_OUT_VOLUME)==0x8000) {
        ac97->headphone_output_present = 1;
        print_serial("[AC97] Headphone Present\n");
    }

    //Set max PCM output volume to 0
    mmio_outw(audio->nam_base + AC97_NAM_IO_PCM_OUT_VOLUME, 0x0);

    ac97->buffer_memory_pointer = (struct ac97_buffer_entry *) ac97->private_page;
    ac97->page_head += sizeof(struct ac97_buffer_entry)*32;

    //read extended capabilities
    ac97->extended_capabilities = mmio_inw(audio->nam_base + AC97_NAM_IO_EXTENDED_CAPABILITIES);
    if((ac97->extended_capabilities & AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE)==AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE) {
        mmio_outw(audio->nam_base + AC97_NAM_IO_EXTENDED_FEATURES_CONTROL, AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE); //enable variable sample rate
    }

    //get number of AUX_OUT volume steps
    ac97->aux_out_number_of_volume_steps = 31;
    mmio_outw(audio->nam_base + AC97_NAM_IO_AUX_OUT_VOLUME, 0x2020);
    if((mmio_inw(audio->nam_base + AC97_NAM_IO_AUX_OUT_VOLUME) & 0x2020)==0x2020) {
        ac97->aux_out_number_of_volume_steps = 63;
    }

    //set output 
    sound_volume = 0;
    if(ac97_is_headphone_connected(audio)==1) {
        ac97_set_output(audio, AC97_HEADPHONE_OUTPUT);
    }
    else {
        ac97_set_output(audio, AC97_SPEAKER_OUTPUT);
    }

    timer_attach(50, ac97_check_headphone_connection_change);
 
    //log
    print_serial("[AC97] Sound card AC97\n");
    print_serial("[AC97] Capabilities: %x\n", mmio_inw(audio->nam_base + AC97_NAM_IO_CAPABILITES));
    print_serial("[AC97] Extended capabilities: %x\n", ac97->extended_capabilities);

	return audio;
}

void ac97_check_headphone_connection_change(){
    for(int i = 0; i < 4; i++){
        struct audio_driver *audio = audio_drivers[i];
        if(audio == NULL || audio->deviceType != AUDIO_AC97) continue;

        struct AC97_driver *ac97 = audio->device.ac97;
        if(ac97->selected_output==AC97_SPEAKER_OUTPUT && ac97_is_headphone_connected(audio)==1) { //headphone was connected
            ac97_set_output(audio, AC97_HEADPHONE_OUTPUT);
        }
        else if(ac97->selected_output==AC97_HEADPHONE_OUTPUT && ac97_is_headphone_connected(audio)==0) { //headphone was disconnected
            ac97_set_output(audio, AC97_SPEAKER_OUTPUT);
        }
    }
}

uint8_t ac97_is_headphone_connected(struct audio_driver *audio){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return 0;
    struct AC97_driver *ac97 = audio->device.ac97;
    if(ac97->headphone_output_present == 1 && (mmio_inw(audio->nam_base + AC97_NAM_IO_JACK_SENSE) & 0x8) == 0x8){
        return 1;
    }
    else{
        return 0;
    }
}

void ac97_set_volume_in_register(struct audio_driver *audio, uint32_t offset, uint8_t numVolSteps, uint8_t vol){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    //struct AC97_driver *ac97 = audio->device.ac97;

    if(vol == 0){
        mmio_outw(audio->nam_base + offset, 0x8000);
    }
    else {
        vol = ((100-vol)*numVolSteps/100);
        mmio_outw(audio->nam_base + offset, ((vol) | (vol << 8)));
    }
}

void ac97_set_output(struct audio_driver *audio, uint8_t output){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    struct AC97_driver *ac97 = audio->device.ac97;

    if(output == AC97_SPEAKER_OUTPUT){
        ac97_set_volume_in_register(audio, AC97_NAM_IO_AUX_OUT_VOLUME, ac97->aux_out_number_of_volume_steps, 0);
        ac97_set_volume_in_register(audio, AC97_NAM_IO_MASTER_VOLUME, AC97_SPEAKER_OUTPUT_NUMBER_OF_VOLUME_STEPS, sound_volume);
    }
    else if(output == AC97_HEADPHONE_OUTPUT){
        ac97_set_volume_in_register(audio, AC97_NAM_IO_MASTER_VOLUME, ac97->aux_out_number_of_volume_steps, 0);
        ac97_set_volume_in_register(audio, AC97_NAM_IO_AUX_OUT_VOLUME, AC97_SPEAKER_OUTPUT_NUMBER_OF_VOLUME_STEPS, sound_volume);
    }
    else{
        return;
    }

    ac97->selected_output = output;
}

void ac97_set_volume(struct audio_driver *audio, uint8_t volume){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    struct AC97_driver *ac97 = audio->device.ac97;
    
    if(ac97->selected_output==AC97_SPEAKER_OUTPUT) {
        ac97_set_volume_in_register(audio, AC97_NAM_IO_MASTER_VOLUME, ac97->aux_out_number_of_volume_steps, volume);
    }
    else if(ac97->selected_output==AC97_HEADPHONE_OUTPUT) {
        ac97_set_volume_in_register(audio, AC97_NAM_IO_AUX_OUT_VOLUME, AC97_SPEAKER_OUTPUT_NUMBER_OF_VOLUME_STEPS, volume);
    }
}

uint8_t ac97_is_supported_sample_rate(struct audio_driver *audio, uint16_t sample_rate){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return 0;
    struct AC97_driver *ac97 = audio->device.ac97;

    if(sample_rate == 48000){
        return 1;
    }
    else if((ac97->extended_capabilities & AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE) == AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE) {
        if(sample_rate == 44100){
            return 1;
        }
        else{
            mmio_outw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_FRONT_DAC, sample_rate);
            if(mmio_inw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_FRONT_DAC) == sample_rate){
                return 1;
            }
        }
    }
    return 0;
}

void ac97_set_sample_rate(struct audio_driver *audio, uint16_t sample_rate){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    struct AC97_driver *ac97 = audio->device.ac97;

    if((ac97->extended_capabilities & AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE)==AC97_EXTENDED_CAPABILITY_VARIABLE_SAMPLE_RATE) {
        //set same variable rate on all outputs
        mmio_outw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_FRONT_DAC, sample_rate);
        mmio_outw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_SURR_DAC, sample_rate);
        mmio_outw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_LFE_DAC, sample_rate);
        mmio_outw(audio->nam_base + AC97_NAM_IO_VARIABLE_SAMPLE_RATE_LR_ADC, sample_rate);
    }
}

extern uint32_t timer_ticks;
void task_ac97_play_buffer_in_loop();
void ac97_play_pcm_data_in_loop(struct audio_driver *audio, uint16_t sample_rate) {
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    struct AC97_driver *ac97 = audio->device.ac97;
    // stop sound
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x0);

    // set sample rate
    ac97_set_sample_rate(audio, sample_rate);

    // reset stream
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x2);
    uint32_t startticks = timer_ticks;
    while ((mmio_inb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL) & 0x2) == 0x2)
    {
        asm("nop");
        if (timer_ticks > startticks + 50)
        { // stream was not reseted after 100 ms
            return;
        }
    }
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x0);

    // clear buffer
    //clear_memory((uint32_t)ac97_buffer_memory_pointer, (sizeof(struct ac97_buffer_entry) * 32));

    // set buffer address
    mmio_outd(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_BUFFER_BASE_ADDRESS, (uint32_t) ac97->buffer_memory_pointer);

    // fill buffer entries
    uint32_t sound_memory = (uint32_t)pcm_data;
    uint32_t sound_length = (sound_buffer_refilling_info->buffer_size * 2);
    for (uint32_t i = 0; i < 32; i++)
    {
        if (sound_length > 0x2000 * 2)
        {
            ac97->buffer_memory_pointer[i].sample_memory = sound_memory;
            ac97->buffer_memory_pointer[i].number_of_samples = 0x2000;
            sound_memory += 0x2000 * 2;
            sound_length -= 0x2000 * 2;
        }
        else
        {
            ac97->buffer_memory_pointer[i].sample_memory = sound_memory;
            ac97->buffer_memory_pointer[i].number_of_samples = ((sound_length / 2) & 0xFFFE);
            break;
        }
    }

    // clear status
    mmio_outw(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_STATUS, 0x1C);

    // start streaming
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x1);

    // add task for playing buffer in loop
    timer_attach(1, task_ac97_play_buffer_in_loop);
}

void task_ac97_play_buffer_in_loop() {
    struct audio_driver *audio = audio_drivers[selected_dev];
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    //struct AC97_driver *ac97 = audio->device.ac97;

    //update Last Valid Entry register for all entries to be valid
    outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_LAST_VALID_ENTRY, (inb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CURRENTLY_PROCESSED_ENTRY)-1) & 0x1F);
}

uint32_t ac97_get_actual_stream_position(struct audio_driver *audio){
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return 0;
    struct AC97_driver *ac97 = audio->device.ac97;
    uint32_t number_of_processed_bytes = 0;

    //add already played buffers
    for(uint32_t i=0; i< mmio_inb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CURRENTLY_PROCESSED_ENTRY); i++) {
        number_of_processed_bytes += ac97->buffer_memory_pointer[i].number_of_samples*2;
    }

    //add actual entry position
    number_of_processed_bytes += (ac97->buffer_memory_pointer[mmio_inb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CURRENTLY_PROCESSED_ENTRY)].number_of_samples*2 - mmio_inw(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CURRENT_ENTRY_POSITION)*2);

    return number_of_processed_bytes;
}

void ac97_stop_sound(struct audio_driver *audio) {
    if(audio == NULL || audio->deviceType != AUDIO_AC97) return;
    struct AC97_driver *ac97 = audio->device.ac97;
    //stop streaming
    mmio_outb(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_CONTROL, 0x0);

    //clear status
    mmio_outw(audio->nabm_base + AC97_NABM_IO_PCM_OUTPUT_STATUS, 0x1C);

    //buffers will not be filled by code in drivers/system/processes_on_background.c
    ac97->playing_state = AC97_BUFFER_FILLED;

    //destroy task for looping
    timer_detach(task_ac97_play_buffer_in_loop);
}