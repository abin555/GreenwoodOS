#include "screen.h"
#include <sys/vp.h>
#include <sys/task.h>
#include <stdlib.h>
#include <string.h>

struct Viewport *window;
uint32_t *drawbuf;
extern int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    switch(event){
        case VP_EXIT:
            running = 0;
            set_schedule(ALWAYS);
            break;
        case VP_MAXIMIZE:
            set_schedule(ALWAYS);
            break;
        case VP_MINIMIZE:
            set_schedule(NEVER);
            break;
        default:
            return;
    }
}

void initScreen(){
    running = 1;
    window = vp_open(
        64,
        128,
        "TETRIS"
    );
    drawbuf = malloc(sizeof(uint32_t) * window->loc.h * window->loc.w);
    vp_set_buffer(window, drawbuf, sizeof(uint32_t) * window->loc.w * window->loc.h);
    vp_add_event_handler(window, event_handler);
}

void writePixel(uint8_t x, uint8_t y, OLED_color color){
    drawbuf[(y-64) + x*window->loc.w] = (color == COLOR_WHITE) ? 0xFFFFFF : 0x000000;
}

void invertDisplay(uint8_t inv){

}

void showScreen(){
    vp_copy(window);
}

void clearDisplay(){
    memset(drawbuf, 0, sizeof(uint32_t) * window->loc.w * window->loc.h);
}