#ifndef SOUNDBLASTER_16_H
#define SOUNDBLASTER_16_H

#include "audio.h"
#include "io.h"
#include "memory.h"
#include "console.h"
#include "paging.h"
#include "timer.h"
#include "interrupts.h"

struct cpu_state sb16_interrupt_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));
void init_soundblaster();

#endif