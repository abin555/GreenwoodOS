#include "memory.h"

void memcpy(u64* source, u64* target, u64 len){
    for(u64 index = 0; index < len; index++){
        *(volatile u64*)(target+index) = *(volatile u64*)(source+index);
    }
}