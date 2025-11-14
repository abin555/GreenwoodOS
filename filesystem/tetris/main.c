#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/vp.h>
#include <sys/task.h>
#include "block.h"
#include "game.h"

int running;
struct Tetris tetris = {
    {0},
    0,
    0
};

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
        64,
        128,
        "TETRIS"
    );
    uint32_t *drawbuf = malloc(sizeof(uint32_t) * window->loc.w * window->loc.h);
    vp_set_buffer(window, drawbuf, sizeof(uint32_t) * window->loc.w * window->loc.h);
    vp_add_event_handler(window, event_handler);

    for(int i = 0; i < 4; i++){
        tetris.board.b[5][19-i] = 1;
    }

    while(running){
        tetris_draw(drawbuf, &tetris);
        vp_copy(window);
        yield();
    }
    vp_close(window);
}