#ifndef GRAPHER_H
#define GRAPHER_H
#include "frame_buffer.h"
#include "gfx.h"
#include "keyboard.h"
#include "system_calls.h"

#define Proportion_Bottom 4
#define Proportion_Right 5

#define Axis_Proportion 3

struct DATA_Settings{
    int left_bound;
    int right_bound;
    int top_bound;
    int bottom_bound;
    int xscale;
    int yscale;
} settings_data;

struct formula{
    int type;
    char expression[80];
} formulas[4];

int previousAscii_Pointer;
int previousKey_Pointer;

int axis_center_x;
int axis_center_y;

void draw_settings_pane();
void draw_axis();
void draw_graph();
void draw_regions();
void grapher_entry();
void plot_point(int x, int y);

#endif