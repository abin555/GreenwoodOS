#ifndef MOUSE_H
#define MOUSE_H

#include "console.h"
#include "interrupts.h"
#include "pic.h"
#include "memory.h"
#include "ps2.h"

extern uint16_t mouse_x;
extern uint16_t mouse_y;

struct cpu_state mouse_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));
void initialize_ps2_mouse();

#endif