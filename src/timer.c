#include "timer.h"

#define MAX_TIMER_FUNCS 0xFF

uint32_t timer_ticks, timer_freq, timer_attached_functions_num;
struct timer_function timer_attached_functions[MAX_TIMER_FUNCS];

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
	//timer_attached_functions_num = 0xFF;
    print_serial("[Timer] Initialized at 0x%x hz\n", frequency);

	IRQ_RES;
}

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

extern struct RealTimeClock RTC;

struct cpu_state timer_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
	pic_acknowledge(32);
	timer_ticks++;
	//print_serial("[Timer] Tick\n");
	for(uint32_t i = 0; i < MAX_TIMER_FUNCS; i++){
		if(timer_attached_functions[i].attached_function == NULL) continue;
		if(timer_ticks % timer_attached_functions[i].divisor == 0){
			timer_attached_functions[i].attached_function();
		}
	}
	return cpu;
}

void timer_attach(int divisor, void *callback){
	int idx = 0;
	for(idx = 0; idx < MAX_TIMER_FUNCS; idx++){
		if(timer_attached_functions[idx].attached_function == NULL) break;
	}
	if(idx == MAX_TIMER_FUNCS) return;
	print_serial("[TIMER] Attached function @%x\n", callback);

	timer_attached_functions[idx].attached_function = callback;
	timer_attached_functions[idx].divisor = divisor;
}

void timer_detach(void *callback){
	for(int i = 0; i < 0xFF; i++){
		if(timer_attached_functions[i].attached_function == callback){
			timer_attached_functions[i].attached_function = NULL;

			print_serial("[TIMER] Detached function @%x\n", callback);
		}
	}
}