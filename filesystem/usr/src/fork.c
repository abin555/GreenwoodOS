#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int fork(void){
    register uint32_t eax asm("eax");
	eax = 0x3C;	
	asm("int 0x80");
    return eax;
}