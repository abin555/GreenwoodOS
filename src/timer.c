#include "stddef.h"
#include "timer.h"

#define MAX_TIMER_FUNCS 0xFF

uint32_t timer_ticks, timer_freq, timer_attached_functions_num;
struct timer_function timer_attached_functions[MAX_TIMER_FUNCS];

void timer_init(uint32_t frequency){
	print_serial("[Timer] Starting Initialization\n");
    fb_putChar(8, 0, '1', 0xFF, 0x0);
	IRQ_OFF;
	uint32_t divisor = 1193180 / frequency;
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x43, 0x36);
    outb(0x40, l);
    outb(0x40, h);
	fb_putChar(8, 0, '2', 0xFF, 0x0);
    timer_freq = frequency;
	timer_ticks = 0;
    interrupt_add_handle(32, timer_callback);
	fb_putChar(8, 0, '3', 0xFF, 0x0);
    IRQ_clear_mask(0);
	memset(&timer_attached_functions, 0, sizeof(timer_attached_functions));
	//timer_attached_functions_num = 0xFF;
    print_serial("[Timer] Initialized at 0x%x hz\n", frequency);
	fb_putChar(8, 0, '4', 0xFF, 0x0);
	IRQ_RES;
	fb_putChar(8, 0, '5', 0xFF, 0x0);
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

void timer_callback(void *t __attribute__((unused))){
	pic_acknowledge(32);
	timer_ticks++;
	//fb_putChar(8, 8, 'T', 0xFF, 0x0);
	print_serial("[Timer] Tick\n");
	for(uint32_t i = 0; i < MAX_TIMER_FUNCS; i++){
		if(timer_attached_functions[i].attached_function == NULL) continue;
		if(timer_attached_functions[i].divisor == 0) continue;
		if(timer_ticks % timer_attached_functions[i].divisor == 0){
			timer_attached_functions[i].attached_function();
		}
	}
	//fb_putChar(8, 8, '@', 0xFF, 0x0);
	return;
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

void wait(int ticks){
	uint32_t start = timer_ticks;
	while(timer_ticks < start + ticks){
	}
}