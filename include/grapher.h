#ifndef GRAPHER_H
#define GRAPHER_H
#include "frame_buffer.h"
#include "gfx.h"
#include "keyboard.h"
#include "system_calls.h"

struct DATA_Settings{
    int left_bound;
    int right_bount;
    int top_bound;
    int bottom_bound;
} settings_data;

struct formula{
    int type;
    char expression[80];
} formulas[4];

int previousAscii_Pointer;
int previousKey_Pointer;

void draw_settings_pane();
void draw_axis();
void draw_graph();
void draw_regions();
void grapher_entry();

#endif