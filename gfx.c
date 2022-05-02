#include "gfx.h"

void gfx_line(u32 x1, u32 y1, u32 x2, u32 y2, u32 color){
    double yperx = (y2-y1)/(x2-x1);
    for(u32 x = x1; x<x2;x++){
        fb_setPixel(x,x*yperx+y1,color);
    }
}

/*
void gfx_hline(u32 x1, u32 x2, u32 y, u32 color){

}


void gfx_vline(u32 y1, u32 y2, u32 x, u32 color){

}
*/