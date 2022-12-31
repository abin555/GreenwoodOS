#include "timer.h"
#include "serial.h"

uint32_t timer_ticks = 0;
uint32_t timer_freq;
uint32_t timer_attached_functions_num;

void delay(uint32_t msec){
    uint32_t start_tick = timer_ticks;
    while (timer_ticks - start_tick <= msec){}
}

void init_timer(uint32_t frequency){
    uint32_t divisor = 1193180 / frequency;
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x43, 0x36);
    outb(0x40, l);
    outb(0x40, h);
    timer_freq = frequency;
    interrupt_add_handle(32, &timer_callback);
    IRQ_clear_mask(0);
    printk("[Timer] Initialized at %xhz\n", frequency);
}

struct cpu_state timer_callback(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    timer_ticks++;
    if(timer_ticks % 2) fb_setChar(fb_terminal_w-1, fb_terminal_h-3, '0', 0xFFFFFF, 0);
    else fb_setChar(fb_terminal_w-1, fb_terminal_h-3, '*', 0xFFFFFF, 0);
    for(uint32_t i = 0; i < timer_attached_functions_num; i++){
        if(timer_ticks % timer_attached_functions[i].divisor == 0){
            timer_attached_functions[i].attached_function();
        }
    }
    return cpu;
}

void timer_attach(int divisor, void* callback){
    timer_attached_functions[timer_attached_functions_num].attached_function = callback;
    timer_attached_functions[timer_attached_functions_num].divisor = divisor;
    timer_attached_functions_num++;
}