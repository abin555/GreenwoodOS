#include "programs.h"

void exec_user_program(int pid){
    asm volatile("call 0x00000000");    
    kill_process(pid);
}