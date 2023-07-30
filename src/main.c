#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"
#include "grub.h"
#include "memory.h"
#include "allocator.h"

int kmain(unsigned int magic, unsigned long magic_addr){
	init_serial();
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        print_serial("WTF, not multiboot?!?!\nHow did we get here...\n:(\n");
        return 0xFF;
    }
	parse_multiboot2(magic_addr);
    // size = *(unsigned *) magic_addr;
    
	load_gdt();
	page_init();

    MEM_populateRegions();

	fb_init(GRUB_tagfb);

    fb_print(0, 0, "Bootup Start");
    interrupts_install_idt();
    fb_print(0,CHAR_H, "Interrupts Loaded");

    alloc_init();

    MEM_printRegions();

    

	return 0;
}
