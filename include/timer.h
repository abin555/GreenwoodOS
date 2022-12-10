#ifndef TIMER_H
#define TIMER_H

#include "interrupts.h"
#include "stdint.h"
#include "io.h"
#include "console.h"

extern uint32_t timer_ticks;
extern uint32_t timer_freq;

void delay(uint32_t msec);

void init_timer(uint32_t frequency);

struct cpu_state timer_callback(struct cpu_state cpu, struct stack_state stack);

struct timer_function{
    void (*attached_function)(void);
    uint32_t divisor;
};

struct timer_function timer_attached_functions[0xFF];
extern uint32_t timer_attached_functions_num;

void timer_attach(int divisor, void* callback);

#endif