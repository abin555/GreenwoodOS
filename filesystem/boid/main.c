#include <stdio.h>
#include <stdlib.h>
#include <sys/vp.h>
#include <stdbool.h>
#include <string.h>
#include <sys/task.h>
#include "boid.h"
#include "gfx.h"

#define WIDTH 400
#define HEIGHT 400

#define BOIDS 100

struct Viewport *main_vp;
bool running;


void event_handler(struct Viewport *vp, VIEWPORT_EVENT_TYPE e){
    switch(e){
        case VP_EXIT:
            running = false;
            break;
        case VP_MINIMIZE:
            set_schedule(NEVER);
            break;
        case VP_MAXIMIZE:
            set_schedule(ALWAYS);
            break;
    }
}

int GetRandomValue(int l, int r){
    int d = (0 - l) + r;
    int v = (rand() % d) - l;
    return v;
}

int main(int argc, char **argv){
    printf("Hello From Boid!\n");
    main_vp = vp_open(WIDTH, HEIGHT, "BOID Simulation");
    uint32_t *main_buf = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
    //uint32_t *control_buf = malloc(sizeof(uint32_t) * WIDTH * 100);
    vp_set_buffer(main_vp, main_buf, sizeof(uint32_t) * WIDTH * HEIGHT);
    //vp_set_buffer(control_vp, control_buf, sizeof(uint32_t) * WIDTH * 100);
    vp_add_event_handler(main_vp, event_handler);
    //vp_add_event_handler(control_vp, event_handler);

    Boid *flock[BOIDS];

    for(int i = 0; i < BOIDS; i++){
        flock[i] = newBoid(
            (Vector2){
                GetRandomValue(0, WIDTH),
                GetRandomValue(i, HEIGHT)
            },
            (Vector2){
                20,
                20
            },
            GetRandomValue(0, 6),
            1
        );
    }
 
    //FILE *serial = fopen("/-/dev/serial", "w");

    running = true;
    int ticks = 0;
    while(running){
        if(ticks % 10 == 0){
            for(int i = 0; i < BOIDS; i++){
                updateBoid(flock[i], flock, BOIDS, ((double) ticks) / 100.0f);
            }

            memset(main_buf, 0, sizeof(uint32_t) * WIDTH * HEIGHT);
            //drawLine(main_vp, 0, 0, WIDTH, HEIGHT, 0xFFFFFF);

            for(int i = 0; i < BOIDS; i++){
                drawBoid(flock[i], main_vp);
            }
            vp_copy(main_vp);
        }
        ticks++;
    }
    //fclose(serial);

    for(int i = 0; i < BOIDS; i++){
        free(flock[i]);
    }

    //vp_close(control_vp);
    vp_close(main_vp);
    printf("Goodbye from Boid!\n");
}