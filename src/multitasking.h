#ifndef MULTITASKING_H
#define MULTITASKING_H

#include "stdbool.h"
#include "memory.h"
#include "allocator.h"
#include "paging.h"
#include "interrupts.h"
#include "timer.h"
#include "io.h"
#include "serial.h"
#include "program.h"

#include "window.h"
#include "console.h"
#include "drive.h"

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

typedef enum {
    ALWAYS = 1,
    ONFOCUS = 2,
    NOCHILD = 3,
    NEVER = 4
} ScheduleType;

#define MT_maxDescriptors 10

struct task_file_association {
    int creator_fd;
    int new_fd;
};

struct task_file_ctx {
    int num_fds;
    struct task_file_association fds[];
};

struct task_state{
    struct task_registers registers;
    int program_slot;//0-9 Represents a slot with a need to switch the program slot context; | value of -1 means this is a kernel task with no page change needed.
    char *task_name;
    uint8_t slot_active;
    uint8_t slot_running;
    uint32_t stack_region;
    ScheduleType schedule_type;
    struct WINDOW *window;
    struct CONSOLE *console;
    struct DIRECTORY currentDirectory;
    void (*keyboard_event_handler)(char);
    bool own_window;
    bool own_console;
    int file_descs[MT_maxDescriptors];
    int num_used;
    int pid;//Unique program ID
    int waitpid;//If <= 0, don't worry about it. Otherwise, program does not run until program referenced by pid is ended.
} __attribute__((packed));

#define MAX_TASKS 30
#define TASK_STACK_SIZE 0x80000
extern uint8_t *task_stack_array[MAX_TASKS];

extern int8_t task_running_idx;
extern struct task_state tasks[MAX_TASKS];
extern bool task_lock;

//Switch from active task to passed task pointer.
void switch_to_task(struct task_state* old_task, struct task_state* new_task, int old_id, int new_id);
//Create new task instance and execute it.
int start_task(void *address, int8_t program_slot, int argc, char **argv, char* name, struct task_file_ctx *file_ctx);
//Remove task from the run queue
void stop_task(int8_t task_idx);
void task_end();
//Timer callback to switch tasks.
void task_callback();

void multitask_init();
void multitask_start();

void list_tasks();
int task_get_slot(int task_idx);
void set_schedule(ScheduleType type);
void set_schedule_task(int taskID, ScheduleType type);

int taskID_fromPID(int pid);
struct task_state *task_fromPID(int pid);

void task_yield();

int task_getFD(struct task_state *task);
int task_allocFD(struct task_state *task, int sysfd);
int task_getSysFD(struct task_state *task, int fd);
void task_freeFD(struct task_state *task, int fd);
int task_dupFD(struct task_state *task, int fd);

int fork();
int task_fork(struct task_state *task);
void save_task_state(struct task_state *task, struct cpu_state *cpu, struct stack_state *stack);
void tasking_setup_kernel_stack();

int task_getCurrentID();
struct task_state *task_getCurrent();
void os_yield();
#endif