#include "system_calls.h"

syscall_fn syscall_functions[0xFF];

void syscall_window_open(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	struct WINDOW *window = window_open((char *) cpu->ebx, cpu->ecx);
	print_serial("Name address is: 0x%x\n", cpu->ebx);
	cpu->eax = (uint32_t) window;
	task->window = window;
	task->own_window = true;
	print_serial("[SYS] Open Window %s\n", window->name);
	return;
}

void syscall_window_close(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYS] Close Window %s\n", ((struct WINDOW *) cpu->ebx)->name);
	window_close((struct WINDOW *) cpu->ebx);
	task->window = NULL;
	task->own_console = false;
	return;
}

void syscall_window_copy(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	window_copy_buffer(task->window);
	return;
}

void syscall_buf_putChar(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	buf_putChar(task->window->backbuffer, cpu->ebx, cpu->ecx, (char) cpu->edx, 0xFFFFFFFF, 0);
	return;
}

void syscall_getc_blk(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) getc_blk;
	return;
}

void syscall_exec(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->ebx = exec((char *) cpu->ebx, cpu->ecx, (char **) cpu->edx, NULL);
	return;
}

void syscall_set_schedule(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	set_schedule(cpu->ebx);
	return;
}

void syscall_print_to_console(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	if(task->console != NULL)
		print_console(task->console, (char*) cpu->ebx);
}

void syscall_console_open(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	if(task->window == NULL) return;
	struct CONSOLE *console = console_open(task->window);
	cpu->eax = (uint32_t) console;
	task->console = console;
	task->own_console = true;
	return;
}

void syscall_console_close(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	if(task->console == NULL) return;
	console_close(task->console);
	task->console = NULL;
	task->own_console = false;
	return;
}

void syscall_kmalloc(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (uint32_t) malloc(cpu->ebx);
}

void syscall_console_printargs(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	if(task->console != NULL)
		print_console(task->console, (char*) cpu->ebx, cpu->ecx);
	return;
}

void syscall_chdir(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	char *change = (char *) cpu->ebx;
	print_serial("Task %s Changing Directory to %s\n", task->task_name, change);
	cpu->eax = vfs_chdir(&task->currentDirectory, change);
}

void syscall_get_dir(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (uint32_t) task->currentDirectory.path;
}

void syscall_print_serial(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial((char *) cpu->ebx);
}

void syscall_get_kernel_feature(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	switch(cpu->ebx){
		//Font Memory
		case 0x01:{
			cpu->eax = (uint32_t) FONT;
			cpu->ebx = (uint32_t) sizeof(FONT);
			break;
		};
		//Keyboard
		case 0x02:{
			cpu->eax = (uint32_t) keyboard_KEYBuffer;
			cpu->ebx = (uint32_t) keyboard_buffer_size;
			break;
		};
		//Paging Memory
		case 0x03:{
			cpu->eax = (uint32_t) page_directory;
			cpu->ebx = 0;
			break;
		};
		//Task Table
		case 0x04:{
			cpu->eax = (uint32_t) &tasks;
			cpu->ebx = sizeof(tasks);
			break;
		};
		//Program Memory Base
		case 0x05:{
			cpu->eax = PROGRAM_VIRT_REGION_BASE;
			cpu->ebx = PROGAM_MAX_SIZE;
			break;
		}
		//Raw Framebuffer
		case 0x06:{
			cpu->eax = (uint32_t) fb_frontbuffer;
			cpu->ebx = fb_width * fb_height * sizeof(uint32_t);
			break;
		}
		//Timer Function Buffer
		case 0x07:{
			cpu->eax = (uint32_t) &timer_attached_functions;
			cpu->ebx = timer_attached_functions_num;
			break;
		}
		//Keyboard Pressed Buffer
		case 0x08:{
			cpu->eax = (uint32_t) &key_pressed_map;
			cpu->ebx = sizeof(key_pressed_map);
			break;
		}
	}
}

void syscall_creatdir(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_creatDirRel(&task->currentDirectory, (char *) cpu->ebx);
}

void syscall_getArrow(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = getArrow();
}

void syscall_srand(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	srand(cpu->ebx);
}

void syscall_rand(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = rand();
}

void syscall_exit(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("EXIT Syscall\n");
	task_lock = 0;
	stop_task(task_running_idx);
	task_yield();
}

