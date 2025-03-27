#include <stdint.h>
#include <sys/stat.h>

int mkdir(const char *pathname, int mode){
    mode = mode;
    register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) pathname;
	eax = 0x1A;
	asm("int 0x80");
    return 0;
}