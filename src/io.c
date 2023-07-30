#include "io.h"

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

void outb(uint16_t portid, uint8_t value){
	asm volatile("out dx, al": :"d" (portid), "a" (value & 0xFF));
}