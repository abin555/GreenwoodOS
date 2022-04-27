# 1 "memory.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "memory.c"
# 1 "./include/memory.h" 1



# 1 "./include/types.h" 1
# 21 "./include/types.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long int i64;
# 5 "./include/memory.h" 2

void memcpy(u64* source, u64* target, u64 len);
# 2 "memory.c" 2

void memcpy(u64* source, u64* target, u64 len){
    for(u64 index = 0; index < len; index++){
        *(volatile u64*)(target+index) = *(volatile u64*)(source+index);
    }
}
