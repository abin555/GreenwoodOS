#ifndef SCREEN_H
#define SCREEN_H
#include "multiboot.h"

int init_fb(struct multiboot_tag_framebuffer *tagfb);

#endif