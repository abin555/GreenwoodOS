#include "framebuffer.h"

uint32_t *fb_frontbuffer;
uint32_t fb_width;
uint32_t fb_height;

void fb_init(struct multiboot_tag_framebuffer* tagfb){
	fb_frontbuffer = (uint32_t *) (uint32_t) tagfb->common.framebuffer_addr;
	fb_width = tagfb->common.framebuffer_width;
	fb_height = tagfb->common.framebuffer_height;

    create_page_entry((uint32_t) fb_frontbuffer, (uint32_t) fb_frontbuffer, 0x93);
    create_page_entry((uint32_t) fb_frontbuffer+0x400000, (uint32_t) fb_frontbuffer+0x400000, 0x93);
}

void fb_setPixel(uint32_t idx, uint32_t color){
	fb_frontbuffer[idx] = color;
}

void fb_putChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
	for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            fb_frontbuffer[fb_width *(y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}