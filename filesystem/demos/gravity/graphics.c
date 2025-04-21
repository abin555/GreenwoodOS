#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphics.h"
#include "object.h"

struct Window *window_init(int w, int h, char *title){
    struct Window *win = malloc(sizeof(struct Window));
    if(win == NULL) return NULL;

    win->vp = vp_open(w, h, title);
    win->width = w;
    win->height = h;
    win->bufSize = sizeof(uint32_t) * win->width * win->height;
    win->drawbuf = malloc(win->bufSize);
    vp_set_buffer(win->vp, win->drawbuf, win->bufSize);
    memset(win->drawbuf, 0, win->bufSize);
    return win;
}

void window_update(struct Window *win){
    if(win == NULL) return;
    vp_copy(win->vp);
}

void window_clear(struct Window *win){
    if(win == NULL) return;
    memset(win->drawbuf, 0, win->bufSize);
}

void gfx_drawObject(struct Window *win, struct Object *obj, bool drawVelVector, bool drawAccelVector){
    gfx_drawCircle(
        win,
        (int) normalize(
            -1.0, 1.0,
            obj->position.x,
            0,
            win->width
        ), 
        (int) normalize(
            -1.0, 1.0,
            obj->position.y,
            0,
            win->height
        ),
        (int) obj->radius,
        (Color) {.raw = 0xFFFFFF}
    );
}

void gfx_drawUnitVector(struct Window *win, Vec2 vec, int drawRadius, int x, int y, Color vColor, Color cColor){
    double max;
    double min;
    if(vec.x > vec.y){
        max = vec.x;
        min = vec.y;
    }
    else {
        max = vec.y;
        min = vec.x;
    }

    IVec2 dVec = {
        normalize(
            min,
            max,
            vec.x,
            -1*drawRadius,
            drawRadius
        ),
        normalize(
            min,
            max,
            vec.y,
            -1*drawRadius,
            drawRadius
        )
    };

    dVec.x += x;
    dVec.y += y;

    gfx_drawCircle(win, x, y, drawRadius, cColor);
    gfx_drawLine(win,
        (IVec2) {x, y},
        dVec,
        vColor
    );
}

void gfx_putPixel(struct Window *win, Color c, int x, int y){
    if(win == NULL) return;
    if(win->drawbuf == NULL) return;
    if(x < 0 || y < 0 || x >= win->width || y >= win->height) return;
    win->drawbuf[x+y*win->width] = c.raw;
}

void gfx_drawLine(struct Window *win, IVec2 a, IVec2 b, Color c){
    if(!(a.x >= 0 && a.x <= win->width && a.y >= 0 && a.y <= win->height)){
      return;
    }
    int dx = abs(b.x - a.x);
    int sx = a.x < b.x ? 1 : -1;
    int dy = -abs(b.y - a.y);
    int sy = a.y < b.y ? 1 : -1;
    int err = dx + dy;
    int e2;
  
    for(;;){
      if(a.x >= 0 && a.x <= win->width && a.y >= 0 && a.y <= win->height){
        win->drawbuf[a.x + a.y * win->width] = c.raw;
      }    
      if(a.x == b.x && a.y == b.y) break;
      e2 = 2 * err;
      if(e2 >= dy){err += dy; a.x += sx;}
      if(e2 <= dx){err += dx; a.y += sy;}
    }
}

void gfx_drawCircle(struct Window *win, int xCenter, int yCenter, int radius, Color c){
    int x, y, r2;    
    r2 = radius * radius;
    gfx_putPixel(win, c, xCenter, yCenter + radius);
    gfx_putPixel(win, c, xCenter, yCenter - radius);
    for (x = 1; x <= radius; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
        gfx_putPixel(win, c, xCenter + x, yCenter + y);
        gfx_putPixel(win, c, xCenter + x, yCenter - y);
        gfx_putPixel(win, c, xCenter - x, yCenter + y);
        gfx_putPixel(win, c, xCenter - x, yCenter - y);
    }
}