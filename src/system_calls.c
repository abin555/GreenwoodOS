#include "system_calls.h"
#include "tasking.h"
#include "task.h"

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	interrupt_add_handle(0x80, syscall_callback);
}

void syscall_callback(void *t){
	IRQ_OFF;
	struct gwos_task *task = t;
	//save_task_state(task, &cpu, &stack);
	//save_task_state(task, cpu, stack);
	//print_serial("[SYSCALL] #%x\n", task->state->eax);
	switch(task->state->eax){
		//Open Window
		case 0x01:{
			struct WINDOW *window = window_open((char *) task->state->ebx, task->state->ecx);
			print_serial("Name address is: 0x%x\n", task->state->ebx);
			task->state->eax = (uint32_t) window;
			task->window = window;
			task->own_window = true;
			print_serial("[SYS] Open Window %s\n", window->name);
			break;
		}
		//Close Window
		case 0x02:{
			print_serial("[SYS] Close Window %s\n", ((struct WINDOW *) task->state->ebx)->name);
			window_close((struct WINDOW *) task->state->ebx);
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
			//print_serial("[SYS] Put Character X: %x Y: %x %c %x\n", task->state->ebx, task->state->ecx, task->state->edx, task->state->edx);
			buf_putChar(task->window->backbuffer, task->state->ebx, task->state->ecx, (char) task->state->edx, 0xFFFFFFFF, 0);
			break;
		}
		//Get Kbd Char
		case 0x05:{
			task->state->eax = (unsigned int) getc_blk;
			break;
		}
		//Execute Program
		case 0x06:{
			exec((char *)task->state->ebx, task->state->ecx, (char **) task->state->edx);
			break;
		}
		//Set Scheduling Type
		case 0x07:{
			set_schedule(task->state->ebx);
			break;
		}
		//Print to console
		case 0x08:{
			if(task->console != NULL)
				print_console(task->console, (char*) task->state->ebx);
			break;
		}
		//Open Console
		case 0x09:{
			if(task->window == NULL) break;
			struct CONSOLE *console = console_open(task->window);
			task->state->eax = (uint32_t) console;
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
			task->state->eax = (uint32_t) malloc(task->state->ebx);
			break;
		}
		//Print to console w/t args=
		case 0x0C:{
			if(task->console != NULL)
				print_console(task->console, (char*) task->state->ebx, task->state->ecx);
			break;
		}
		//File Open
		case 0x0D:{
			break;
		}
		//File Close
		case 0x0E:{
			break;
		}
		//File GetC
		case 0x0F:{
			break;
		}
		//File Get Size
		case 0x10:{
			break;
		}
		//File Copy
		case 0x11:{
			break;
		}
		//Change Directory
		case 0x12:{
			char *change = (char *) task->state->ebx;
			print_serial("Task %s Changing Directory to %s\n", task->name, change);
			task->state->eax = vfs_chdir(&task->currentDirectory, change);
			break;
		}
		//Get Directory
		case 0x13:{
			//return currentDirectory string
			task->state->eax = (uint32_t) task->currentDirectory.path;
			break;
		}
		//List Directory
		case 0x14:{
			//Return string of files and folders in current directory.
			break;
		}
		//File Exists
		case 0x15:{
			break;
		}
		//Print Serial
		case 0x16:{
			print_serial((char *) task->state->ebx);
			break;
		}
		//Get Raw Kernel Feature
		//Arguments:
		// EBX = Feature
		//Returns:
		// EAX = Pointer
		// EBX = Size
		case 0x17:{
			switch(task->state->ebx){
				//Font Memory
				case 0x01:{
					task->state->eax = (uint32_t) FONT;
					task->state->ebx = (uint32_t) sizeof(FONT);
					break;
				};
				//Keyboard
				case 0x02:{
					task->state->eax = (uint32_t) keyboard_KEYBuffer;
					task->state->ebx = (uint32_t) keyboard_buffer_size;
					break;
				};
				//Paging Memory
				case 0x03:{
					task->state->eax = (uint32_t) page_directory;
					task->state->ebx = 0;
					break;
				};
				//Task Table
				case 0x04:{
					task->state->eax = (uint32_t) &tasks;
					task->state->ebx = sizeof(tasks);
					break;
				};
				//Program Memory Base
				case 0x05:{
					task->state->eax = PROGRAM_VIRT_REGION_BASE;
					task->state->ebx = PROGAM_MAX_SIZE;
					break;
				}
				//Raw Framebuffer
				case 0x06:{
					task->state->eax = (uint32_t) fb_frontbuffer;
					task->state->ebx = fb_width * fb_height * sizeof(uint32_t);
					break;
				}
				//Timer Function Buffer
				case 0x07:{
					task->state->eax = (uint32_t) &timer_attached_functions;
					task->state->ebx = timer_attached_functions_num;
					break;
				}
				//Keyboard Pressed Buffer
				case 0x08:{
					task->state->eax = (uint32_t) &key_pressed_map;
					task->state->ebx = sizeof(key_pressed_map);
					break;
				}
			}
			break;
		}
		//File Seek
		case 0x18:{
			break;
		}
		//File Put Character
		case 0x19:{
			break;
		}
		case 0x1A:{
			break;
		}
		case 0x1B:{
			break;
		}
		case 0x1C:{
			task->state->eax = getArrow();
			break;
		}
		case 0x1D:{
			srand(task->state->ebx);
			break;
		}
		case 0x1E:{
			task->state->eax = rand();
			break;
		}
		case 0x1F:{
			print_serial("EXIT Syscall\n");
			task_lock = 0;
			stop_task(task_running_idx);
			task_yield();
			break;
		}
		//Request Memory Block, takes number of bytes, returns void pointer to region
		case 0x20:{
			int block = MEM_findRegionIdx(task->state->ebx);
			uint32_t addr = MEM_reserveRegionBlock(block, task->state->ebx, 0, PROGRAM);
			task->state->eax = addr;
			//MEM_printRegions();
			break;
		}
		//Add Keyboard Event Listener
		case 0x21:{
			print_serial("[SYSCALL] Program requested keyboard event handler @%x\n", task->state->ebx);
			tasks[task_running_idx].keyboard_event_handler = (void (*)(char)) task->state->ebx;
			break;
		}
		//Extend Files
		case 0x22:{
			break;
		}
		//Request Memory Block of size to be alloced to requested virtual memory region
		case 0x23:{
			int block = MEM_findRegionIdx(task->state->ebx);
			MEM_reserveRegionBlock(block, task->state->ebx, task->state->ecx, PROGRAM);
			//MEM_printRegions();
			task->state->eax = task->state->ecx;
			break;
		}
		//Add timer callback
		case 0x24:{
			timer_attach((int) task->state->ebx, (void *) task->state->ecx);
			break;
		}
		case 0x25:{
			break;
		}
		case 0x26:{
			start_task((void *) task->state->ebx, -1, 0, NULL, (char *) task->state->ecx);
			break;
		}
		case 0x27:{
			break;
		}
		case 0x28:{
			task->state->eax = (uint32_t) &PCSpeaker_Handle;
			print_serial("[SYSCALL] Program requested handle to PCspeaker - returned 0x%x\n", task->state->eax);
			break;
		}
		case 0x29:{
			task->state->eax = (uint32_t) &timer_ticks;
			print_serial("[SYSCALL] Program requested handle to timer ticks - returned 0x%x\n", task->state->eax);
			break;
		}
		case 0x2A:{
			print_serial("[SYSCALL] Program requested mouse event handler @%x\n", task->state->ebx);
			tasks[task_running_idx].mouse_event_handler = (void (*)(void)) task->state->ebx;
			break;
		}
		case 0x2B:{
			print_serial("[SYSCALL] Free Region\n");
			MEM_freeRegionBlock(task->state->ebx, task->state->ecx);
			//MEM_printRegions();
			break;
		}
		case 0x2C:{
			print_serial("[SYSCALL] Program requested program end event handler @%x\n", task->state->ebx);
			tasks[task_running_idx].end_callback = (void (*)(void)) task->state->ebx;
			break;
		}
		case 0x2E:{
			struct CONSOLE *console = console_open_vp((struct Viewport *) task->state->ebx);
			task->state->eax = (uint32_t) console;
			task->console = console;
			task->own_console = true;
			break;
		}
		case 0x30:{
			task->state->eax = (uint32_t) &vfs_taskListDirectory;
			break;
		}
		case 0x32:{
			task_lock = task->state->ebx & 1;
			//print_serial("[SYSCALL] Task Lock %d\n", task_lock);
			break;
		}
		case 0x33:{
			break;
		}
		case 0x34:{
			task_yield();
			break;
		}
		case 0x35:{
			//task->state->eax = (unsigned int) task_allocFD(task, vfs_openRel(&task->currentDirectory, (char *) task->state->ebx, (int) task->state->ecx));
			break;
		}
		case 0x36:{
			//vfs_close(task_getSysFD(task, (int) task->state->ebx));
			//task_freeFD(task, (int) task->state->ebx);
			break;
		}
		case 0x37:{
			//task->state->eax = (unsigned int) vfs_read(task_getSysFD(task, (int) task->state->ebx), (void *) task->state->ecx, (int) task->state->edx);
			break;
		}
		case 0x38:{
			//task->state->eax = (unsigned int) vfs_write(task_getSysFD(task, (int) task->state->ebx), (void *) task->state->ecx, (int) task->state->edx);
			break;
		}
		case 0x39:{
			//task->state->eax = (unsigned int) vfs_seek(task_getSysFD(task, (int) task->state->ebx), task->state->ecx, (int) task->state->edx);
			break;
		}
		case 0x3A:{
			//task->state->eax = (unsigned int) vfs_creatRel(&task->currentDirectory, (char *) task->state->ebx);
			break;
		}
		case 0x3B:{
			//task->state->eax = (unsigned int) task_dupFD(task, (int) task->state->ebx);
			break;
		}
		case 0x3C:{
			//print_serial("EIP: 0x%x\n", stack.eip);
			//task->registers.eip = stack.eip;
			print_serial("[SYSCALL] Fork\n");
			task->state->eax = 0;//(unsigned int) fork();
			break;
		}
		//Print to console
		case 0x3D:{
			break;
		}
		case 0x3E:{
			break;
		}
		case 0x3F:{
			//task->state->eax = (unsigned int) vfs_ftruncate(task->state->ebx, task->state->ecx);
			break;
		}
	}
	IRQ_RES;
	return;
}