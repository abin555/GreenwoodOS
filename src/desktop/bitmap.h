#ifndef BITMAP_H
#define BITMAP_H

#include "stdint.h"
#include "allocator.h"
#include "window.h"
#include "drive.h"
#include "desktop_shared.h"

struct Bitmap{
    uint8_t *file;
    uint32_t *bitmap;
    uint32_t width;
    uint32_t height;
};

struct Bitmap loadBitmap(char *filename);
void drawBitmap(int x, int y, struct Bitmap bitmap, struct WINDOW *window);

#endif