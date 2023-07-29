#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"

int kmain(unsigned int magic, unsigned long magic_addr){
	init_serial();
	struct multiboot_tag *tag;
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return 0xFF;
    }
	struct multiboot_tag_framebuffer *fb;
    // size = *(unsigned *) magic_addr;
    for (
        tag = (struct multiboot_tag *)(magic_addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *)tag;
            print_serial("Multiboot Address: 0x%x \nFramebuffer Address: 0x%x\n", (unsigned int) magic_addr, tagfb->common.framebuffer_addr);
            //init_framebuffer(tagfb);
			fb = tagfb;
            break;
        }
        }
    }
	load_gdt();
    interrupts_install_idt();
	page_init();
	fb_init(fb);

    fb_print(0, 0, "Bootup Start");
    fb_print(0,CHAR_H, "Interrupts Loaded");

    while(1){

    }
	//fb_setPixel(10, 0xFFFFFF);

	return 0;
}
