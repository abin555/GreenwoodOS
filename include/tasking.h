#ifndef TASKING_H
#define TASKING_H

#include "stdint.h"
#include "stdbool.h"
#include "processor_state.h"
#include "memory.h"

struct gwos_task;

void tasking_init();
void tasking_start();

struct gwos_task *task_create(void *eip, int program_slot, uint8_t securityLevel, char *name, int argc, char *argv[]);

void task_initMemory(struct gwos_task *task);
void task_memInitStacks(struct gwos_task *task);
struct gwos_stack task_memCreateStack(int npages);
void task_endPen();
void task_resetState(struct gwos_task *task, void *eip, uint8_t security);
void task_restoreState(struct gwos_task *task);

struct gwos_task *task_getCurrent();
void task_saveState(struct gwos_task *task, struct processor_state *state);
void task_printState(struct processor_state *state);

#endif