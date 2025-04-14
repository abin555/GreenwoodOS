#ifndef PROCESSOR_STATE_H
#define PROCESSOR_STATE_H

#include "stdint.h"

struct processor_state {
    // Pushed by the interrupt request/routine handler
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	// Pushed by ISR handler if available
	uint32_t intr;
	uint32_t error;

	// Pushed by the processor
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;

	// Only pushed/popped on Ring 3 <-> Ring 0 switches
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed));

#endif