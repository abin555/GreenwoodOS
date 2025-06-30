#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/vp.h>
#include "tools.h"

typedef struct Color {
    union {
        uint32_t raw;
        uint8_t rgb[4];
    };
} Color;

struct Window {
    struct Viewport *vp;
    uint32_t *drawbuf;
    int width;
    int height;
    uint32_t bufSize;
    int shouldClose;
};

struct Object;

struct Window *window_init(int w, int h, char *title);
void window_update(struct Window *win);
void window_clear(struct Window *win);
void gfx_drawObject(struct Window *win, struct Object *obj, bool drawVelVector, bool drawAccelVector);
void gfx_drawUnitVector(struct Window *win, Vec2 vec, int drawRadius, int x, int y, Color vColor, Color cColor);
void gfx_putPixel(struct Window *win, Color c, int x, int y);
void gfx_drawLine(struct Window *win, IVec2 a, IVec2 b, Color c);
void gfx_drawCircle(struct Window *win, int xCenter, int yCenter, int radius, Color c);


#endif