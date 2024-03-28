#include "timer.h"

uint32_t timer_ticks, timer_freq, timer_attached_functions_num;
struct timer_function timer_attached_functions[0xFF];

void timer_init(uint32_t frequency){
	print_serial("[Timer] Starting Initialization\n");
	IRQ_OFF;
	uint32_t divisor = 1193180 / frequency;
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x43, 0x36);
    outb(0x40, l);
    outb(0x40, h);
    timer_freq = frequency;
	timer_ticks = 0;
    interrupt_add_handle(32, timer_callback);
    IRQ_clear_mask(0);
	memset(&timer_attached_functions, 0, sizeof(timer_attached_functions));
    print_serial("[Timer] Initialized at 0x%x hz\n", frequency);

	IRQ_RES;
}

struct cpu_state timer_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	pic_acknowledge(32);
	timer_ticks++;
	//print_serial("[Timer] Tick\n");
	for(uint32_t i = 0; i < timer_attached_functions_num; i++){
		if(timer_ticks % timer_attached_functions[i].divisor == 0){
			timer_attached_functions[i].attached_function();
		}
	}
	return cpu;
}

void timer_attach(int divisor, void *callback){
	timer_attached_functions[timer_attached_functions_num].attached_function = callback;
	timer_attached_functions[timer_attached_functions_num].divisor = divisor;
	timer_attached_functions_num++;
}