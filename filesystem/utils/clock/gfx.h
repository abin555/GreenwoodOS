#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/vp.h>

typedef struct {
    uint32_t *buf;
    size_t bufsize;
    uint32_t bufwidth;
    uint32_t bufheight;
} sGFXctx;

void GFX_fromVP(sGFXctx *ctx, struct Viewport *vp);
void GFX_line(sGFXctx *ctx, uint32_t color, int ax, int ay, int bx, int by);
void GFX_circle(sGFXctx *ctx, uint32_t color, int cx, int cy, int r);

#endif