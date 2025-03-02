#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "interrupts.h"
#include "window.h"
#include "console.h"
#include "memory.h"
#include "allocator.h"
#include "drive.h"
#include "program.h"
#include "keyboard.h"
#include "random.h"
#include "mouse.h"
#include "pcspeaker.h"
#include "viewport.h"
#include "rtc.h"
#include "vfs.h"

void init_syscalls();

struct cpu_state syscall_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused)));

#endif