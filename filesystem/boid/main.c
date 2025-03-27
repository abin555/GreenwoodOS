#include <stdio.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <stdbool.h>
#include "boid.h"

#define WIDTH 400
#define HEIGHT 400

struct Viewport *main_vp, *control_vp;
bool running;


void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE e){
    switch(e){
        case VP_EXIT:
            running = false;
            break;
    }
}

int main(int argc, char **argv){
    printf("Hello From Boid!\n");
    main_vp = vp_open(WIDTH, HEIGHT, "BOID Simulation");
    control_vp = vp_open(WIDTH, 100, "BOID Controls");
    control_vp->loc.y += HEIGHT;
    uint32_t *main_buf = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    uint32_t *control_buf = malloc(sizeof(uint32_t) * WIDTH * 100);
    vp_set_buffer(main_vp, main_buf, sizeof(uint32_t) * WIDTH * HEIGHT);
    vp_set_buffer(control_vp, control_buf, sizeof(uint32_t) * WIDTH * 100);
    vp_add_event_handler(main_vp, event_handler);
    vp_add_event_handler(control_vp, event_handler);
    running = true;
    while(running){

    }
    vp_close(control_vp);
    vp_close(main_vp);
    printf("Goodbye from Boid!\n");
}