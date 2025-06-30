#include "graph.h"
#include <string.h>

float normalize_float(float min, float max, float v, float a, float b){
    return (((b - a) * (float) (v - min)) / (float) (max - min)) + a;
}

int normalize_to_int(float min, float max, float v, int a, int b){
    return (((b - a) * (int) (v - min)) / (int) (max - min)) + a;
}

int normalize_int(int min, int max, int v, int a, int b){
    return (((b - a) * (int) (v - min)) / (int) (max - min)) + a;
}

struct GraphWindow *graph_open(int w, int h, char *title){
    struct GraphWindow *graph = malloc(sizeof(struct GraphWindow));
    graph->width = w;
    graph->height = h;
    graph->title = title;
    graph->vp = vp_open(graph->width, graph->height, graph->title);
    graph->drawbuf = malloc(sizeof(uint32_t) * graph->width * graph->height);
    vp_set_buffer(graph->vp, graph->drawbuf, sizeof(uint32_t) * graph->width * graph->height);
    return graph;
}

void graph_set_bounds(struct GraphWindow *window, float minX, float maxX, float minY, float maxY){
    if(window == NULL) return;
    window->minX = minX;
    window->maxX = maxX;
    window->minY = minY;
    window->maxY = maxY;
}

void graph_update(struct GraphWindow *window, float *points, int N){
    if(window == NULL) return;
    if(points == NULL) return;

    int width = window->width;
    int height = window->height;
    uint32_t *buf = window->drawbuf;

    memset(window->drawbuf, 0, window->width * window->height * sizeof(uint32_t));

    float max_mag = 0.0f;
    for (int i = 0; i < N; i++) {
        if (points[i] > max_mag) max_mag = points[i];
    }
    if (max_mag == 0.0f) return;  // prevent divide-by-zero

    for (int i = 0; i < width; i++) {
        int x = (i * N) / width;
        if (x >= N) x = N - 1;

        float normalized = points[x] / max_mag;
        int y = height - (int)(normalized * height);

        if (y < 0) y = 0;
        if (y >= height) y = height - 1;
        
        //printf("(%d/%d, %d)\n", i, x, y);

        //buf[i + (y * width)] = 0xFF0000;  // red pixel
        for(int j = y; j < height; j++){
            buf[i + j * width] = 0xFF0000;
        }
    }
    vp_copy(window->vp);
}

void graph_close(struct GraphWindow *window){
    if(window == NULL) return;
    vp_close(window->vp);
    free(window->drawbuf);
    free(window);
}
