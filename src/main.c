#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"
#include "grub.h"
#include "memory.h"
#include "allocator.h"
#include "pci.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "multitasking.h"
#include "ahci.h"
#include "drive.h"

void kbd_test(){
    fb_putChar(8*CHAR_W, 8*CHAR_H, kbd_getChar(), 0xFFFFFF, 0);
}


int test_switch = 1;
int task_test(){
    while(1){
        if(test_switch){
            test_switch = 0;
            fb_putChar(fb_width-CHAR_W, 0, '@', 0xFFFFFF, 0);
        }
        else{
            test_switch = 1;
            fb_putChar(fb_width-CHAR_W, 0, 'X', 0xFFFFFF, 0);
        }
        for(int i = 0; i < 10000; i++){
            asm("nop");
            asm("nop");
            asm("nop");
        }
    }
}

void kernal_task(){
    print_serial("Kernel Continuing Boot\n");

    init_drive_system();

    print_serial("[Driver] Initializing Drivers\n");
    for(int i = 0; i < PCI_numDrivers; i++){
        if(PCI_drivers[i]->init_driver != NULL){
            print_serial("[Driver] Init #%x\n", i);
            PCI_drivers[i]->init_driver(PCI_drivers[i]);
        }
    }
    MEM_printRegions();
    AHCI_read((volatile HBA_PORT *)(drives[0]->driver.ahci), 0, 0, 5, (uint16_t *) fb_frontbuffer);
    while(1){

    }
}

int kmain(unsigned int magic, unsigned long magic_addr){
	init_serial();
    print_serial("\n\nGreenwood OS Boot Start!\n");
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        print_serial("WTF, not multiboot?!?!\nHow did we get here...\n:(\n");
        return 0xFF;
    }
	parse_multiboot2(magic_addr);
    // size = *(unsigned *) magic_addr;
    
	load_gdt();
    interrupts_install_idt();
    getCPUVendorString();
    enableSSE();
    //asm("int 0x80");
	page_init();


    set_PAT();

    MEM_populateRegions();

	fb_init(GRUB_tagfb);

    fb_print(0, 0, "Bootup Start");

    alloc_init();

    MEM_printRegions();

    PCI_init();

    kbd_init(0xFF);
    ps2_init();


    timer_init(1000);

    start_task(task_test, -1, 0, NULL, "Debug");
    start_task(kernal_task, -1, 0, NULL, "Kernel");
    print_serial("[Are we here?]\n");
    multitask_init();

    while(1){

    }

	return 0;
}
