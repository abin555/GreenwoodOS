#include "timer.h"
#include "interrupts.h"

uint32_t timer_ticks = 0;
uint32_t timer_freq;

void delay(uint32_t msec){
    uint32_t start_tick = timer_ticks;
    while (timer_ticks - start_tick <= msec){}
}

void timer_init(uint32_t frequency){
    interrupt_add_handle(32, &timer_callback);
    IRQ_clear_mask(0);
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x40, l);
    outb(0x40, h);
    timer_freq = frequency;
}

void timer_callback(){
    timer_ticks++;
}