#ifndef GFX_H
#define GFX_H

#include "stdint.h"
#include "allocator.h"
#include "window.h"
#include "desktop_shared.h"

void drawRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
);

void fillRect(
    uint32_t outerColor,
    uint32_t innerColor,
    int x1,
    int y1,
    int x2,
    int y2,
    uint32_t *buf,
    uint32_t buf_width
);

#endif