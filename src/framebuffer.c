#include "framebuffer.h"

const int CHAR_H = 8;
const int CHAR_W = 8;

void init_framebuffer(struct multiboot_tag_framebuffer* tagfb){
    framebuffer = (uint32_t *) (uint32_t) tagfb->common.framebuffer_addr;
    fb_height = tagfb->common.framebuffer_height;
    fb_width = tagfb->common.framebuffer_width;
    fb_terminal_w = fb_width / CHAR_W;
    fb_terminal_h = fb_height / CHAR_H;
}

void fb_setPixel(uint32_t x, uint32_t y, uint32_t color){
    if (fb_width <= x || fb_height <= y)
        return;
    framebuffer[fb_width * y + x] = color;
}

void fb_write_cell(uint32_t index, char c, uint32_t fg, uint32_t bg){
    uint32_t x = (index % (fb_terminal_w))*CHAR_W;
    uint32_t y = (index / (fb_terminal_w))*CHAR_H;
    if(!(c >= 32 && c <= 126)) c = ' ';
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            framebuffer[fb_width * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void fb_setChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            framebuffer[fb_width * ((y*CHAR_H)+layer) + (x*CHAR_W)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void fb_write_xy(char* Buffer, int length, int x, int y){
    for(int i = 0; i < length; i++){
        fb_setChar(x+i, y, Buffer[i], 0xFFFFFF, 0);
    }
}

void fb_clear(uint32_t color){
    for(uint32_t i = 0; i < fb_width*fb_height; i++){
        framebuffer[i] = color;
    }
}

void change_video_pointer(){
    fb_clear(0);
    framebuffer = (uint32_t *) framebuffer;
}