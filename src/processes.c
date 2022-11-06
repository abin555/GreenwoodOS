#include "processes.h"

uint8_t add_process(void* program){
    processes[process_num].program = program;
    processes[process_num].running = 1;
    
    process_num++;
    return process_num-1;
}

void kill_process(uint8_t process_id){
    processes[process_id].running = 0;
}

void process_scheduler(){
    while(1){
        if(process_num){
            for(uint8_t i = 0; i < process_num; i++){
                if(processes[i].running){
                    processes[i].program(i);
                }
            }
        }
    }
}