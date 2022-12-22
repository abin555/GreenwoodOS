#include "processes.h"

uint8_t add_process(void* program, int args, ...){
    va_list listptd;
    va_start(listptd, args);

    processes[process_num].program = program;
    processes[process_num].running = 1;
    
    for(int i = 0; i < args; i++){
        processes[process_num].args[i] = va_arg(listptd, unsigned int);
    }

    process_num++;
    va_end(listptd);
    return process_num-1;
}

void kill_process(uint8_t process_id){
    processes[process_id].running = 0;
    process_num--;
}

void process_scheduler(){
    while(1){
        if(process_num){
            for(uint8_t i = 0; i < process_num; i++){
                if(processes[i].running){
                    processes[i].program(i, processes[i].args);
                }
            }
        }
    }
}