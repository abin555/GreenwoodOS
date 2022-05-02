#ifndef GFX_H
#define GFX_H

#include "frame_buffer.h"
#include "types.h"

void gfx_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);
void gfx_hline(u32 x1, u32 x2, u32 y, u32 color);
void gfx_vline(u32 y1, u32 y2, u32 x, u32 color);


#endif