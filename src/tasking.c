#include "tasking.h"
#include "stdint.h"
#include "stddef.h"
#include "serial.h"
#include "cpu.h"
#include "task.h"
#include "allocator.h"
#include "utils.h"
#include "timer.h"

struct gwos_task gwos_tasks[G_MAX_TASKS];
int gwos_current_task;

int tasking_process_counter;
int tasking_ready = 0;
int scheduler_started = 0;
int tasking_lock;

void tasking_init(){
    print_serial("[TASK] Init Multitasking\n");
    //task_stack_base = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE * MAX_TASKS), PAGE_SIZE * MAX_TASKS, 0, STACK));
    //tasks = (struct task_state *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(sizeof(struct task_state) * MAX_TASKS), sizeof(struct task_state) * MAX_TASKS, 0, KERNEL));
    tasking_lock = 1;
    //memset(gwos_tasks, 0, sizeof(struct gwos_task) * G_MAX_TASKS);
    MEM_printRegions();
    //print_serial("[TASK] Setting Stack! - 0x%x\n", (uint32_t *) task_stack_base);
    
    for(int i = 0; i < G_MAX_TASKS; i++){
        gwos_tasks[i].free = 1;
        gwos_tasks[i].fpu_state = malloc(512);
    }
    
    tasking_process_counter = 0;
    scheduler_started = 0;
    print_serial("[TASK] Init Multitasking Complete\n");
}

void taskSchedule(){
    print_serial("[TASKING] Switching Task\n");
    if(!scheduler_started){
        scheduler_started = 1;
        print_serial("[TASKING] Freeing Scheduler\n");
    }
}

void tasking_start(){
    tasking_lock = 0;
    tasking_ready = 1;
    gwos_current_task = 0;
    scheduler_started = 0;
	timer_attach(1, taskSchedule);
}

struct gwos_task *task_create(void *eip, int program_slot, uint8_t securityLevel, char *name){
    struct gwos_task *new_task = NULL;
    struct gwos_task *parent_task = task_getCurrent();
    for(int i = 0; i < G_MAX_TASKS; i++){
        if(gwos_tasks[i].free == 1){
            new_task = &gwos_tasks[i];
            break;
        }
    }
    if(new_task == NULL) return NULL;

    print_serial("[TASKING] Creating New Task \"%s\" with eip @ 0x%x using slot %d and security %d\n", name, eip, program_slot, securityLevel);

    new_task->id = tasking_process_counter++;
    new_task->program_slot = program_slot;
    new_task->free = 0;
    new_task->status = 1;
    
    if(name == NULL){
        new_task->name = "UNKNOWN TASK NAME\0";
    }
    else{
        int len = strlen(name);
        char *namebuf = malloc(len+1);
        memset(namebuf, 0, len+1);
        memcpy(namebuf, name, len);
        new_task->name = namebuf;
    }

    new_task->security_level = securityLevel;
    new_task->schedule_type = G_ALWAYS;
    
    if(parent_task != NULL){
        new_task->window = parent_task->window;
        new_task->console = parent_task->console;
        memcpy(new_task->currentDirectory.path, parent_task->currentDirectory.path, sizeof(new_task->currentDirectory.path));
    }
    else{
        new_task->window = NULL;
        new_task->console = NULL;
        const char defaultPath[] = "/A/";
        memcpy(new_task->currentDirectory.path, (char *) defaultPath, sizeof(defaultPath));
    }

    new_task->keyboard_event_handler = NULL;
    new_task->mouse_event_handler = NULL;
    new_task->end_callback = NULL;

    new_task->own_window = false;
    new_task->own_window = false;
    
    for(int j = 0; j < G_MT_maxDescriptors; j++){
        new_task->file_descs[j] = -1;
    }
    new_task->num_used = 0;    


    //new_task->state->eip = (uint32_t) eip;
    task_initMemory(new_task);
    task_resetState(new_task, eip, securityLevel);
    //cpu_fxsave(new_task->fpu_state);
    //TODO: Stack & State Management

    print_serial("[TASKING] Created New Task \"%s\" with pid %d using slot %d and security %d\n", new_task->name, new_task->id, new_task->program_slot, new_task->security_level);
    return new_task;
}

void task_initMemory(struct gwos_task *task){
    task_memInitStacks(task);
}

void task_resetState(struct gwos_task *task, void *eip, uint8_t security){
    struct processor_state *state;
    if(security == 3 && task->security_level > 0){
        state = (struct processor_state *) (task->interrupt_stack.end - sizeof(struct processor_state));
    }
    else{
        state = (struct processor_state *) (task->stack.end - sizeof(struct processor_state));
    }

    task->state = state;

    memset(task->state, 0, sizeof(struct processor_state));
    state->eflags = 0x200;
    state->esp = (uint32_t) task->state;

    if(security == 0){
        state->cs = 0x8 | 0;
		state->ss = 0x10 | 0;
		state->ds = 0x10 | 0;
		state->es = 0x10 | 0;
    }
    else{
        state->cs = 0x18 | 3;
		state->ss = 0x20 | 3;
		state->ds = 0x20 | 3;
		state->es = 0x20 | 3;
    }

    if(security <= 2)
	{
		state->eflags |= 0x3000; // IOPL 3
	}

    state->eip = (uint32_t) eip;
}

extern struct tss_state tss;

void task_restoreState(struct gwos_task *task){
    if(task == NULL){
        print_serial("[TASK] [PANIC] Attempting to restore null task\n");
        while(1){

        }
    }
    print_serial("[TASK] Restoring State of task 0x%x, tss is at 0x%x to esp 0x%x\n", task, &tss, task->interrupt_stack.end);

    tss.esp0 = (uint32_t) task->interrupt_stack.end;
    cpu_fxrstor(task->fpu_state);
}

void task_memInitStacks(struct gwos_task *task){
    if(task->security_level != 3){//KERNEL
        task->interrupt_stack = task_memCreateStack(2);
    }
    else{
        task->interrupt_stack.start = 0;
        task->interrupt_stack.end = 0;
    }

    if(task->security_level == 3){//Kernel
        task->stack = task_memCreateStack(1);
    }
    else{
        task->stack = task_memCreateStack(2);
    }
}

struct gwos_stack task_memCreateStack(int npages){
    struct gwos_stack stack;
    uint32_t size = PAGE_SIZE * npages;
    stack.start = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(size), size, 0, STACK));
    stack.end = stack.start + size;
    return stack;
}

struct gwos_task *task_getCurrent(){
    if(tasking_ready == 0 || !scheduler_started) return NULL;
    print_serial("[TASK] Current Task #%d - pid %d\n", gwos_current_task, gwos_tasks[gwos_current_task].id);
    return &gwos_tasks[gwos_current_task];
}

void task_saveState(struct gwos_task *task, struct processor_state *state){
    task->state = state;
    cpu_fxsave(task->fpu_state);
}

void task_printState(struct processor_state *state){
    print_serial("Processor State:\nGS: 0x%x\nFS: 0x%x\nES: 0x%x\nDS: 0x%x\nEAX: 0x%x\nEBX: 0x%x\nECX: 0x%x\nEDX: 0x%x\nEBP: 0x%x\nINTR: 0x%x\nEIP: 0x%x\n",
        state->gs,
        state->fs,
        state->es,
        state->ds,
        state->eax,
        state->ebx,
        state->ecx,
        state->edx,
        state->intr,
        state->eip
    );
}