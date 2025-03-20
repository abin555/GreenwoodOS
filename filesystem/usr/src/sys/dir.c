#include <sys/dir.h>
#include <stdint.h>

int changeDirectory(char *change){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) change;
	eax = 0x12;
	asm("int 0x80");
	return eax;
}

char *getDirectory(){
	register uint32_t eax asm("eax");
	eax = 0x13;
	asm("int 0x80");
	return (char *) eax;
}

void printDirectoryContents(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) path;
	eax = 0x14;
	asm("int 0x80");
}