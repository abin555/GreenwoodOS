#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stdint.h"
#include "memory.h"
#include "multiboot.h"
#include "font.h"

uint32_t fb_width, fb_height, fb_real_w, fb_real_h;

uint32_t* framebuffer;
uint32_t* real_framebuffer;
uint32_t back_buffer[1280*720*sizeof(uint32_t)];
//uint32_t *back_buffer;

uint32_t fb_terminal_w, fb_terminal_h;
uint8_t use_backbuffer = 0;

void init_framebuffer(struct multiboot_tag_framebuffer* tagfb);
void init_backbuffer();
void fb_write_cell(uint32_t index, char c, uint32_t fg, uint32_t bg);
void fb_drawChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void fb_setPixel(uint32_t x, uint32_t y, uint32_t color);
void fb_setChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void fb_write_xy(char* Buffer, int length, int x, int y, uint32_t fg, uint32_t bg);
void fb_clear(uint32_t color);
void set_backbuffer(uint8_t back_buf_state);
void swap_buffers();

void change_video_pointer();

#endif