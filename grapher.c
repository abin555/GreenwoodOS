#include "grapher.h"

void draw_regions(){
    gfx_vline(0,fb_height/2, fb_width-250, 0xFFFFFF);
    gfx_hline(fb_width-250, fb_width,fb_height/2, 0xFFFFFF);
}

void grapher_entry(){
    while(1){
        draw_regions();
    }
}