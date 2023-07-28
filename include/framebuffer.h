#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stdint.h"
#include "paging.h"
#include "multiboot.h"

extern uint32_t *fb_frontbuffer;
extern uint32_t fb_width;
extern uint32_t fb_height;

void fb_init(struct multiboot_tag_framebuffer* tagfb);

void fb_setPixel(uint32_t idx, uint32_t color);

#endif