#ifndef IO_H
#define IO_H

#include "stdint.h"

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }

//extern void outb(unsigned short port, unsigned char data);
static inline void outb(uint16_t portid, uint8_t value){
	asm volatile("out dx, al": :"d" (portid), "a" (value & 0xFF));
}

static inline uint8_t inb(uint16_t port){
	uint8_t data;
	__asm__ volatile("in al, dx" : "=a"(data) : "d"(port));
	return data;
}

void outportl(uint16_t port, uint32_t value);
uint32_t inportl(uint16_t port);

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

void reboot();

#endif