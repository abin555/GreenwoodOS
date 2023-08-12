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
#include "program.h"

void kbd_test(){
    char c = kbd_getChar();
    if(!c) return;
    fb_putChar(fb_width-CHAR_W, CHAR_H, c, 0xFFFFFF, 0);
}

void kernal_task(){
    print_serial("Kernel Continuing Boot\n");
    fb_print(0, 8, "Kernal Tasking!");

    program_init();

    init_drive_system();

    print_serial("[Driver] Initializing Drivers\n");
    for(int i = 0; i < PCI_numDrivers; i++){
        if(PCI_drivers[i]->init_driver != NULL){
            print_serial("[Driver] Init #%x\n", i);
            PCI_drivers[i]->init_driver(PCI_drivers[i]);
        }
    }
    MEM_printRegions();
    drive_enumerate();
    
    //AHCI_read((volatile HBA_PORT *)(drives[0]->driver.ahci), 0, 0, 5, (uint16_t *) fb_frontbuffer);
    
    ISO9660_printTree(drive_get('A')->format_info.ISO);
    ISO9660_printTree(drive_get('B')->format_info.ISO);

    print_serial("Testing File Open\n");
    //struct File_Info test = ISO9660_GetFile(drive_get('A')->format_info.ISO, "CHILD/TEST/../../CHILD_CO/OTHER.TXT");
    //char *buf = (char *) malloc(test.size);
    //ISO9660_openFile(drive_get('A')->format_info.ISO, test, buf, test.size);
    //print_serial("File Contents: %s\n", buf);

    struct FILE* file = fopen("A/CHILD/TEST/TEST.TXT");
    //print_serial("First Char: %c\n", fgetc(file));
    char *buf = malloc(fsize(file));
    fcopy(file, buf, fsize(file));
    print_serial("%s\n", buf);
    fclose(file);

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

    start_task(kernal_task, -1, 0, NULL, "Kernel");
    multitask_init();

    while(1){

    }

	return 0;
}
