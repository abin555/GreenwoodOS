#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

struct Bitmap{
    uint8_t *file;
    uint32_t *bitmap;
    uint32_t width;
    uint32_t height;
};

struct WINDOW;

struct Bitmap loadBitmap(char *filename);
void drawBitmap(int x, int y, struct Bitmap *bitmap, struct WINDOW *window);

#endif