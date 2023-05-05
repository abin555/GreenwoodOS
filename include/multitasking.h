#ifndef MULTITASKING_H
#define MULTITASKING_H
#include "programs.h"
#include "memory.h"
#include "interrupts.h"
#include "timer.h"
#include "stdint.h"

struct task_registers{
    uint32_t esp;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;

    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    uint32_t eip;
};

struct task_state{
    struct task_registers registers;
    int8_t program_slot;//0-9 Represents a slot with a need to switch the program slot context; | value of -1 means this is a kernel task with not page change needed.
    char task_name[10];
    uint8_t slot_active;
    uint8_t slot_running;
    uint32_t stack_region;
};

#define MAX_TASKS 10
#define TASK_STACK_SIZE sizeof(uint32_t) * 0x100
uint8_t task_stack_array[MAX_TASKS][TASK_STACK_SIZE] __attribute__((aligned (4)));

int8_t task_running_idx;
struct task_state tasks[MAX_TASKS];

//Switch from active task to passed task pointer.
void switch_to_task(struct task_state* old_task, struct task_state* new_task);
//Create new task instance and execute it.
void start_task(uint32_t address, int8_t program_slot,  uint32_t arg, char* name);
//Remove task from the run queue
void stop_task(int8_t task_idx);
void task_end();
//Timer callback to switch tasks.
void task_callback();

void multitask_init();

#endif