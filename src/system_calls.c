#include "system_calls.h"

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	interrupt_add_handle(0x80, syscall_callback);
}

struct cpu_state syscall_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	IRQ_OFF;
	struct cpu_state cpu_state = cpu;
	struct task_state *task = &tasks[task_running_idx];
	//save_task_state(task, &cpu, &stack);
	//save_task_state(task, cpu, stack);
	switch(cpu.eax){
		//Open Window
		case 0x01:{
			struct WINDOW *window = window_open((char *) cpu.ebx, cpu.ecx);
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
			cpu_state.eax = fexists_rel(&task->currentDirectory, (char *) cpu_state.ebx);
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
				//Keyboard
				case 0x02:{
					cpu_state.eax = (uint32_t) keyboard_KEYBuffer;
					cpu_state.ebx = (uint32_t) keyboard_buffer_size;
					break;
				};
				//Paging Memory
				case 0x03:{
					cpu_state.eax = (uint32_t) page_directory;
					cpu_state.ebx = 0;
					break;
				};
				//Task Table
				case 0x04:{
					cpu_state.eax = (uint32_t) &tasks;
					cpu_state.ebx = sizeof(tasks);
					break;
				};
				//Program Memory Base
				case 0x05:{
					cpu_state.eax = PROGRAM_VIRT_REGION_BASE;
					cpu_state.ebx = PROGAM_MAX_SIZE;
					break;
				}
				//Raw Framebuffer
				case 0x06:{
					cpu_state.eax = (uint32_t) fb_frontbuffer;
					cpu_state.ebx = fb_width * fb_height * sizeof(uint32_t);
					break;
				}
				//Timer Function Buffer
				case 0x07:{
					cpu_state.eax = (uint32_t) &timer_attached_functions;
					cpu_state.ebx = timer_attached_functions_num;
					break;
				}
				//Keyboard Pressed Buffer
				case 0x08:{
					cpu_state.eax = (uint32_t) &key_pressed_map;
					cpu_state.ebx = sizeof(key_pressed_map);
					break;
				}
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
			fmkdir(&task->currentDirectory, (char *) cpu.ebx);
			break;
		}
		case 0x1B:{
			fmkfile(&task->currentDirectory, (char *) cpu.ebx, (uint32_t) cpu.ecx);
			break;
		}
		case 0x1C:{
			cpu_state.eax = getArrow();
			break;
		}
		case 0x1D:{
			srand(cpu.ebx);
			break;
		}
		case 0x1E:{
			cpu_state.eax = rand();
			break;
		}
		case 0x1F:{
			print_serial("EXIT Syscall\n");
			task_lock = 0;
			stop_task(task_running_idx);
			break;
		}
		//Request Memory Block, takes number of bytes, returns void pointer to region
		case 0x20:{
			int block = MEM_findRegionIdx(cpu.ebx);
			uint32_t addr = MEM_reserveRegionBlock(block, cpu.ebx, 0, PROGRAM);
			cpu_state.eax = addr;
			//MEM_printRegions();
			break;
		}
		//Add Keyboard Event Listener
		case 0x21:{
			print_serial("[SYSCALL] Program requested keyboard event handler @%x\n", cpu.ebx);
			tasks[task_running_idx].keyboard_event_handler = (void (*)(char)) cpu.ebx;
			break;
		}
		//Extend Files
		case 0x22:{
			cpu_state.eax = fextend((struct FILE *) cpu.ebx, (uint32_t) cpu.ecx);
			break;
		}
		//Request Memory Block of size to be alloced to requested virtual memory region
		case 0x23:{
			int block = MEM_findRegionIdx(cpu.ebx);
			MEM_reserveRegionBlock(block, cpu.ebx, cpu.ecx, PROGRAM);
			//MEM_printRegions();
			cpu_state.eax = cpu.ecx;
			break;
		}
		//Add timer callback
		case 0x24:{
			timer_attach((int) cpu.ebx, (void *) cpu.ecx);
			break;
		}
		case 0x25:{
			print_serial((char *) cpu.ebx);
			break;
		}
		case 0x26:{
			start_task((void *) cpu.ebx, -1, 0, NULL, (char *) cpu.ecx);
			break;
		}
		case 0x27:{
			cpu_state.eax = (uint32_t) &mouseStatus;
			break;
		}
		case 0x28:{
			cpu_state.eax = (uint32_t) &PCSpeaker_Handle;
			print_serial("[SYSCALL] Program requested handle to PCspeaker - returned 0x%x\n", cpu_state.eax);
			break;
		}
		case 0x29:{
			cpu_state.eax = (uint32_t) &timer_ticks;
			print_serial("[SYSCALL] Program requested handle to timer ticks - returned 0x%x\n", cpu_state.eax);
			break;
		}
		case 0x2A:{
			print_serial("[SYSCALL] Program requested mouse event handler @%x\n", cpu.ebx);
			tasks[task_running_idx].mouse_event_handler = (void (*)(void)) cpu.ebx;
			break;
		}
		case 0x2B:{
			print_serial("[SYSCALL] Free Region\n");
			MEM_freeRegionBlock(cpu.ebx, cpu.ecx);
			//MEM_printRegions();
			break;
		}
		case 0x2C:{
			print_serial("[SYSCALL] Program requested program end event handler @%x\n", cpu.ebx);
			tasks[task_running_idx].end_callback = (void (*)(void)) cpu.ebx;
			break;
		}
		case 0x2D:{
			cpu_state.eax = (uint32_t) &global_viewport_functions;
			break;
		}
		case 0x2E:{
			print_serial("Syscall 0x2E\n");
			struct CONSOLE *console = console_open_vp((struct Viewport *) cpu.ebx);
			cpu_state.eax = (uint32_t) console;
			task->console = console;
			task->own_console = true;
			break;
		}
		case 0x2F:{
			cpu_state.eax = (uint32_t) &RTC;
			break;
		}
		case 0x30:{
			cpu_state.eax = (uint32_t) &task_listDirectory;
			break;
		}
		case 0x31:{
			print_serial((char *) cpu.ebx);
			break;
		}
		case 0x32:{
			task_lock = cpu.ebx & 1;
			break;
		}
		case 0x33:{
			write_serial((char) cpu.ebx);
			break;
		}
		case 0x34:{
			task_yield();
			break;
		}
		case 0x35:{
			cpu_state.eax = (unsigned int) task_allocFD(task, vfs_openRel(&task->currentDirectory, (char *) cpu.ebx, (int) cpu.ecx));
			break;
		}
		case 0x36:{
			vfs_close(task_getSysFD(task, (int) cpu.ebx));
			task_freeFD(task, (int) cpu.ebx);
			break;
		}
		case 0x37:{
			cpu_state.eax = (unsigned int) vfs_read(task_getSysFD(task, (int) cpu.ebx), (void *) cpu.ecx, (int) cpu.edx);
			break;
		}
		case 0x38:{
			cpu_state.eax = (unsigned int) vfs_write(task_getSysFD(task, (int) cpu.ebx), (void *) cpu.ecx, (int) cpu.edx);
			break;
		}
		case 0x39:{
			cpu_state.eax = (unsigned int) vfs_seek(task_getSysFD(task, (int) cpu.ebx), cpu.ecx, (int) cpu.edx);
			break;
		}
		case 0x3A:{
			cpu_state.eax = (unsigned int) vfs_creatRel(&task->currentDirectory, (char *) cpu.ebx);
			break;
		}
		case 0x3B:{
			cpu_state.eax = (unsigned int) task_dupFD(task, (int) cpu.ebx);
			break;
		}
		case 0x3C:{
			//print_serial("EIP: 0x%x\n", stack.eip);
			//task->registers.eip = stack.eip;
			print_serial("[SYSCALL] Fork\n");
			cpu_state.eax = 0;//(unsigned int) fork();
			break;
		}
		//Print to console
		case 0x3D:{
			print_console((struct CONSOLE *) cpu.ebx, (char*) cpu.ecx);
			break;
		}
		case 0x3E:{
			cpu_state.eax = (unsigned int) task->console;
			break;
		}
	}
	IRQ_RES;
	return cpu_state;
}