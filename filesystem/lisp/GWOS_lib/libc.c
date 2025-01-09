#include "libc.h"

int strcmp(const char *s1, const char *s2){
	while(*s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char*)s2;
}

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

void print_float(char *str, float arg){
	print_arg(str, *((uint32_t *) &arg));
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

struct FILE *fopen(char *file){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) file;
	eax = 0x0D;
	asm("int 0x80");
	struct FILE *f = (struct FILE *) eax;
	return f;
}

void fclose(struct FILE *file){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) file;
	eax = 0x0E;
	asm("int 0x80");
}

char fgetc(struct FILE *file){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) file;
	eax = 0x0F;
	asm("int 0x80");
	return (char) eax;
}

int fsize(struct FILE *file){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) file;
	eax = 0x10;
	asm("int 0x80");
	return (int) eax;
}

int fcopy(struct FILE *file, char *buf, int buf_size){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = buf_size;
	ecx = (uint32_t) buf;
	ebx = (uint32_t) file;
	eax = 0x11;
	asm("int 0x80");
}

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

int fexists(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) path;
	eax = 0x15;
	asm("int 0x80");
	return eax;
}

int fextend(struct FILE *file, uint32_t amount){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = amount;
	ebx = (uint32_t) file;
	eax = 0x22;
	asm("int 0x80");
	return eax;
}

struct FEATURE_INFO getKernelFeature(KERNEL_FEATURE feature){
	void *addr;
	uint32_t size;
	struct FEATURE_INFO info;
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) feature;
	eax = 0x17;
	asm("int 0x80");
	addr = (void *) eax;
	size = ebx;
	info.addr = addr;
	info.size = size;
	return info;
}

void fseek(struct FILE *file, int idx){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) idx;
	ebx = (uint32_t) file;
	eax = 0x18;
	asm("int 0x80");
}

void fputc(struct FILE *file, char c){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) c;
	ebx = (uint32_t) file;
	eax = 0x19;
	asm("int 0x80");	
}

void fmkdir(char *path){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) path;
	eax = 0x1A;
	asm("int 0x80");
}

void fmkfile(char *path, int size){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) size;
	ebx = (uint32_t) path;
	eax = (uint32_t) 0x1B;
	asm("int 0x80");
}

int getArrowKey(){
	register uint32_t eax asm("eax");
	eax = 0x1C;
	asm("int 0x80");
	return eax;
}

void srand(uint32_t seed){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = seed;
	eax = 0x1D;
	asm("int 0x80");
}

int rand(){
	register uint32_t eax asm("eax");
	eax = 0x1E;
	asm("int 0x80");
	return eax;
}

void exit(int code){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) code;
	eax = 0x1F;
	asm("int 0x80");
	return;
}

void *requestRegion(unsigned int bytes){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = bytes;
	eax = 0x20;
	asm("int 0x80");
	return (void *) eax;
}

void addKbdEventHandler(void (*handler)(unsigned char)){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) handler;
	eax = 0x21;
	asm("int 0x80");
}

void *requestRegionAt(unsigned int bytes, unsigned int addr){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = addr;
	ebx = bytes;
	eax = 0x23;
	asm("int 0x80");
	return (void *) eax;
}

void attachTimerCallback(unsigned int timer, void *callback){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) callback;
	ebx = (uint32_t) timer;
	eax = 0x24;
	asm("int 0x80");
}

void dprint(char *msg){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) msg;
	eax = 0x25;
	asm("int 0x80");
}

void start_manual_task(void *addr, char *name){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) name;
	ebx = (uint32_t) addr;
	eax = 0x26;
	asm("int 0x80");
}

struct MouseStatus getMouse(){
	struct MouseStatus *mousePtr;
	struct MouseStatus mouse;
	register uint32_t eax asm("eax");
	eax = 0x27;
	asm("int 0x80");
	mousePtr = (struct MouseStatus *) eax;
	mouse = *mousePtr;
	return mouse;
}

struct PCSpeaker_Handle *getPCSpeaker(){
	struct PCSpeaker_Handle *handle;
	register uint32_t eax asm("eax");
	eax = 0x28;
	asm("int 0x80");
	handle = (struct PCSpeaker_Handle *) eax;
	return handle;
}

uint32_t *getTimerTickHandle(){
	uint32_t *tick_handle;
	register uint32_t eax asm("eax");
	eax = 0x29;
	asm("int 0x80");
	tick_handle = (uint32_t *) eax;
	return tick_handle;
}

void addMouseEventHandler(void (*handler)(void)){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) handler;
	eax = 0x2A;
	asm("int 0x80");
}

void freeRegion(void *addr, uint32_t size){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = size;
	ebx = (uint32_t) addr;
	eax = 0x2B;
	asm("int 0x80");
}

void addEndCallback(void (*handler)(void)){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) handler;
	eax = 0x2C;
	asm("int 0x80");
}

struct ViewportFunctions *viewport_get_funcs(){
	register uint32_t eax asm("eax");
	eax = 0x2D;
	asm("int 0x80");
	return (struct ViewportFunctions *) eax;
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

struct RealTimeClock *get_rtc(){
	register uint32_t eax asm("eax");
	eax = 0x2F;
	asm("int 0x80");
	return (struct RealTimeClock *) eax;
}

struct DirectoryListing getDirectoryListing(char *path){
	struct DirectoryListing (*listFunc)(char *);
	register uint32_t eax asm("eax");
	eax = 0x30;
	asm("int 0x80");
	listFunc = (struct DirectoryListing (*)(char *)) eax;
	return listFunc(path);
}

void print_serial(char *str){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) str;
	eax = 0x31;
	asm("int 0x80");
}