#include <stdio.h>
#include <stdint.h>

void dprint(const char *msg){
	register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = (unsigned int) msg;
	eax = 0x25;
	asm("int $0x80");
}

__attribute__((section(".text")))
const char *msg = "Test Program\n";

int main(int argc, char **argv){
    dprint(msg);
    return 1;
}
