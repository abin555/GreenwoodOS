#ifndef TASK_H
#define TASK_H

#include "stdint.h"
#include "stdbool.h"
#include "dir.h"

#define G_MAX_TASKS 100
#define G_MT_maxDescriptors 10

typedef enum {
    G_ALWAYS = 1,
    G_ONFOCUS = 2,
    G_NOCHILD = 3,
    G_NEVER = 4
} GWOS_ScheduleType;

struct gwos_stack {
    void *start;
    void *end;
};

struct gwos_task {
    int id;
    int program_slot;
    uint8_t free;
    uint8_t status;
    char *name;

    uint8_t security_level;
    struct processor_state *state;

    GWOS_ScheduleType schedule_type;
    struct WINDOW *window;
    struct CONSOLE *console;
    struct DIRECTORY currentDirectory;
    void (*keyboard_event_handler)(char);
    void (*mouse_event_handler)(void);
    void (*end_callback)(void);
    bool own_window;
    bool own_console;
    int file_descs[G_MT_maxDescriptors];
    int num_used;

    struct gwos_stack stack;
    struct gwos_stack interrupt_stack;

    char *fpu_state;
};

#endif