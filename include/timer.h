#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"
#include "io.h"
#include "interrupts.h"
#include "serial.h"
#include "framebuffer.h"

struct timer_function{
    void (*attached_function)(void);
    uint32_t divisor;
};

extern uint32_t timer_ticks;
extern uint32_t timer_freq;
extern uint32_t timer_attached_functions_num;
extern struct timer_function timer_attached_functions[0xFF];

void timer_init(uint32_t frequency);
void timer_attach(int divisor, void* callback);
void timer_detach(void *callback);
struct cpu_state timer_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));
void wait(int ticks);

#endif