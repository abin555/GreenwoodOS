#include "programs.h"

void exec_user_program(int pid, uint32_t args[10]){
    register uint32_t arg asm("eax")__attribute__((unused)) = args[0];
    asm volatile("call 0x00000000");    
    kill_process(pid);
    console_fullPut();
}