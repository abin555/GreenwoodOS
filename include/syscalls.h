#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "console.h"
#include "interrupts.h"
#include "memory.h"
#include "keyboard.h"
#include "timer.h"
#include "processes.h"
#include "programs/image.h"

void init_syscalls();

struct cpu_state syscalls_callback(struct cpu_state cpu, struct stack_state stack);

#endif