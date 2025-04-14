#include "tasking.h"
#include "stdint.h"
#include "stddef.h"
#include "serial.h"
#include "cpu.h"
#include "task.h"

struct gwos_task gwos_tasks[G_MAX_TASKS];
int gwos_current_task;

int tasking_ready = 0;

void tasking_init(){
    print_serial("[TASK] Init Multitasking\n");
    //task_stack_base = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE * MAX_TASKS), PAGE_SIZE * MAX_TASKS, 0, STACK));
    //tasks = (struct task_state *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(sizeof(struct task_state) * MAX_TASKS), sizeof(struct task_state) * MAX_TASKS, 0, KERNEL));
    memset(gwos_tasks, 0, sizeof(struct gwos_task) * G_MAX_TASKS);
    MEM_printRegions();
    //print_serial("[TASK] Setting Stack! - 0x%x\n", (uint32_t *) task_stack_base);
    for(int i = 0; i < G_MAX_TASKS; i++){
        cpu_fxsave(gwos_tasks[i].fpu_state);
    }
    tasking_ready = 1;
}

struct gwos_task *task_getCurrent(){
    if(tasking_ready == 0) return NULL;

    return &gwos_tasks[gwos_current_task];
}

void task_saveState(struct gwos_task *task, struct processor_state *state){
    task->state = state;
    cpu_fxsave(task->fpu_state);
}