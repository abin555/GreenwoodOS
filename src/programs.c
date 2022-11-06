#include "programs.h"

void exec_user_program(int pid){
    printk("Executing Program\n--------\n");

    asm volatile("call 0x00000000");
    //program();
    
    printk("--------\nExecuted Program\n");
    
    kill_process(pid);
}