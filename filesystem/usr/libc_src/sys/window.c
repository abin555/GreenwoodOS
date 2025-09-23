#include <sys/window.h>
#include <sys/syscall.h>

struct WINDOW *window_open(char *name, uint32_t copyOnPrompt){
	register unsigned int eax asm("eax");
    register unsigned int ebx asm("ebx");
    register unsigned int ecx asm("ecx");
    register unsigned int edx asm("edx"); 
	ecx = (unsigned int) copyOnPrompt;   
    ebx = (unsigned int) name;
    eax = SYSCALL_WINDOW_OPEN;
    asm("int 0x80");
    struct WINDOW *win = (void *) eax;
	return win;
}

void window_close(struct WINDOW *window){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	eax = SYSCALL_WINDOW_CLOSE;
	ebx = (uint32_t) window;
	asm("int 0x80");
}

void window_update(){
	register uint32_t eax asm("eax");
	eax = SYSCALL_WINDOW_COPY;
	asm("int 0x80");
}

char window_getc(){
	
	char (*getc_blk)(void);
	register uint32_t eax asm("eax");
	eax = SYSCALL_GETC;
	asm("int 0x80");
	getc_blk = (char (*)(void)) eax;
	//return ((char (*)(void))eax)();
	return getc_blk();
	/*
	uint32_t c = 0;
	while(!c){
		register uint32_t eax asm("eax");
		eax = 0x05;
		asm("int 0x80");
		c = eax;
	}
	return (char) c;
	*/
}