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

int fstat(int fd, struct stat *statbuf){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) statbuf;
	ebx = (uint32_t) fd;
	eax = SYSCALL_FSTAT;
	asm("int 0x80");
    return 0;
}