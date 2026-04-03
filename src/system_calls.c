#include "system_calls.h"

Syscall syscall_functions[0xFF];
struct syscall_debug {
	int enabled;
	int pid;
} syscall_debug;

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
			cpu->eax = program_region_virt_base;
			cpu->ebx = PROGRAM_MAX_SIZE;
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
/*
void syscall_keyboard_add_event(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Program requested keyboard event handler @%x\n", cpu->ebx);
	tasks[task_running_idx].keyboard_event_handler = (void (*)(char)) cpu->ebx;
}
*/
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

/*
void syscall_end_event(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	print_serial("[SYSCALL] Program requested program end event handler @%x\n", cpu->ebx);
	tasks[task_running_idx].end_callback = (void (*)(void)) cpu->ebx;
}
*/

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

void syscall_pidAlive(struct cpu_state *cpu __attribute__((unused)), struct task_state *task __attribute__((unused))){
	if(taskID_fromPID((int) cpu->ebx) != -1) cpu->ebx = 1;
	else cpu->ebx = 0;
	//print_serial("[SYSCALL] PID %d is alive\n")
	return;
}

#define set(ID, FN) syscall_set(ID, FN, #FN)

void init_syscalls(){
	print_serial("[SYSCALL] Init\n");
	memset(syscall_functions, 0, sizeof(syscall_functions));
	set(0x01, syscall_window_open);
	set(0x02, syscall_window_close);
	set(0x03, syscall_window_copy);
	set(0x04, syscall_buf_putChar);
	set(0x05, syscall_getc_blk);
	set(0x06, syscall_exec);
	set(0x07, syscall_set_schedule);
	set(0x08, syscall_print_to_console);
	set(0x09, syscall_console_open);
	set(0x0A, syscall_console_close);
	set(0x0B, syscall_kmalloc);
	set(0x0C, syscall_console_printargs);
	set(0x12, syscall_chdir);
	set(0x13, syscall_get_dir);
	set(0x17, syscall_get_kernel_feature);
	set(0x1A, syscall_creatdir);
	set(0x1D, syscall_srand);
	set(0x1E, syscall_rand);
	set(0x1F, syscall_exit);
	set(0x20, syscall_mem_request);
	//set(0x21, syscall_keyboard_add_event);
	set(0x23, syscall_mem_reserve);
	set(0x26, syscall_start_task);
	set(0x28, syscall_get_pcspeaker);
	set(0x29, syscall_get_timerticks);
	set(0x2A, syscall_mouse_event);
	set(0x2B, syscall_mem_free);
	//set(0x2C, syscall_end_event);
	set(0x2E, syscall_console_open_vp);
	set(0x30, syscall_get_vfsListDir);
	set(0x32, syscall_task_lock);
	set(0x34, syscall_yield);
	set(0x35, syscall_open);
	set(0x36, syscall_close);
	set(0x37, syscall_read);
	set(0x38, syscall_write);
	set(0x39, syscall_seek);
	set(0x3A, syscall_creat);
	set(0x3B, syscall_dupfd);
	set(0x3C, syscall_fork);
	set(0x3F, syscall_truncate);
	set(0x40, syscall_wait);
	set(0x41, syscall_pipe);
	set(0x42, syscall_dup2);
	set(0x43, syscall_exec_spec);
	set(0x44, syscall_fstat);
	set(0x45, syscall_pidAlive);

	syscall_debug = (struct syscall_debug) {0, -1};

	interrupt_add_handle(0x80, syscall_callback);
}

void syscall_set(uint8_t call_id, syscall_fn fn, char *name){
	syscall_functions[call_id].fn = fn;
	syscall_functions[call_id].name = name;
}

void syscall_callback(struct cpu_state *cpu __attribute__((unused)), struct stack_state *stack __attribute__((unused))){
	//IRQ_OFF;
	struct task_state *task = &tasks[task_running_idx];
	if(syscall_functions[cpu->eax].fn != NULL){
		if(syscall_debug.enabled && task->pid == syscall_debug.pid){
			print_serial("[SYSCALL] 0x%x (%s) - from 0x%x EBX: 0x%x ECX: 0x%x EDX: 0x%x\n", cpu->eax, syscall_functions[cpu->eax].name, stack->eip, cpu->ebx, cpu->ecx, cpu->edx);
			print_stack_trace(cpu->ebp, 15);
		}
		syscall_functions[cpu->eax].fn(cpu, task);
	}
	else{
		print_serial("[SYSCALL] Invalid call to syscall id %d from process pid %d (@0x%x)\n", cpu->eax, task->pid, stack->eip);
	}
	//IRQ_RES;
	return;	
}

#include <sysfs.h>
#include <vfs.h>

struct SyscallHook syscall_hook_buf;
/*
int syscall_hooks_read(void *cdev, void *buf, int roffset, int nbytes, int *head){
	struct SysFS_Chardev *syscall_cdev = cdev;
	if(buf == NULL) return -1;
	if(nbytes != sizeof(syscall_hook_buf) || roffset != 0) return -1;
	syscall_hook_buf.fn = syscall_functions[syscall_hook_buf.call_id];
	memcpy(buf, ((void *) &syscall_cdev->buf) + roffset, nbytes);
	*head = 0;
	return nbytes;
}

int syscall_hooks_write(void *cdev, void *buf, int woffset, int nbytes, int *head){
	struct SysFS_Chardev *syscall_cdev = cdev;
	if(buf == NULL) return -1;
	//if((nbytes + woffset) > sizeof(syscall_hook_buf)) nbytes = (sizeof(syscall_hook_buf) - woffset);
	if(nbytes != sizeof(syscall_hook_buf) || woffset != 0) return -1;
	memcpy(((void *) &syscall_cdev->buf) + woffset, buf, nbytes);
	syscall_functions[syscall_hook_buf.call_id] = syscall_hook_buf.fn;
	*head = 0;
	return nbytes;
}

void *syscall_init_hooks(){
	syscall_hook_buf.call_id = 0;
	syscall_hook_buf.fn = syscall_functions[syscall_hook_buf.call_id];
	struct SysFS_Chardev *syscall_cdev = sysfs_createCharDevice(
		(char *) &syscall_hook_buf,
		sizeof(syscall_hook_buf),
		CDEV_READ | CDEV_WRITE
	);
	sysfs_setCallbacks(syscall_cdev,
		NULL,
		NULL,
		syscall_hooks_write,
		syscall_hooks_read
	);

	return sysfs_mkcdev("syscall", syscall_cdev);
}
*/


void syscall_dbg_write(void *cdev, int offset, int nbytes, int *head){
	(void) offset;
	(void) nbytes;
	struct SysFS_Chardev *syscall_cdev = cdev;
	struct task_state *task = &tasks[task_running_idx];
	if(syscall_cdev->buf[0]){
		syscall_debug.enabled = 1;
		syscall_debug.pid = task->pid;
	}
	else{
		syscall_debug.enabled = 0;
	}
	*head = 0;
}

void *syscall_init_debug(){
	char *buf = malloc(2);
	struct SysFS_Chardev *syscall_dbg_cdev = sysfs_createCharDevice(
		buf,
		2,
		CDEV_WRITE
	);
	sysfs_setCallbacks(syscall_dbg_cdev,
		syscall_dbg_write,
		NULL,
		NULL,
		NULL
	);
	return sysfs_mkcdev("syscall_dbg", syscall_dbg_cdev);
}