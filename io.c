#include "io.h"

unsigned char *INT_Software_Value = (unsigned char *)0x10;

void software_interrupt(unsigned char interrupt){
	INT_Software_Value[0] = interrupt;
	software_int();
}

void WriteMem(uint32_t Address, uint32_t Value){
	*(volatile uint32_t*)Address = Value;
}

uint32_t ReadMem(uint32_t Address){
	return *(volatile uint32_t*)Address;
}
void outportl(uint16_t portid, uint32_t value)
{
	asm volatile("out dx, eax": :"d" (portid), "a" (value));
}
uint32_t inportl(uint16_t portid)
{
	uint32_t ret;
	asm volatile("in eax, dx":"=a"(ret):"d"(portid));
	return ret;
}

