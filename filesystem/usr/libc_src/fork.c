#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int fork(void){
    register uint32_t eax asm("eax");
	eax = SYSCALL_FORK;	
	asm("int 0x80");
    return eax;
}