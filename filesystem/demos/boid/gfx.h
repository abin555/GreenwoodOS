#ifndef GFX_H
#define GFX_H

struct Vector2 {
    float x;
    float y;
};

typedef struct Vector2 Vector2;

void drawLine(struct Viewport *vp, int x1, int y1, int x2, int y2, uint32_t color);
void drawTriangle(struct Viewport *vp, Vector2 v0, Vector2 v1, Vector2 v2, uint32_t color);

#endif