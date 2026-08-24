#ifndef THREAD_H
#define THREAD_H

#include "multitasking.h"

struct Task;

struct Thread {
    struct Task *owner_task;
    struct task_registers registers;
    void *stack_region;
    ScheduleType schedule_type;
    uint64_t cpu_usage_count;
};


#endif