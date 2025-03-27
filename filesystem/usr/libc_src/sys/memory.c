#include <sys/memory.h>

void *memory_requestRegion(unsigned int size){
    register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = size;
	eax = 0x20;
	asm("int 0x80");
	return (void *) eax;
}
void memory_returnRegion(void *region, unsigned int size){
    register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	register unsigned int ecx asm("ecx");
	ecx = size;
	ebx = (unsigned int) region;
	eax = 0x2B;
	asm("int 0x80");
}
