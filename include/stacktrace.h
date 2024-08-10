#ifndef STACKTRACE_H
#define STACKTRACE_H

#include "stdint.h"
#include "serial.h"

void print_stack_trace(uint32_t ebp, unsigned int maxFrames);

#endif