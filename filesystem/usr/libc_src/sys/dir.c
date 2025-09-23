#include <sys/dir.h>
#include <stdint.h>
#include <sys/syscall.h>

int changeDirectory(char *change){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) change;
	eax = SYSCALL_CHDIR;
	asm("int 0x80");
	return eax;
}

char *getDirectory(){
	register uint32_t eax asm("eax");
	eax = SYSCALL_GET_DIR;
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

struct DirectoryListing getDirectoryListing(char *path){
	struct DirectoryListing (*listFunc)(char *);
	register uint32_t eax asm("eax");
	eax = SYSCALL_VFS_LISTDIR;
	asm("int 0x80");
	listFunc = (struct DirectoryListing (*)(char *)) eax;
	return listFunc(path);
}

void creatdir(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) path;
	eax = SYSCALL_CREATDIR;
	asm("int 0x80");
}