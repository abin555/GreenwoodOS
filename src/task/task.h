#ifndef TASK_H
#define TASK_H

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

#include "multitasking.h"

#include "thread.h"
#include "stdint.h"

#define MAX_THREADS 10

struct Task {
    size_t thread_count;
    struct Thread *threads[MAX_THREADS];
    char *name;
    ScheduleType schedule_type;
    struct WINDOW *window;
    struct CONSOLE *console;
    struct DIRECTORY currentDirectory;
    bool own_window;
    bool own_console;
    char auto_stdinout;
    int file_descs[MT_maxDescriptors];
    int num_used;
    int pid;
    int waitpid;
};

#endif