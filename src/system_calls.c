#include "system_calls.h"

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	interrupt_add_handle(0x80, syscall_callback);
}

struct cpu_state syscall_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	IRQ_OFF;
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
			cpu_state.eax = (uint32_t) console;
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
		//Print to console w/t args=
		case 0x0C:{
			if(task->console != NULL)
				print_console(task->console, (char*) cpu.ebx, cpu.ecx);
			break;
		}
		//File Open
		case 0x0D:{
			struct FILE *file = fopen_rel(&task->currentDirectory, (char *) cpu.ebx);
			cpu_state.eax = (uint32_t) file;
			break;
		}
		//File Close
		case 0x0E:{
			fclose((struct FILE *) cpu.ebx);
			break;
		}
		//File GetC
		case 0x0F:{
			char c = fgetc((struct FILE *) cpu.ebx);
			cpu_state.eax = c;
			break;
		}
		//File Get Size
		case 0x10:{
			int size = fsize((struct FILE *) cpu.ebx);
			cpu_state.eax = size;
			break;
		}
		//File Copy
		case 0x11:{
			int status = fcopy((struct FILE *) cpu.ebx, (char *) cpu.ecx, cpu.edx);
			cpu_state.eax = status;
			break;
		}
		//Change Directory
		case 0x12:{
			char *change = (char *) cpu.ebx;
			print_serial("Task %s Changing Directory to %s\n", task->task_name, change);
			cpu_state.eax = changeDirectory(&task->currentDirectory, change);
			break;
		}
		//Get Directory
		case 0x13:{
			//return currentDirectory string
			cpu_state.eax = (uint32_t) task->currentDirectory.path;
			break;
		}
		//List Directory
		case 0x14:{
			//Return string of files and folders in current directory.
			listFiles(task->console, &task->currentDirectory, (char *) cpu.ebx);
			break;
		}
		//File Exists
		case 0x15:{
			cpu_state.eax = fexists((char *) cpu_state.ebx);
			break;
		}
		//Print Serial
		case 0x16:{
			print_serial((char *) cpu.ebx);
			break;
		}
		//Get Raw Kernel Feature
		//Arguments:
		// EBX = Feature
		//Returns:
		// EAX = Pointer
		// EBX = Size
		case 0x17:{
			switch(cpu.ebx){
				//Font Memory
				case 0x01:{
					cpu_state.eax = (uint32_t) FONT;
					cpu_state.ebx = (uint32_t) sizeof(FONT);
					break;
				};
			}
			break;
		}
		//File Seek
		case 0x18:{
			fseek((struct FILE *) cpu.ebx, cpu.ecx);
			break;
		}
		//File Put Character
		case 0x19:{
			fputc((struct FILE *) cpu.ebx, (char) cpu.ecx);
			break;
		}
		case 0x1A:{
			fmkdir((char *) cpu.ebx);
			break;
		}
	}
	IRQ_RES;
	return cpu_state;
}