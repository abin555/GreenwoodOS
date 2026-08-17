#include "gfx.h"

void GFX_fromVP(sGFXctx *ctx, struct Viewport *vp){
    if(ctx == NULL || vp == NULL) return;

    ctx->buf = vp->backbuf;
    ctx->bufsize = vp->buf_size;
    ctx->bufwidth = vp->loc.w;
    ctx->bufheight = vp->loc.h;
}

void GFX_line(sGFXctx *ctx, uint32_t color, int ax, int ay, int bx, int by){
    if(ctx == NULL) return;
    if(!(ax >= 0 && bx <= ctx->bufwidth && ay >= 0 && by <= ctx->bufheight)){
      return;
    }
    int dx = abs(bx - ax);
    int sx = ax < bx ? 1 : -1;
    int dy = -abs(by - ay);
    int sy = ay < by ? 1 : -1;
    int err = dx + dy;
    int e2;

    for(;;){
      if(ax >= 0 && ax <= ctx->bufwidth && ay >= 0 && ay <= ctx->bufheight){
        ctx->buf[ax + ay * ctx->bufwidth] = color;
      }    
      if(ax == bx && ay == by) break;
      e2 = 2 * err;
      if(e2 >= dy){err += dy; ax += sx;}
      if(e2 <= dx){err += dx; ay += sy;}
    }
}

static inline void GFX_plot(sGFXctx *ctx, uint32_t color, int x, int y){
    if(x < 0 || y < 0 || x >= ctx->bufwidth || y >= ctx->bufheight) return;
    ctx->buf[x + y * ctx->bufwidth] = color;
}

void GFX_circle(sGFXctx *ctx, uint32_t color, int cx, int cy, int r){
    if(r < 0) return;

    int t1 = r / 16;
    int x  = r;
    int y  = 0;

    while(x >= y){
        GFX_plot(ctx, color, cx + x, cy + y);
        GFX_plot(ctx, color, cx + y, cy + x);
        GFX_plot(ctx, color, cx - y, cy + x);
        GFX_plot(ctx, color, cx - x, cy + y);
        GFX_plot(ctx, color, cx - x, cy - y);
        GFX_plot(ctx, color, cx - y, cy - x);
        GFX_plot(ctx, color, cx + y, cy - x);
        GFX_plot(ctx, color, cx + x, cy - y);

        y++;
        t1 += y;
        int t2 = t1 - x;
        if(t2 >= 0){
            t1 = t2;
            x--;
        }
    }
}
