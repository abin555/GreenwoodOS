#include "multitasking.h"
#include "vfs.h"

bool task_lock;
int8_t task_running_idx;
void *task_stack_base;
uint8_t *task_stack_array[MAX_TASKS] __attribute__((aligned (16)));
char task_fxsave_region[MAX_TASKS][512] __attribute__((aligned(16)));
struct task_state tasks[MAX_TASKS];

extern uint32_t kernel_stack_base;
int pid_counter;

#define TASK_STACK_VIRTUAL_BASE 0x40000000

void tasking_setup_kernel_stack(){
    kernel_stack_base = get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE), PAGE_SIZE, 0, STACK));
    print_serial("[KERNEL] Setup Kernel Stack at 0x%x\n", kernel_stack_base);
    MEM_printRegions();
}

void multitask_init(){
	print_serial("[TASK] Init Multitasking\n");
    task_stack_base = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE * MAX_TASKS), PAGE_SIZE * MAX_TASKS, 0, STACK));
    //tasks = (struct task_state *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(sizeof(struct task_state) * MAX_TASKS), sizeof(struct task_state) * MAX_TASKS, 0, KERNEL));
    memset(tasks, 0, sizeof(struct task_state) * MAX_TASKS);
    pid_counter = 1;
    MEM_printRegions();
    print_serial("[TASK] Setting Stack! - 0x%x\n", (uint32_t *) task_stack_base);
    for(int i = 0; i < MAX_TASKS; i++){
        task_stack_array[i] = task_stack_base + (TASK_STACK_SIZE * i);
        cpu_fxsave(task_fxsave_region[i]);
    }
}

void multitask_start(){
    task_lock = 0;
	timer_attach(1, task_callback);
}

void print_tasks(){
    print_serial("[TASKS] Task Status:\n");
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active == 0) continue;
        print_serial("PID %d - EAX: 0x%x Slot: %d EIP: 0x%x Running: %d Active: %d [\"%s\"] %c\n",
            i,
            tasks[i].registers.eax,
            tasks[i].program_slot,
            tasks[i].registers.eip,
            tasks[i].slot_running,
            tasks[i].slot_active,
            tasks[i].task_name,
            i == task_running_idx ? '*' : ' '
        );
    }
}

void save_task_state(struct task_state *task, struct cpu_state *cpu __attribute__((unused)), struct stack_state *stack __attribute__((unused))){
    task->registers.eip = stack->eip;
    task->registers.eax = cpu->eax;
    task->registers.ebx = cpu->ebx;
    task->registers.ecx = cpu->ecx;
    task->registers.edx = cpu->edx;
    task->registers.ebp = cpu->ebp;
    task->registers.esi = cpu->esi;
    task->registers.edi = cpu->edi;
    task->registers.esp = cpu->esp;
}

int task_get_available(){
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active == 0){
            return i;
        }
    }
    return -1;
}

