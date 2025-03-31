#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/vp.h>
#include <sys/task.h>

#define WIDTH 300
#define HEIGHT 300

#define BUF_SIZE WIDTH * HEIGHT * sizeof(uint32_t)

struct Viewport *win;
uint32_t *backbuffer;

int running;

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event);

int main(int argc, char **argv){
    printf("Opening ART Window\n");
    set_schedule(ALWAYS);
    backbuffer = (uint32_t *) 0x6000;
    win = vp_open(WIDTH, HEIGHT, "ART");
    vp_add_event_handler(win, event_handler);
    if(win == NULL) return 1;
    uint32_t *buf = backbuffer;
    vp_set_buffer(win, buf, BUF_SIZE);
    uint32_t x = WIDTH/2;
    uint32_t y = HEIGHT/2;
    int dx = 1;
    int dy = -1;
    uint32_t color = 0;
    uint32_t old_color = 0;

    running = 1;

    int i = 0;
    while(running){
        i++;
        buf[x + y*WIDTH] = old_color; 
        old_color = color;
        color++;

        x += dx;
        y += dy;
        //fb_setPixel(x, y, color);
        buf[x + y*WIDTH] = color;
        
        if(x <= 0){
            dx = -dx;
            x = 1;
        }
        if(x >= WIDTH){
            dx = -dx;
            x = WIDTH - 1;
        }
        if(y <= 0){
            dy = -dy;
        }
        if(y >= HEIGHT){
            dy = -dy;
            y = HEIGHT - 1;
        }
        if(i > 5000){
            vp_copy(win);
            i = 0;
        }
    }
    vp_close(win);
}

void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE event){
    if(event == VP_FOCUSED || event == VP_MAXIMIZE){
        set_schedule(ALWAYS);
    }
    else if(event == VP_UNFOCUSED || event == VP_MINIMIZE){
        set_schedule(NEVER);
    }
    else if(event == VP_EXIT){
        running = 0;
    }
    
}