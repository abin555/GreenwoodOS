#ifndef TASKING_H
#define TASKING_H

#include "stdint.h"
#include "stdbool.h"
#include "processor_state.h"

struct gwos_task;

void tasking_init();

struct gwos_task *task_getCurrent();
void task_saveState(struct gwos_task *task, struct processor_state *state);

#endif