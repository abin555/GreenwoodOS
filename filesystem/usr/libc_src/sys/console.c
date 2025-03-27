#include <sys/console.h>
#include <internal/stdio.h>
#include <stdio.h>


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

struct CONSOLE *console_open_vp(struct Viewport *vp){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) vp;
	eax = 0x2E;
	asm("int 0x80");
	struct CONSOLE *console = (struct CONSOLE *) eax;
	return console;
}

void console_print(struct CONSOLE *console, char *str){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) str;
    ebx = (uint32_t) console;
	eax = 0x3D;
	asm("int 0x80");
}

struct CONSOLE *console_get_current(){
    register uint32_t eax asm("eax");
	eax = 0x3E;
	asm("int 0x80");
    struct CONSOLE *console = (struct CONSOLE *) eax;
    return console;
}