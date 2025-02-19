#include "multitasking.h"

bool task_lock;
int8_t task_running_idx;
void *task_stack_base;
uint8_t *task_stack_array[MAX_TASKS] __attribute__((aligned (8)));
struct task_state tasks[MAX_TASKS];

#define TASK_STACK_VIRTUAL_BASE 0x80000000

void multitask_init(){
	print_serial("[TASK] Init Multitasking\n");
    task_stack_base = (void *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(PAGE_SIZE * MAX_TASKS), PAGE_SIZE * MAX_TASKS, 0, STACK));
    //tasks = (struct task_state *) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(sizeof(struct task_state) * MAX_TASKS), sizeof(struct task_state) * MAX_TASKS, 0, KERNEL));
    memset(tasks, 0, sizeof(struct task_state) * MAX_TASKS);
    MEM_printRegions();
    print_serial("[TASK] Setting Stack!\n");
    for(int i = 0; i < MAX_TASKS; i++){
        task_stack_array[i] = task_stack_base + (TASK_STACK_SIZE * i);
        print_serial("%d - 0x%x\n", i, task_stack_array[i]);
    }
}

void multitask_start(){
    task_lock = 0;
	timer_attach(1, task_callback);
}

void start_task(void *address, int8_t program_slot, int argc, char **argv, char* name __attribute__((unused))){
    if(name != NULL){
        print_serial("[TASK] Added Task %s to queue\n", name);
    }
    else{
        print_serial("[TASK] Added new Task to queue\n");
    }
	for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active == 0){
            tasks[i].registers.eax = argc;
            tasks[i].registers.ebx = (uint32_t) argv;
            tasks[i].registers.ecx = 0;
            tasks[i].registers.edx = 0;
            tasks[i].registers.eip = (uint32_t) address;
            register uint32_t esi asm("esi");
            register uint32_t edi asm("edi");
            tasks[i].registers.edi = edi;
            tasks[i].registers.esi = esi;
            
            tasks[i].stack_region = (uint32_t) task_stack_array[i];
            tasks[i].registers.ebp = (uint32_t) task_stack_array[i]+TASK_STACK_SIZE-(3*4);
            tasks[i].registers.esp = (uint32_t) tasks[i].registers.ebp-(8*4);

            uint32_t *return_instruction = (uint32_t *) tasks[i].registers.ebp;
            print_serial("RI: 0x%x\n", (uint32_t) return_instruction);
            return_instruction[1] = tasks[i].registers.ebp;
            return_instruction[-1] = (uint32_t) argv;
            return_instruction[-2] = (uint32_t) argc;
            return_instruction[-3] = (uint32_t) &task_end;


            tasks[i].program_slot = program_slot;
            tasks[i].schedule_type = ALWAYS;
            tasks[i].task_name = name;
            tasks[i].window = tasks[task_running_idx].window;
            tasks[i].own_window = false;
            tasks[i].console = tasks[task_running_idx].console;
            tasks[i].own_console = false;
            memcpy(tasks[i].currentDirectory.path, tasks[task_running_idx].currentDirectory.path, sizeof(tasks[i].currentDirectory.path));
            print_serial("[TASK] Added to queue idx %x\n", i);
            tasks[i].slot_active = 1;
            tasks[i].keyboard_event_handler = NULL;
            tasks[i].mouse_event_handler = NULL;
            tasks[i].end_callback = NULL;
            for(int j = 0; j < MT_maxDescriptors; j++){
                tasks[i].file_descs[j] = -1;
            }
            break;
        }
    }
}

void set_schedule(ScheduleType type){
    //print_serial("[TASK] Set Task %d schedule type to %d\n", task_running_idx, type);
    tasks[task_running_idx].schedule_type = type;
}

void stop_task(int8_t task_idx){
    if(tasks[task_idx].end_callback != NULL){
        select_program(tasks[task_idx].program_slot);
        tasks[task_idx].end_callback();
        if(tasks[task_running_idx].program_slot != -1){
            select_program(tasks[task_running_idx].program_slot);
        }
    }

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
    task_callback();
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
    
	while(1){}
}

void __attribute__ ((optimize("-O3"))) switch_to_task(struct task_state* old_task, struct task_state* new_task){
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

    //Load next task's state
    if(new_task->program_slot != -1){
        select_program(new_task->program_slot);
    }
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
    ////printk("[TASK] Callback\n");
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
            ////printk("Task is available\n");
            break;
        }
    }
    if(!task_available){
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
            if(tasks[next_idx].registers.eip == 0x400000){
                print_serial("[TASK] Funny thing... the next program is at the end of executable space...\n");
                print_serial("[TASK] %s\n", tasks[next_idx].task_name);
            }
            switch_to_task((struct task_state*) &tasks[running_idx], (struct task_state*) &tasks[next_idx]);
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
            print_serial("Task #%x Is Called %s\n", i, tasks[i].task_name);
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