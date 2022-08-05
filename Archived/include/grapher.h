#ifndef GRAPHER_H
#define GRAPHER_H
#include "frame_buffer.h"
#include "gfx.h"
#include "keyboard.h"
#include "system_calls.h"

#define Proportion_Bottom 4
#define Proportion_Right 5

#define Axis_Proportion 3

#define MAX_FORMULAS 4
#define MAX_FORMULA_LEN 80

struct DATA_Settings{
    int left_bound;
    int right_bound;
    int top_bound;
    int bottom_bound;
    int xscale;
    int yscale;
    char settings;
    double step;
} settings_data;

struct interface_struct{
    unsigned int last_ASCII_P;
    unsigned int select_region;

} grapher_interface;

struct formula{
    int type;
    unsigned int ex_pointer;
    char expression[MAX_FORMULA_LEN];
} formulas[MAX_FORMULAS];

int previousAscii_Pointer;
int previousKey_Pointer;

int axis_center_x;
int axis_center_y;

void draw_settings_pane();
void draw_axis();
void draw_graph();
void clear_region();
void draw_regions();
void grapher_entry();
void grapher_key_handler(char key);
void grapher_draw_formulas();
void plot_point(float x, float y);
float sqrt(float x);

#endif