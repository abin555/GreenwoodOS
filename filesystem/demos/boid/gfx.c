#include <stdint.h>
#include <sys/vp.h>
#include "gfx.h"

int abs(int x){
  if(x < 0) return x * -1;
  return x;
}

void drawLine(struct Viewport *vp, int x1, int y1, int x2, int y2, uint32_t color){
  if(!(x1 >= 0 && x1 <= vp->loc.w && y1 >= 0 && y1 <= vp->loc.h)){
    return;
  }
  int dx = abs(x2 - x1);
  int sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1);
  int sy = y1 < y2 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for(;;){
    if(x1 >= 0 && x1 <= vp->loc.w && y1 >= 0 && y1 <= vp->loc.h){
      vp->backbuf[x1 + y1 * vp->loc.w] = color;
    }    
    if(x1 == x2 && y1 == y2) break;
    e2 = 2 * err;
    if(e2 >= dy){err += dy; x1 += sx;}
    if(e2 <= dx){err += dx; y1 += sy;}
  }
}

void drawTriangle(struct Viewport *vp, Vector2 v0, Vector2 v1, Vector2 v2, uint32_t color){
  drawLine(vp, (int) v0.x, (int) v0.y, (int) v1.x, (int) v1.y, color);
  drawLine(vp, (int) v1.x, (int) v1.y, (int) v2.x, (int) v2.y, color);
  drawLine(vp, (int) v2.x, (int) v2.y, (int) v0.x, (int) v0.y, color);
}