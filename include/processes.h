#ifndef PROCESS_H
#define PROCESS_H

#include "memory.h"
#include "console.h"
#include "interrupts.h"
#include "stdint.h"
#include <stdarg.h>

extern uint8_t current_process;

struct process_task{
    void (*program)(uint8_t process_id, uint32_t args[10]);
    uint8_t running     : 1;
    uint8_t enabled     : 1;
    struct cpu_state cpu;
    uint32_t addr;
    uint32_t args[10];
};

struct process_task processes[0xF];
uint8_t process_num;
uint8_t add_process(void* program, int args, ...);
void kill_process(uint8_t process_id);

void process_scheduler();

void process_timed_exit();

void process_yield();

extern void restore_process(struct cpu_state cpu, uint32_t addr);
#endif