int start_task(void *address, int8_t program_slot, int argc, char **argv, char* name, struct task_file_ctx *file_ctx){
	int retpid = 0;
    int task_idx = task_get_available();
    if(task_idx == -1) return -1;
    struct task_state *task = &tasks[task_idx];

    task->registers.eax = 0;
    task->registers.ebx = 0;
    task->registers.ecx = 0;
    task->registers.edx = 0;
    task->registers.eip = (uint32_t) address;
    register uint32_t esi asm("esi");
    register uint32_t edi asm("edi");
    task->registers.edi = edi;
    task->registers.esi = esi;
    
    task->stack_region = (uint32_t) task_stack_array[task_idx];
    task->registers.ebp = (uint32_t) task_stack_array[task_idx]+TASK_STACK_SIZE-(3*4);
    //task->registers.ebp = (uint32_t) TASK_STACK_VIRTUAL_BASE+TASK_STACK_SIZE-(3*4);
    task->registers.esp = (uint32_t) task->registers.ebp-(8*4);

    uint32_t *return_instruction = (uint32_t *) task->registers.ebp;
    return_instruction[1] = task->registers.ebp;
    return_instruction[0] = task->registers.esp;
    return_instruction[-1] = (uint32_t) argv;
    return_instruction[-2] = (uint32_t) argc;
    return_instruction[-3] = (uint32_t) &task_end;


    task->program_slot = program_slot;
    task->schedule_type = ALWAYS;
    //task->task_name = name; //"Task\0";
    
    if(name == NULL){
        task->task_name = "UNKNOWN TASK NAME\0";
    }
    else{
        int len = strlen(name);
        char *namebuf = malloc(len+1);
        memset(namebuf, 0, len+1);
        memcpy(namebuf, name, len);
        task->task_name = namebuf;
    }
    
    
    task->window = tasks[task_running_idx].window;
    task->own_window = false;
    task->console = tasks[task_running_idx].console;
    task->own_console = false;
    memcpy(task->currentDirectory.path, tasks[task_running_idx].currentDirectory.path, sizeof(task->currentDirectory.path));
    //print_serial("[TASK] Added to queue idx %d\n", i);
    task->slot_active = 1;
    task->keyboard_event_handler = NULL;
    //task->mouse_event_handler = NULL;
    for(int j = 0; j < MT_maxDescriptors; j++){
        task->file_descs[j] = -1;
    }
    if(file_ctx == NULL){
        task->file_descs[0] = vfs_openRel(&task->currentDirectory, "/-/sys/kbd", VFS_FLAG_READ);
        task->file_descs[1] = vfs_openRel(&task->currentDirectory, "/-/dev/console", VFS_FLAG_WRITE);
    }
    else {
        for(int i = 0; i < file_ctx->num_fds; i++){
            print_serial("[TASK] Association #%d - Creator FD %d -> Child FD %d\n", i, file_ctx->fds[i].creator_fd, file_ctx->fds[i].new_fd);
            task->file_descs[file_ctx->fds[i].new_fd] = tasks[task_running_idx].file_descs[file_ctx->fds[i].creator_fd];
        }
    }
    //task_allocFD(&tasks[i], vfs_openRel(&task->currentDirectory, "/-/dev/console", VFS_FLAG_WRITE));
    task->pid = pid_counter++;
    retpid = task->pid;
    task->waitpid = 0;
    print_serial("[TASK] Added Task \"%s\" to queue at %d (ESP: 0x%x, EBP: 0x%x) PID #%d\n", task->task_name, task_idx, task->registers.esp, task->registers.ebp, task->pid);
    for(int j = 0; j < MT_maxDescriptors; j++){
        print_serial("[TASK] \t Proc FD %d -> Sys FD %d\n", j, task->file_descs[j]);
    }
    print_serial("[TASK] Args: \n");
    for(int i = 0; i < argc; i++){
        if(argv == NULL || argv[i] == NULL) continue;
        print_serial("\t%d - %s\n", i, argv[i]);
    }

    return retpid;
}

int fork_ret(){
    return 0;
}

int fork(){
    print_serial("[FORK] Syscall\n");
    int pid;
    task_lock = 1;
    pid = task_fork(&tasks[task_running_idx]);
    task_lock = 0;
    return pid;
}

int task_fork(struct task_state *task){
    if(task == NULL) return -1;
    if(task->slot_active == 0) return -1;
    print_serial("[FORK] Forking Process - Current PID %d\n", task_running_idx);

    struct task_state *new_task;
    int pid;
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active == 0){
            new_task = &tasks[i];
            pid = i;
            break;
        }
    }
    print_serial("[FORK] Child PID %d\n", pid);

    memcpy(
        new_task,
        task,
        sizeof(struct task_state)
    );
    memcpy(
        task_stack_array[pid],
        task_stack_array[task_running_idx],
        TASK_STACK_SIZE
    );

    new_task->registers.eip = (uint32_t) &fork_ret;
    new_task->slot_running = 0;
    new_task->registers.eax = pid;
    print_tasks();
    print_serial("[FORK] Complete\n");
    return 0;
}



void set_schedule(ScheduleType type){
    //print_serial("[TASK] Set Task %d schedule type to %d\n", task_running_idx, type);
    tasks[task_running_idx].schedule_type = type;
}

void set_schedule_task(int taskID, ScheduleType type){
    print_serial("[TASK] Set Task %d (\"%s\") schedule type to %d\n", taskID, tasks[taskID].task_name, type);
    tasks[taskID].schedule_type = type;
}

int taskID_fromPID(int pid){
    //print_serial("[TASK] Looking for PID #%d\n", pid);
    if(pid == 0) return -1;
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active && tasks[i].pid == pid){
            //print_serial("[TASK] PID #%d -> Task ID %d\n", pid, i);
            return i;
        }
    }
    return -1;
}

