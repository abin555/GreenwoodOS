#include <sys/io.h>
#include <stdint.h>

int open(char *path, int flags){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ecx = (uint32_t) flags;
	ebx = (uint32_t) path;
	eax = 0x35;
	asm("int 0x80");
	return (int) eax;
}

void close(int fd){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ebx = (uint32_t) fd;
	eax = 0x36;
	asm("int 0x80");
}

int read(int fd, void *buf, int nbytes){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = (uint32_t) nbytes;
	ecx = (uint32_t) buf;
	ebx = (uint32_t) fd;
	eax = 0x37;
	asm("int 0x80");
	return (int) eax;
}

int write(int fd, void *buf, int nbytes){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = (uint32_t) nbytes;
	ecx = (uint32_t) buf;
	ebx = (uint32_t) fd;
	eax = 0x38;
	asm("int 0x80");
	return (int) eax;
}

int lseek(int fd, int off, int whence){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = (uint32_t) whence;
	ecx = (uint32_t) off;
	ebx = (uint32_t) fd;
	eax = 0x39;
	asm("int 0x80");
	return (int) eax;
}

int creat(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ebx = (uint32_t) path;
	eax = 0x3A;
	asm("int 0x80");
	return (int) eax;
}