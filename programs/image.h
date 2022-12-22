#ifndef IMAGE_H
#define IMAGE_H


#include "framebuffer.h"
#include "memory.h"
#include "filesystem.h"
#include "console.h"
#include "processes.h"

struct image_buffer_definition{
    uint8_t* buffer;
    uint32_t width;
    uint32_t height;
};

extern struct image_buffer_definition image_buffer_def;


void image_load_buffer(uint32_t w, uint32_t h, uint8_t* buffer);
void draw_buffer(int x, int y);

#endif