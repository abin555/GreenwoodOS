#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"

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
	page_init();
	fb_init(fb);
	//fb_setPixel(10, 0xFFFFFF);

	for(uint32_t i = 0; i < fb_width * fb_height; i+=6){
		fb_setPixel(i, 0xFFFFFF);
	}

	return 0;
}