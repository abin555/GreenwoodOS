#include "exceptions.h"
#include "task.h"

void exception_holding();

void exception_divide_by_zero(void *t){
	struct gwos_task *task = t;
	print_serial("Divide by zero from %s @ 0x%x\n", tasks[task_running_idx].task_name, task->state->eip);
	print_console(tasks[task_running_idx].console, "Divide by zero, terminating.\n");
	//print_stack_trace(task->state->ebx, 10);
	stop_task(task_running_idx);
	switch_to_task(&tasks[task_running_idx], &tasks[0], task_running_idx, 0);
	return;
}

void exception_invalid_opcode(void *t){
	struct gwos_task *task = t;
	print_serial("Invalid Opcode @ 0x%x\n", task->state->eip);
	print_console(tasks[task_running_idx].console, "Invalid Opcode @ 0x%x\n", task->state->eip);
	print_stack_trace(task->state->ebx, 10);
	stop_task(task_running_idx);
	override_state_return = true;
	most_recent_int_stack_state.eip = (uint32_t) &exception_holding;
	return;
}

void exception_gpf(void *t){
	struct gwos_task *task = t;
	print_serial("General Protection Fault @ 0x%x Task %s\n", task->state->eip, tasks[task_running_idx].task_name);
	print_serial("ESP: 0x%x\nEBP: 0x%x\n", task->state->esp, task->state->ebp);
	print_console(tasks[task_running_idx].console, "General Protection Fault @ 0x%x Task %s\n", task->state->eip, tasks[task_running_idx].task_name);
	print_console(tasks[task_running_idx].console, "ESP: 0x%x\nEBP: 0x%x\n", task->state->esp, task->state->ebp);
	//print_stack_trace(task->state->ebx, 10);
	//asm volatile ("hlt");
	//stop_task(task_running_idx);
	//override_state_return = true;
	//most_recent_int_stack_state.eip = (uint32_t) &exception_holding;
	return;
}

void exception_3(void *t){
	struct gwos_task *task = t;
	print_serial("Exception #3 from %s @ %x\n", tasks[task_running_idx].task_name, task->state->eip);
	//print_stack_trace(task->state->ebx, 10);
	stop_task(task_running_idx);
	switch_to_task(&tasks[task_running_idx], &tasks[0], task_running_idx, 0);
	return;
}

void exceptions_init(){
	interrupt_add_handle(0x0, exception_divide_by_zero);
	interrupt_add_handle(0x6, exception_invalid_opcode);
	interrupt_add_handle(0xD, exception_gpf);
	interrupt_add_handle(0x3, exception_3);
	print_serial("[EXCEPTION] Handler Init Complete\n");
}

void exception_holding(){
	while(1){
	}
}