void syscall_mem_request(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	int block = MEM_findRegionIdx(cpu->ebx);
	uint32_t addr = MEM_reserveRegionBlock(block, cpu->ebx, 0, PROGRAM);
	cpu->eax = addr;
}

void syscall_keyboard_add_event(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Program requested keyboard event handler @%x\n", cpu->ebx);
	tasks[task_running_idx].keyboard_event_handler = (void (*)(char)) cpu->ebx;
}

void syscall_mem_reserve(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	int block = MEM_findRegionIdx(cpu->ebx);
	MEM_reserveRegionBlock(block, cpu->ebx, cpu->ecx, PROGRAM);
	//MEM_printRegions();
	cpu->eax = cpu->ecx;
}

void syscall_timer_attach(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	timer_attach((int) cpu->ebx, (void *) cpu->ecx);
}

void syscall_start_task(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->ebx = start_task((void *) cpu->ebx, -1, 0, NULL, (char *) cpu->ecx, NULL);
}

void syscall_get_pcspeaker(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (uint32_t) &PCSpeaker_Handle;
	print_serial("[SYSCALL] Program requested handle to PCspeaker - returned 0x%x\n", cpu->eax);
}

void syscall_get_timerticks(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (uint32_t) &timer_ticks;
	print_serial("[SYSCALL] Program requested handle to timer ticks - returned 0x%x\n", cpu->eax);
}

void syscall_mouse_event(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Program requested mouse event handler @%x\n", cpu->ebx);
	//tasks[task_running_idx].mouse_event_handler = (void (*)(void)) cpu->ebx;
}

void syscall_mem_free(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Free Region\n");
	MEM_freeRegionBlock(cpu->ebx, cpu->ecx);
}

void syscall_end_event(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Program requested program end event handler @%x\n", cpu->ebx);
	tasks[task_running_idx].end_callback = (void (*)(void)) cpu->ebx;
}

void syscall_console_open_vp(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	struct CONSOLE *console = console_open_vp((struct Viewport *) cpu->ebx);
	cpu->eax = (uint32_t) console;
	task->console = console;
	task->own_console = true;
}

void syscall_get_vfsListDir(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (uint32_t) &vfs_taskListDirectory;
}

void syscall_task_lock(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	task_lock = cpu->ebx & 1;
}

void syscall_yield(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	task_yield();
}

void syscall_open(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) task_allocFD(task, vfs_openRel(&task->currentDirectory, (char *) cpu->ebx, (int) cpu->ecx));
	print_serial("[SYSCALL] Opened \"%s\" into process FD %d, SYS FD %d\n", (char *) cpu->ebx, (int) cpu->eax, tasks[task_running_idx].file_descs[cpu->eax]);
}

void syscall_close(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	vfs_close(task_getSysFD(task, (int) cpu->ebx));
	task_freeFD(task, (int) cpu->ebx);
}

void syscall_read(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_read(task_getSysFD(task, (int) cpu->ebx), (void *) cpu->ecx, (int) cpu->edx);
}

void syscall_write(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_write(task_getSysFD(task, (int) cpu->ebx), (void *) cpu->ecx, (int) cpu->edx);
}

void syscall_seek(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_seek(task_getSysFD(task, (int) cpu->ebx), cpu->ecx, (int) cpu->edx);
}

void syscall_creat(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_creatRel(&task->currentDirectory, (char *) cpu->ebx);
}

void syscall_dupfd(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) task_dupFD(task, (int) cpu->ebx);
}

void syscall_fork(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	//print_serial("EIP: 0x%x\n", stack.eip);
	//task->registers.eip = stack.eip;
	print_serial("[SYSCALL] Fork\n");
	cpu->eax = 0;//(unsigned int) fork();
}

void syscall_truncate(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_ftruncate(task_getSysFD(task, cpu->ebx), cpu->ecx);
}

void syscall_fstat(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	cpu->eax = (unsigned int) vfs_stat(task_getSysFD(task, cpu->ebx), (void *) cpu->ecx);
}

void syscall_wait(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	int id = task_getCurrentID();
	print_serial("[SYSCALL] Program %d requests to wait until PID %d ends\n", tasks[id].pid, cpu->ebx);
	tasks[id].waitpid = cpu->ebx;
}

