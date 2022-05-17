#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include "gcc_stdint.h"

extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short pos);

extern void outportl(uint16_t port, uint32_t value);
extern uint32_t inportl(uint16_t port);

static inline void outdw(uint16_t port, uint32_t data) {
	__asm__ volatile("out dx,eax" : : "a"(data), "d"(port));
}

static inline uint32_t indw(uint16_t port) {
	uint32_t data;
	__asm__ volatile("in eax,dx" : "=a"(data) : "d"(port));
	return data;
}

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void kreboot();
extern uint32_t * restore_kernel_addr;
extern void PROGA();

extern unsigned int *externalProgram;
#endif
