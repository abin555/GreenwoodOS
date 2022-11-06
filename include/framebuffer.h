#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stdint.h"
#include "multiboot.h"
#include "font.h"

uint32_t fb_width, fb_height;
uint32_t* framebuffer;

uint32_t fb_terminal_w, fb_terminal_h;

void init_framebuffer(struct multiboot_tag_framebuffer* tagfb);
void fb_write_cell(uint32_t index, char c, uint32_t fg, uint32_t bg);
void fb_setPixel(uint32_t x, uint32_t y, uint32_t color);
void fb_setChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void fb_write_xy(char* Buffer, int length, int x, int y);
void fb_clear(uint32_t color);

void change_video_pointer();

#endif