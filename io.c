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