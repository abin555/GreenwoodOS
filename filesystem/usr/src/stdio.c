#include <stdio.h>

struct FILE **fileListing;

int puts(const char *s){
    register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = (unsigned int) s;
	eax = 0x25;
	asm("int 0x80");
    return 0;
}

