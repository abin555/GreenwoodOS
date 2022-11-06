#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "console.h"
#include "interrupts.h"
#include "memory.h"
#include "keyboard.h"

void init_syscalls();

void syscalls_callback(struct cpu_state cpu, struct stack_state stack);

#endif