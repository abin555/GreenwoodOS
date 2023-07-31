#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stdint.h"
#include "memory.h"
#include "paging.h"
#include "multiboot.h"
#include "font.h"

extern uint32_t *fb_frontbuffer;
extern uint32_t *fb_backbuffer;
extern uint32_t fb_width;
extern uint32_t fb_height;

void fb_init(struct multiboot_tag_framebuffer* tagfb);

void fb_setPixel(uint32_t idx, uint32_t color);
void fb_putChar(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void fb_print(uint32_t x, uint32_t y, char *str);

#endif