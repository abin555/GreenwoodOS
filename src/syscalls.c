#include "syscalls.h"

void init_syscalls(){
    interrupt_add_handle(0x80, &syscalls_callback);
    printk("[SYS] System Calls Initialized\n");
}

void syscalls_callback(struct cpu_state cpu, struct stack_state stack __attribute__((unused))){
    switch(cpu.eax){
        case 0x01:
            printk((char *) cpu.ebx);
            break;
        case 0x02:
            fb_setChar(fb_terminal_h-4,fb_terminal_w, cpu.ebx, 0x0000FF, 0);
            break;
        case 0x03:
            printk((char *) cpu.ebx, cpu.edx);
            break;
    }
}