#include "pcspk.h"

void play_sound(uint32_t frequency){
    uint32_t div;
    uint8_t temp;

    div = 1193180 / frequency;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) div);
    outb(0x42, (uint8_t) (div >> 8));

    temp = inb(0x61);
    if(temp != (temp | 3)){
        outb(0x61, temp | 3);
    }
}

void mute(){
    uint8_t temp = inb(0x61) & 0xFC;
    outb(0x61, temp);
}

void beep(){
    play_sound(1000);
    delay(100);
    mute();
}