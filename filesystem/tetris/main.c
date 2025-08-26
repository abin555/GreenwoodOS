#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>

int running;

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

int main(int argc, char **argv){
    printf("TETRIS\n");
    running = 1;
    struct Viewport *window = vp_open(
        8*26,
        20*8,
        "TETRIS"
    );
    uint32_t *drawbuf = malloc(sizeof(uint32_t) * window->loc.w * window->loc.h);
    vp_set_buffer(window, drawbuf, sizeof(uint32_t) * window->loc.w * window->loc.h);
    vp_add_event_handler(window, event_handler);

    while(running){
        yield();
    }
    vp_close(window);
}