void syscall_pipe(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	int sys_fildesc[2];
	int (*fildesc)[2];
	fildesc = (int (*)[2]) cpu->ebx;
	vfs_mkpipe(&sys_fildesc[0], &sys_fildesc[1]);
	if(sys_fildesc[0] == -1 || sys_fildesc[1] == -1){
		cpu->eax = -1;
		return;
	}
	(*fildesc)[0] = task_allocFD(task, sys_fildesc[0]);
	(*fildesc)[1] = task_allocFD(task, sys_fildesc[1]);
	if((*fildesc)[0] == -1 || (*fildesc)[1] == -1){
		cpu->eax = -1;
		return;
	}
	cpu->eax = 0;
	return;	
}

void syscall_dup2(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	int oldfd = cpu->ebx;
	if(task->file_descs[oldfd] == -1){
		cpu->eax = -1;
		return;
	}
	int newfd = cpu->ecx;
	if(task->file_descs[newfd] != -1){
		vfs_close(task->file_descs[newfd]);
	}
	task->file_descs[newfd] = task->file_descs[oldfd];
	cpu->eax = newfd;
	return;
}

void syscall_exec_spec(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	struct exec_spec_ctx *ctx = (struct exec_spec_ctx *) cpu->ebx;
	if(ctx == NULL){
		cpu->eax = -1;
		return;
	}
	cpu->eax = exec_spec(ctx);
}

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	memset(syscall_functions, 0, sizeof(syscall_functions));
	syscall_set(0x01, syscall_window_open);
	syscall_set(0x02, syscall_window_close);
	syscall_set(0x03, syscall_window_copy);
	syscall_set(0x05, syscall_getc_blk);
	syscall_set(0x06, syscall_exec);
	syscall_set(0x07, syscall_set_schedule);
	syscall_set(0x08, syscall_print_to_console);
	syscall_set(0x09, syscall_console_open);
	syscall_set(0x0A, syscall_console_close);
	syscall_set(0x0B, syscall_kmalloc);
	syscall_set(0x0C, syscall_console_printargs);
	syscall_set(0x12, syscall_chdir);
	syscall_set(0x13, syscall_get_dir);
	syscall_set(0x17, syscall_get_kernel_feature);
	syscall_set(0x1A, syscall_creatdir);
	syscall_set(0x1D, syscall_srand);
	syscall_set(0x1E, syscall_rand);
	syscall_set(0x1F, syscall_exit);
	syscall_set(0x20, syscall_mem_request);
	syscall_set(0x21, syscall_keyboard_add_event);
	syscall_set(0x23, syscall_mem_reserve);
	syscall_set(0x26, syscall_start_task);
	syscall_set(0x28, syscall_get_pcspeaker);
	syscall_set(0x29, syscall_get_timerticks);
	syscall_set(0x2A, syscall_mouse_event);
	syscall_set(0x2B, syscall_mem_free);
	syscall_set(0x2C, syscall_end_event);
	syscall_set(0x2E, syscall_console_open_vp);
	syscall_set(0x30, syscall_get_vfsListDir);
	syscall_set(0x32, syscall_task_lock);
	syscall_set(0x34, syscall_yield);
	syscall_set(0x35, syscall_open);
	syscall_set(0x36, syscall_close);
	syscall_set(0x37, syscall_read);
	syscall_set(0x38, syscall_write);
	syscall_set(0x39, syscall_seek);
	syscall_set(0x3A, syscall_creat);
	syscall_set(0x3B, syscall_dupfd);
	syscall_set(0x3C, syscall_fork);
	syscall_set(0x3F, syscall_truncate);
	syscall_set(0x40, syscall_wait);
	syscall_set(0x41, syscall_pipe);
	syscall_set(0x42, syscall_dup2);
	syscall_set(0x43, syscall_exec_spec);
	syscall_set(0x44, syscall_fstat);

	interrupt_add_handle(0x80, syscall_callback);
}

void syscall_set(uint8_t call_id, syscall_fn fn){
	syscall_functions[call_id] = fn;
}

void syscall_callback(struct cpu_state *cpu __attribute__((unused)), struct stack_state *stack __attribute__((unused))){
	//IRQ_OFF;
	struct task_state *task = &tasks[task_running_idx];
	if(syscall_functions[cpu->eax] != NULL){
		syscall_functions[cpu->eax](cpu, task);
	}
	else{
		print_serial("[SYSCALL] Invalid call to syscall id %d from process pid %d (@0x%x)\n", cpu->eax, task->pid, stack->eip);
	}
	//IRQ_RES;
	return;	
}