#ifndef IO_H
#define IO_H

#include "stdint.h"

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }

extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);

extern void outportl(uint16_t port, uint32_t value);
extern uint32_t inportl(uint16_t port);

extern void load_gdt();

static inline void outdw(uint16_t port, uint32_t data) {
	__asm__ volatile("out dx,eax" : : "a"(data), "d"(port));
}

static inline void outqw(uint16_t port, uint32_t data){
	__asm__ volatile("out edx,eax" :: "a"(data), "d"(port));
}

static inline uint32_t indw(uint16_t port) {
	uint32_t data;
	__asm__ volatile("in eax,dx" : "=a"(data) : "d"(port));
	return data;
}
static inline uint32_t inqw(uint16_t port){
	uint32_t data;
	__asm__ volatile("in eax,edx" : "=a"(data) : "d"(port));
	return data;
}

extern char program_mode;
extern void program_setds();
extern void normal_setds();
void set_program_segment();
void set_kernel_segment();

void reboot();

#endif