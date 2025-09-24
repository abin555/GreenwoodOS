#include <sys/io.h>
#include <stdint.h>
#include <sys/syscall.h>

int open(char *path, int flags){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ecx = (uint32_t) flags;
	ebx = (uint32_t) path;
	eax = SYSCALL_OPEN;
	asm("int 0x80");
	return (int) eax;
}

void close(int fd){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ebx = (uint32_t) fd;
	eax = SYSCALL_CLOSE;
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
	eax = SYSCALL_READ;
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
	eax = SYSCALL_WRITE;
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
	eax = SYSCALL_SEEK;
	asm("int 0x80");
	return (int) eax;
}

int creat(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ebx = (uint32_t) path;
	eax = SYSCALL_CREAT;
	asm("int 0x80");
	return (int) eax;
}

int ftruncate(int fd, unsigned int length){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	ecx = (uint32_t) length;
	ebx = (uint32_t) fd;
	eax = SYSCALL_TRUNCATE;
	asm("int 0x80");
	return (int) eax;
}