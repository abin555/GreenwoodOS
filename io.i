# 1 "io.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "io.c"
# 1 "./include/io.h" 1



# 1 "./include/gcc_stdint.h" 1
# 34 "./include/gcc_stdint.h"
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef int intptr_t;


typedef unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 5 "./include/io.h" 2

extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void PROGA();

extern unsigned int *externalProgram;
# 2 "io.c" 2

unsigned char *INT_Software_Value = (unsigned char *)0x10;

void software_interrupt(unsigned char interrupt){
 INT_Software_Value[0] = interrupt;
 software_int();
}

void WriteMem(uint32_t Address, uint32_t Value){
 *(volatile uint32_t*)Address = Value;
}

uint32_t ReadMem(uint32_t Address){
 return *(volatile uint32_t*)Address;
}
