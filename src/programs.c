#include "programs.h"

void exec_user_program(int pid, uint32_t args[10]__attribute__((unused))){
    asm volatile("call 0x00000000");    
    kill_process(pid);
    console_fullPut();
}