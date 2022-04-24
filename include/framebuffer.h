#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "types.h"
#include "multiboot.h"

u32 fb_width;
u32 fb_height;
u64* fb;
int fb_terminal_w;
int fb_terminal_h;

void fb_setPixel(u32 x, u32 y, u32 color);

void init_fb(struct multiboot_tag_framebuffer *tagfb);

void fb_write_cell(u32 x, u32 y, char c, u32 fb, u32 bg);

#endif