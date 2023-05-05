#include "multitasking.h"

void multitask_init(){
    printk("[TASK] Init Multitasking\n");
    timer_attach(2000, task_callback);
}
#define TASK_STACK_SIZE sizeof(uint32_t) * 0x100
void start_task(uint32_t address, int8_t program_slot, uint32_t arg, char* name __attribute__((unused))){
    for(int i = 0; i < MAX_TASKS; i++){
        if(tasks[i].slot_active == 0){
            tasks[i].registers.eax = arg;
            tasks[i].registers.ebx = 0;
            tasks[i].registers.ecx = 0;
            tasks[i].registers.edx = 0;
            tasks[i].registers.eip = address;
            register uint32_t esi asm("esi");
            register uint32_t edi asm("edi");
            tasks[i].registers.edi = edi;
            tasks[i].registers.esi = esi;
            if(program_slot == -1){
                //tasks[i].stack_region = (uint32_t) malloc(TASK_STACK_SIZE);
                //tasks[i].registers.ebp = tasks[i].stack_region+TASK_STACK_SIZE-(3*4);
                //tasks[i].registers.esp = (uint32_t) tasks[i].registers.ebp-(12*4);

                tasks[i].stack_region = (uint32_t) &task_stack_array[i];
                tasks[i].registers.ebp = (uint32_t) &task_stack_array[i]+TASK_STACK_SIZE-(3*4);
                tasks[i].registers.esp = (uint32_t) tasks[i].registers.ebp-(8*4);

                uint32_t *return_instruction = (uint32_t *) tasks[i].registers.ebp;
                return_instruction[1] = tasks[i].registers.ebp;
                return_instruction[-3] = (uint32_t) &task_end;
            }
            else{
                tasks[i].registers.ebp = 0x400000;
                tasks[i].registers.esp = 0x400000;

            }
            tasks[i].program_slot = program_slot;
            /*
            for(uint32_t i = 0; i < sizeof(tasks[i].task_name) && name[i] != 0; i++){
                tasks[i].task_name[i] = name[i];
            }
            */
            printk("[TASK] Added to queue idx %2x\n", i);
            tasks[i].slot_active = 1;
            break;
        }
    }
}

void stop_task(int8_t task_idx){
    tasks[task_idx].slot_active = 0;
}

void task_end(){
    tasks[task_running_idx].slot_active = 0;
    printk("Task %d Ended\n", task_running_idx);
    while(1){}
}

void switch_to_task(struct task_state* old_task, struct task_state* new_task){
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

void task_callback(){
    printk("[TASK] Callback\n");
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
            printk("Task is available\n");
            break;
        }
    }
    if(!task_available) return;
    /*
    printk("Task end is at 0x%x\n", (uint32_t) &task_end);
    printk("Dumping Task #%d's Top of Stack:\n", task_running_idx);
    for(unsigned int i = 0; i < (tasks[task_running_idx].stack_region + TASK_STACK_SIZE - tasks[task_running_idx].registers.esp)/4; i++){
        uint32_t *stack = (uint32_t *) tasks[task_running_idx].registers.esp;
        printk("0x%x ", stack+i);
        if(4*i + tasks[task_running_idx].registers.esp == tasks[task_running_idx].registers.ebp){
            printk("EBP ");
        }
        else if(4*i + tasks[task_running_idx].registers.esp == tasks[task_running_idx].registers.esp){
            printk("ESP ");
        }
        else{
            printk("    ");
        }
        printk("%d 0x%x\n", i, stack[i]);
    }
    */
    int8_t next_idx = running_idx + 1;
    while(1){
        if(next_idx == MAX_TASKS){
            next_idx = 0;
            continue;
        }
        if(tasks[next_idx].slot_active == 0 || tasks[next_idx].slot_running == 1){
            next_idx++;
            continue;
        }
        if(tasks[next_idx].slot_active){
            switch_to_task((struct task_state*) &tasks[running_idx], (struct task_state*) &tasks[next_idx]);
            tasks[running_idx].slot_running = 0;
            tasks[next_idx].slot_running = 1;
            running_idx = next_idx;
            task_running_idx = running_idx;
            printk("Switched to task %2x\n", running_idx);
            break;
        }
    }
}