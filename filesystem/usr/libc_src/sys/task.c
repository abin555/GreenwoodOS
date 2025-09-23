#include <sys/task.h>
#include <stdint.h>
#include <sys/syscall.h>

int exec(char *filename, int argc, char **argv){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	register uint32_t edx asm("edx");
	edx = (uint32_t) argv;
	ecx = (uint32_t) argc;
	ebx = (uint32_t) filename;
	eax = SYSCALL_EXEC;
	asm("int 0x80");
	return (int) ebx;
}

void waitpid(int pid){
	register uint32_t eax asm("eax");
	register int ebx asm("ebx");
	ebx = pid;
	eax = SYSCALL_WAIT;
	asm("int 0x80");
	yield();
}

void set_schedule(ScheduleType type){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = type;
	eax = SYSCALL_SET_SCHEDULE;
	asm("int 0x80");
}

void yield(){
	register uint32_t eax asm("eax");
	eax = SYSCALL_YIELD;
	asm("int 0x80");
}

void task_lock(int state){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) state;
	eax = SYSCALL_TASK_LOCK;
	asm("int 0x80");
}

int manual_task(void *address, char *name){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	register uint32_t ecx asm("ecx");
	ecx = (uint32_t) name;
	ebx = (uint32_t) address;
	eax = SYSCALL_START_TASK;
	asm("int 0x80");
	return (int) ebx;
}