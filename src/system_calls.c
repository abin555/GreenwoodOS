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
			print_serial("Name address is: 0x%x\n", cpu.ebx);
			cpu_state.eax = (uint32_t) window;
			task->window = window;
			task->own_window = true;
			print_serial("[SYS] Open Window %s\n", window->name);
			break;
		}
		//Close Window
		case 0x02:{
			print_serial("[SYS] Close Window %s\n", ((struct WINDOW *) cpu.ebx)->name);
			window_close((struct WINDOW *) cpu_state.ebx);
			task->window = NULL;
			task->own_console = false;
			break;
		}
		//Copy Buffer
		case 0x03:{
			window_copy_buffer(task->window);
			break;
		}
		//Put Char
		case 0x04:{
			//print_serial("[SYS] Put Character X: %x Y: %x %c %x\n", cpu.ebx, cpu.ecx, cpu.edx, cpu.edx);
			buf_putChar(task->window->backbuffer, cpu.ebx, cpu.ecx, (char) cpu.edx, 0xFFFFFFFF, 0);
			break;
		}
		//Get Kbd Char
		case 0x05:{
			cpu_state.eax = (unsigned int) getc_blk;
			break;
		}
		//Execute Program
		case 0x06:{
			print_console(task->console, "Executing %s %d: ", (char *) cpu.ebx, cpu.ecx);
			for(uint32_t i = 0; i < cpu.ecx; i++){
				print_console(task->console, " %s", ((char **) cpu.edx)[i]);
			}
			exec((char *)cpu.ebx, cpu.ecx, (char **) cpu.edx);
			break;
		}
		//Set Scheduling Type
		case 0x07:{
			set_schedule(cpu.ebx);
			break;
		}
		//Print to console
		case 0x08:{
			if(task->console != NULL)
				print_console(task->console, (char*) cpu.ebx);
			break;
		}
		//Open Console
		case 0x09:{
			if(task->window == NULL) break;
			struct CONSOLE *console = console_open(task->window);
			task->console = console;
			task->own_console = true;
			break;
		}
		//Close Console
		case 0x0A:{
			if(task->console == NULL) break;
			console_close(task->console);
			task->console = NULL;
			task->own_console = false;
			break;
		}
		//Malloc
		case 0x0B:{
			cpu_state.eax = (uint32_t) malloc(cpu.ebx);
			break;
		}
	}
	return cpu_state;
}