#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/vp.h>

struct GraphWindow {
    struct Viewport *vp;
    uint32_t *drawbuf;
    int width;
    int height;
    char *title;
    float minX;
    float maxX;
    float minY;
    float maxY;
};

float normalize_float(float min, float max, float v, float a, float b);
int normalize_to_int(float min, float max, float v, int a, int b);
int normalize_int(int min, int max, int v, int a, int b);

struct GraphWindow *graph_open(int w, int h, char *title);
void graph_set_bounds(struct GraphWindow *window, float minX, float maxX, float minY, float maxY);
void graph_update(struct GraphWindow *window, float *points, int N);
void graph_close(struct GraphWindow *window);

#endif