#include "syscalls.h"

void init_syscalls(){
    interrupt_add_handle(0x80, &syscalls_callback);
    printk("[SYS] System Calls Initialized\n");
}

void syscalls_callback(struct cpu_state cpu, struct stack_state stack __attribute__((unused))){
    char str[2];
    switch(cpu.eax){
        case 0x01:
            printk((char *) cpu.ebx);
            break;
        case 0x02:
            str[0] = cpu.ebx;
            str[1] = '\n';
            printk(str);
            break;
    }
}