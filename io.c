#include "io.h"

void WriteMem(uint32_t Address, uint32_t Value){
	*(volatile uint32_t*)Address = Value;
}

uint32_t ReadMem(uint32_t Address){
	return *(volatile uint32_t*)Address;
}