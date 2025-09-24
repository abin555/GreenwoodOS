#include <stdint.h>
#include <sys/stat.h>
#include <sys/syscall.h>

int mkdir(const char *pathname, int mode){
    mode = mode;
    register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) pathname;
	eax = SYSCALL_CREATDIR;
	asm("int 0x80");
    return 0;
}