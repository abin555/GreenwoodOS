#include "framebuffer.h"

const int CHAR_H = 8;
const int CHAR_W = 8;

void init_framebuffer(struct multiboot_tag_framebuffer* tagfb){
    real_framebuffer = (uint32_t *) (uint32_t) tagfb->common.framebuffer_addr;
    framebuffer = real_framebuffer;
    fb_real_h = tagfb->common.framebuffer_height;
    fb_real_w = tagfb->common.framebuffer_width;

    fb_width = fb_real_w;
    fb_height = fb_real_h;
    fb_terminal_w = fb_width / CHAR_W;
    fb_terminal_h = fb_height / CHAR_H;
}

void init_backbuffer(){
    //back_buffer = (uint32_t*) malloc(sizeof(uint32_t) * fb_width * fb_height);
}

void fb_drawChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            framebuffer[fb_real_w *(y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void fb_setPixel(uint32_t x, uint32_t y, uint32_t color){
    if (fb_real_w <= x || fb_real_h <= y)
        return;
    framebuffer[fb_real_w * y + x] = color;
}

void fb_write_cell(uint32_t index, char c, uint32_t fg, uint32_t bg){
    uint32_t x = (index % (fb_terminal_w))*CHAR_W;
    uint32_t y = (index / (fb_terminal_w))*CHAR_H;
    if(!(c >= 32 && c <= 126)) c = ' ';
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            framebuffer[fb_real_w * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void buf_write_cell(uint32_t *buf, uint32_t w, uint32_t index, char c, uint32_t fg, uint32_t bg){
    uint32_t x = (index % (w/CHAR_W))*CHAR_W;
    uint32_t y = (index / (w/CHAR_W))*CHAR_H;
    if(!(c >= 32 && c <= 126)) c = ' ';
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            buf[w * (y+layer) + x+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void fb_setChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            framebuffer[fb_real_w * ((y*CHAR_H)+layer) + (x*CHAR_W)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void buf_setChar(uint32_t *buf, uint32_t w, uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg){
    for(int layer = 0; layer < CHAR_H; layer++){
        for(int pixel = 0; pixel < CHAR_W; pixel++){
            buf[(w * ((y*CHAR_H)+layer))*CHAR_W + (x*CHAR_W)+pixel] = ((FONT[(int)c][layer] >> pixel) & 1) ? fg : bg;
        }
    }
}

void fb_write_xy(char* Buffer, int length, int x, int y, uint32_t fg, uint32_t bg){
    for(int i = 0; i < length; i++){
        fb_setChar(x+i, y, Buffer[i], fg, bg);
    }
}

void fb_clear(uint32_t color){
    for(uint32_t i = 0; i < fb_real_w*fb_real_h; i++){
        framebuffer[i] = color;
    }
}

void change_video_pointer(){
    fb_clear(0);
    framebuffer = (uint32_t *) framebuffer;
}

void swap_buffers(){
    memcpy(framebuffer, real_framebuffer, fb_width*fb_height*sizeof(uint32_t));
}

void set_backbuffer(uint8_t back_buf_state){
    use_backbuffer = back_buf_state;
    if(use_backbuffer){
        framebuffer = back_buffer;
    }
    else{
        framebuffer = real_framebuffer;
    }
}