struct task_state *task_fromPID(int pid){
    return &tasks[taskID_fromPID(pid)];
}

struct task_state *task_getCurrent(){
    print_serial("[TASK] Getting current task (TIDX: %d) - 0x%x\n", task_running_idx, &tasks[task_running_idx]);
    return &tasks[task_running_idx];
}

void stop_task(int8_t task_idx){
    tasks[task_idx].slot_active = 0;
    if(tasks[task_idx].program_slot != -1){
        program_slot_status[tasks[task_idx].program_slot] = false;
        if(tasks[task_idx].window != NULL && tasks[task_idx].own_window == true){
            window_close(tasks[task_idx].window);
            print_serial("[TASK] %d %s Closed Window\n", task_idx, tasks[task_idx].task_name);
        }
        tasks[task_idx].window = NULL;
        tasks[task_idx].own_window = false;
        if(tasks[task_idx].console != NULL && tasks[task_idx].own_console == true){
            console_close(tasks[task_idx].console);
            print_serial("[TASK] %d %s Closed Console\n", task_idx, tasks[task_idx].task_name);
        }
        tasks[task_idx].console = NULL;
        tasks[task_idx].own_console = false;
    }
    print_serial("[TASK] Killed Task %d %s\n", task_idx, tasks[task_idx].task_name);
    //task_callback();
}

void task_end(){
    tasks[task_running_idx].slot_active = 0;
    if(tasks[task_running_idx].program_slot != -1){
        program_slot_status[tasks[task_running_idx].program_slot] = false;
        if(tasks[task_running_idx].window != NULL && tasks[task_running_idx].own_window == true){
            window_close(tasks[task_running_idx].window);
            print_serial("[TASK] %d %s Closed Window\n", task_running_idx, tasks[task_running_idx].task_name);
        }
        tasks[task_running_idx].window = NULL;
        tasks[task_running_idx].own_window = false;
        if(tasks[task_running_idx].console != NULL && tasks[task_running_idx].own_console == true){
            console_close(tasks[task_running_idx].console);
            print_serial("[TASK] %d %s Closed Console\n", task_running_idx, tasks[task_running_idx].task_name);
        }
        tasks[task_running_idx].console = NULL;
        tasks[task_running_idx].own_console = false;
    }
    print_serial("[TASK] Task %d:%s Ended\n", task_running_idx, tasks[task_running_idx].task_name);
	/*
    if(tasks[task_running_idx].program_slot == -1)
        free((void *) tasks[task_running_idx].stack_region);
    else 
        freeProgramSlot(tasks[task_running_idx].program_slot);
    */

    for(int i = 0; i < MT_maxDescriptors; i++){
        if(tasks[task_running_idx].file_descs[i] != -1){
            vfs_close(tasks[task_running_idx].file_descs[i]);
        }
    }
    
	while(1){}
}

void task_kill(int8_t task_idx){
    tasks[task_idx].slot_active = 0;
}

void select_stack(int next, int current){
    print_serial("[TASK] (%d %d) Routing 0x%x to 0x%x and 0x%x to 0x%x\n",
        next, current,
        (uint32_t) task_stack_array[current],
        (uint32_t) task_stack_array[current],
        (uint32_t) task_stack_array[next],
        TASK_STACK_VIRTUAL_BASE
    );
    create_page_entry((uint32_t) (task_stack_base + TASK_STACK_SIZE*current), (uint32_t) (task_stack_base + TASK_STACK_SIZE*current), 0x83);
    create_page_entry((uint32_t) (task_stack_base + TASK_STACK_SIZE*next), TASK_STACK_VIRTUAL_BASE, 0x83);
    //MEM_printRegions();
}

