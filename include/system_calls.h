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

void syscall_callback(void *t);

#endif