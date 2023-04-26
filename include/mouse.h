#ifndef MOUSE_H
#define MOUSE_H

#include "console.h"
#include "interrupts.h"
#include "pic.h"
#include "memory.h"
#include "io.h"
#include "filesystem.h"
#include "framebuffer.h"
#include "timer.h"


extern int mouse_x;
extern int mouse_y;
extern bool mouse_left_click;
extern bool mouse_right_click;
extern bool mouse_middle_click;
extern bool mouse_enable;
extern int move_x;
extern int move_y;

struct cpu_state mouse_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));
void initialize_ps2_mouse();

void mouse_draw();
void mouse_timer_routine();
void init_mouse();
#endif