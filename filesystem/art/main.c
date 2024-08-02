#include "libc.h"

struct WINDOW *win;

int main(int argc, char **argv){
    print("Opening ART Window\n");
    set_schedule(2);
    win = window_open("ART", 1);
    uint32_t *buf = win->backbuffer;
    uint32_t x = win->width/2;
    uint32_t y = win->height/2;
    int dx = 1;
    int dy = -1;
    uint32_t color = 0;
    uint32_t old_color = 0;

    int i = 0;
    while(1){
        i++;
        buf[x + y*win->width] = old_color; 
        old_color = color;
        color++;

        x += dx;
        y += dy;
        //fb_setPixel(x, y, color);
        buf[x + y*win->width] = color;
        
        if(x <= 0){
            dx = -dx;
            x = 1;
        }
        if(x >= win->width){
            dx = -dx;
            x = win->width - 1;
        }
        if(y <= 0){
            dy = -dy;
        }
        if(y >= win->height){
            dy = -dy;
            y = win->height - 1;
        }
        if(i > 5000){
            window_update();
            i = 0;
        }
    }
}