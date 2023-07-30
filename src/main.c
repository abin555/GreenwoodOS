#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"
#include "grub.h"
#include "memory.h"
#include "allocator.h"
#include "pci.h"

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
	page_init();

    getCPUVendorString();

    set_PAT();

    MEM_populateRegions();

	fb_init(GRUB_tagfb);

    fb_print(0, 0, "Bootup Start");

    alloc_init();

    MEM_printRegions();

    PCI_init();

	return 0;
}
