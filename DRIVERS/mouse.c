#include "mouse.h"

#define mouse_port1 0x60
#define mouse_port2 0x64
#define PS2_ACK 0xFA

void mouse_interrupt_handler(unsigned int interrupt){
    interrupt++;
}

void mouse_wait(uint8_t type){
    int timeout = 10000000;
    if(type == 0){
        while(timeout--){
            if((inb(0x64) & 1) == 1){
                return;
            }
        }
        return;
    }
    else{
        while(timeout--){
            if((inb(0x64) & 20) == 0){
                return;
            }
        }
        return;
    }
}
void mouse_write(uint8_t data){
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, data);
}
uint8_t mouse_read(){
    mouse_wait(0);
    uint8_t in = inb(0x60);
    printk("%x\n", in);
    return in;
}

void mouse_init(){
    uint8_t status;
    //Enable AUX mouse
    mouse_wait(1);
    outb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status = (inb(0x60));
    printk("Mouse | Status: %8b\n", status);
    status = status | 2;
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    //Use defaults
    mouse_write(0xF6);
    mouse_read(); //ACK

    //Enable Mouse
    mouse_write(0xF4);
    mouse_read(); //ACK
    printk("Mouse Init | Status: %8b\n", status);

    mouse_write(0xEB);
    printk("%x - ", mouse_read());
    printk("%x\n", mouse_read());
}