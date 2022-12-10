#ifndef IMAGE_H
#define IMAGE_H


#include "framebuffer.h"
#include "Chad.h"
#include "memory.h"
#include "filesystem.h"
#include "console.h"
#include "processes.h"


extern int image_width;
extern int image_height;

struct image_buffer_definition{
    uint8_t* buffer;
    uint32_t width;
    uint32_t height;
};

extern struct image_buffer_definition image_buffer_def;

void image_callback();
void draw_image(int x, int y);

void image_load_buffer(uint32_t w, uint32_t h, uint8_t* buffer);
void draw_buffer(int x, int y);

#endif