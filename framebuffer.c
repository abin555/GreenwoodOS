#include "framebuffer.h"
#include "font.h"

#define CHAR_W 8
#define CHAR_H 8

void fb_setPixel(u32 x, u32 y, u32 color){
    if (fb_width <= x || fb_height <= y)
        return;
        
    fb[ fb_width * y + x] = color;
}

void init_fb(struct multiboot_tag_framebuffer *tagfb){
    fb = (u64 *) (unsigned long) tagfb->common.framebuffer_addr;
    fb_height = tagfb->common.framebuffer_height;
    fb_width = tagfb->common.framebuffer_width;
    fb_terminal_w = fb_width / CHAR_W;
    fb_terminal_h = fb_height / CHAR_H;
}

void fb_write_cell(u32 x, u32 y, char c, u32 fg, u32 bg){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            fb[fb_width * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}