#include "exceptions.h"

void exception_holding();

struct cpu_state exception_divide_by_zero(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	print_serial("Divide by zero from %s @ %x\n", tasks[task_running_idx].task_name, stack.eip);
	print_console(tasks[task_running_idx].console, "Divide by zero, terminating.\n");
	//print_stack_trace(cpu.ebx, 10);
	stop_task(task_running_idx);
	switch_to_task(&tasks[task_running_idx], &tasks[0]);
	return cpu;
}

struct cpu_state exception_invalid_opcode(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	print_serial("Invalid Opcode @ 0x%x\n", stack.eip);
	print_console(tasks[task_running_idx].console, "Invalid Opcode @ 0x%x\n", stack.eip);
	print_stack_trace(cpu.ebx, 10);
	stop_task(task_running_idx);
	override_state_return = true;
	most_recent_int_stack_state.eip = (uint32_t) &exception_holding;
	return cpu;
}

struct cpu_state exception_gpf(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	print_serial("General Protection Fault @ 0x%x Task %s\n", stack.eip, tasks[task_running_idx].task_name);
	print_serial("ESP: 0x%x\nEBP: 0x%x\n", cpu.esp, cpu.ebp);
	print_console(tasks[task_running_idx].console, "General Protection Fault @ 0x%x Task %s\n", stack.eip, tasks[task_running_idx].task_name);
	print_console(tasks[task_running_idx].console, "ESP: 0x%x\nEBP: 0x%x\n", cpu.esp, cpu.ebp);
	print_stack_trace(cpu.ebx, 10);
	//asm volatile ("hlt");
	stop_task(task_running_idx);
	override_state_return = true;
	most_recent_int_stack_state.eip = (uint32_t) &exception_holding;
	return cpu;
}

void exceptions_init(){
	interrupt_add_handle(0x0, exception_divide_by_zero);
	interrupt_add_handle(0x6, exception_invalid_opcode);
	interrupt_add_handle(0xD, exception_gpf);
	print_serial("[EXCEPTION] Handler Init Complete\n");
}

void exception_holding(){
	while(1){
	}
}