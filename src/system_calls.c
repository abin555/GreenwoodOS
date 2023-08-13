#include "system_calls.h"

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	interrupt_add_handle(0x80, syscall_callback);
}

struct cpu_state syscall_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	struct cpu_state cpu_state = cpu;
	struct task_state *task = &tasks[task_running_idx];
	switch(cpu.eax){
		//Open Window
		case 0x01:{
			struct WINDOW *window = window_open((char *) cpu.ebx, true);
			cpu_state.eax = (uint32_t) window;
			task->window = window;
			print_serial("[SYS] Open Window %s\n", (char*) cpu.ebx);
			break;
		}
		//Close Window
		case 0x02:{
			print_serial("[SYS] Close Window %s\n", ((struct WINDOW *) cpu.ebx)->name);
			window_close((struct WINDOW *) cpu_state.ebx);
			break;
		}
		//Copy Buffer
		case 0x03:{
			window_copy_buffer(task->window);
			break;
		}
		//Put Char
		case 0x04:{
			print_serial("[SYS] Put Character X: %x Y: %x %c\n", cpu.ebx, cpu.ecx, cpu.edx);
			buf_putChar(task->window->backbuffer, cpu.ebx, cpu.eax, (char) cpu.edx, 0xFFFFFFFF, 0);
			break;
		}
	}
	return cpu_state;
}