#include <sys/window.h>

struct WINDOW *window_open(char *name, uint32_t copyOnPrompt){
	register unsigned int eax asm("eax");
    register unsigned int ebx asm("ebx");
    register unsigned int ecx asm("ecx");
    register unsigned int edx asm("edx"); 
	ecx = (unsigned int) copyOnPrompt;   
    ebx = (unsigned int) name;
    eax = 0x01;
    asm("int 0x80");
    struct WINDOW *win = (void *) eax;
	return win;
}

void window_close(struct WINDOW *window){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	eax = 0x02;
	ebx = (uint32_t) window;
	asm("int 0x80");
}

void window_update(){
	register uint32_t eax asm("eax");
	eax = 0x03;
	asm("int 0x80");
}
