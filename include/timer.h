#ifndef TIMER_H
#define TIMER_H

#include "console.h"
#include "interrupts.h"
#include "pic.h"
#include "io.h"

extern uint32_t timer_ticks;

void delay(uint32_t msec);
void timer_init(uint32_t frequency);
void timer_callback();

#endif