void __attribute__ ((optimize("-O3"))) switch_to_task(struct task_state* old_task, struct task_state* new_task, int old_id __attribute__((unused)), int new_id __attribute__((unused))){
    //Save current task's state
    old_task->registers.eip = most_recent_int_stack_state.eip;
    old_task->registers.eax = most_recent_int_cpu_state.eax;
    old_task->registers.ebx = most_recent_int_cpu_state.ebx;
    old_task->registers.ecx = most_recent_int_cpu_state.ecx;
    old_task->registers.edx = most_recent_int_cpu_state.edx;
    old_task->registers.ebp = most_recent_int_cpu_state.ebp;
    old_task->registers.esi = most_recent_int_cpu_state.esi;
    old_task->registers.edi = most_recent_int_cpu_state.edi;
    old_task->registers.esp = most_recent_int_cpu_state.esp;
    cpu_fxsave(task_fxsave_region[old_id]);
    //asm volatile("FINIT");

    //Load next task's state
    //select_stack(new_id, old_id);
    if(new_task->program_slot != -1){
        select_program(new_task->program_slot);
    }
    cpu_fxrstor(task_fxsave_region[new_id]);
    override_state_return = true;
    most_recent_int_stack_state.eip = new_task->registers.eip;
    most_recent_int_cpu_state.eax = new_task->registers.eax;
    most_recent_int_cpu_state.ebx = new_task->registers.ebx;
    most_recent_int_cpu_state.ecx = new_task->registers.ecx;
    most_recent_int_cpu_state.edx = new_task->registers.edx;
    most_recent_int_cpu_state.ebp = new_task->registers.ebp;
    most_recent_int_cpu_state.esi = new_task->registers.esi;
    most_recent_int_cpu_state.edi = new_task->registers.edi;
    most_recent_int_cpu_state.esp = new_task->registers.esp;
}


void __attribute__ ((optimize("-O3"))) task_callback(){
    if(task_lock) return;
    int8_t running_idx=-1;
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_running){
            running_idx = i;
            break;
        }
    }
    bool task_available = false;
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active && i != running_idx){
            task_available = true;
            //print_serial("Task is available - %d\n", i);
            break;
        }
    }
    if(!task_available){
        //print_serial("[TASK] Somehow, there are no tasks available...\n");
        return;
    }
    int8_t next_idx = running_idx + 1;
    for(int checks = 0; checks < MAX_TASKS+5; checks++){
        if(next_idx == MAX_TASKS){
            next_idx = 0;
            continue;
        }
        if(tasks[next_idx].slot_active == 0 || tasks[next_idx].slot_running == 1){
            next_idx++;
            continue;
        }
        if(tasks[next_idx].slot_active 
            && (tasks[next_idx].schedule_type == ALWAYS 
            || (tasks[next_idx].schedule_type == ONFOCUS && tasks[next_idx].window == &windows[window_selected]))
            && (tasks[next_idx].schedule_type != NEVER)
        ){            
            if(tasks[next_idx].waitpid > 0){
                int waittask = taskID_fromPID(tasks[next_idx].waitpid);
                if(waittask != -1){
                    next_idx++;
                    continue;
                }
                else{
                    tasks[next_idx].waitpid = 0;
                }
            }
            
            switch_to_task((struct task_state*) &tasks[running_idx], (struct task_state*) &tasks[next_idx], running_idx, next_idx);
            tasks[running_idx].slot_running = 0;
            tasks[next_idx].slot_running = 1;
            running_idx = next_idx;
            task_running_idx = running_idx;
            break;
        }
        else{
            next_idx++;
            continue;
        }
    }
}

void list_tasks(){
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active){
            print_serial("Task #%d Is Called %s (Slot %d)\n", i, tasks[i].task_name, tasks[i].program_slot);
        }
    }
}

void task_yield(){
    task_callback();
}

int task_getFD(struct task_state *task){
    for(int i = 0; i < MT_maxDescriptors; i++){
        if(task->file_descs[i] == -1){
            return i;
        }
    }
    return -1;
}

int task_allocFD(struct task_state *task, int sysfd){
    if(sysfd == -1) return -1;
    int fd = task_getFD(task);
    task->file_descs[fd] = sysfd;
    return fd;
}

int task_getSysFD(struct task_state *task, int fd){
    return task->file_descs[fd];
}

void task_freeFD(struct task_state *task, int fd){
    task->file_descs[fd] = -1;
}

int task_dupFD(struct task_state *task, int fd){
    int dup_fd = task_allocFD(
        task, 
        task->file_descs[fd]
    );
    return dup_fd;
}

int task_get_slot(int task_idx){
    int slot = tasks[task_idx].program_slot;
    print_serial("[TASK] Got slot %d\n", slot);
    return slot;
}

int task_getCurrentID(){
    return task_running_idx;
}

void os_yield(){
	register uint32_t eax asm("eax") __attribute__((unused));
	eax = 0x34;
	asm("int 0x80");
}