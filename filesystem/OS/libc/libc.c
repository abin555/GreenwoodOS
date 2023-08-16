#include "libc.h"

struct WINDOW *window_open(char *name){
	register unsigned int eax asm("eax");
    register unsigned int ebx asm("ebx");
    register unsigned int ecx asm("ecx");
    register unsigned int edx asm("edx");    
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

void drawChar(uint32_t x, uint32_t y, char c){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = c;
	ecx = y;
	ebx = x;
	eax = 0x04;
	asm("int 0x80");
}

char getc(){
	
	char (*getc_blk)(void);
	register uint32_t eax asm("eax");
	eax = 0x05;
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

void exec(char *filename, int argc, char **argv){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = (uint32_t) argv;
	ecx = (uint32_t) argc;
	ebx = (uint32_t) filename;
	eax = 0x06;
	asm("int 0x80");
}
void set_schedule(ScheduleType type){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = type;
	eax = 0x07;
	asm("int 0x80");
}

void print(char *str){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) str;
	eax = 0x08;
	asm("int 0x80");
}

void print_arg(char *str, uint32_t arg){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) arg;
	ebx = (uint32_t) str;
	eax = 0x0C;
	asm("int 0x80");
}

struct CONSOLE *console_open(){
	register uint32_t eax asm("eax");
	eax = 0x09;
	asm("int 0x80");
	struct CONSOLE *console = (struct CONSOLE *) eax;
	return console;
}

void console_close(){
	register uint32_t eax asm("eax");
	eax = 0x0A;
	asm("int 0x80");
}

void *kmalloc(uint32_t size){
	register uint32_t eax asm("eax");
	eax = 0x0B;
	asm("int 0x80");
	return (void *) eax;
}