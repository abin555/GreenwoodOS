# 1 "pic.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "pic.c"
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

extern void outportl(uint16_t port, uint32_t value);
extern uint32_t inportl(uint16_t port);

static inline void outdw(uint16_t port, uint32_t data) {
 __asm__ volatile("out dx,eax" : : "a"(data), "d"(port));
}

static inline uint32_t indw(uint16_t port) {
 uint32_t data;
 __asm__ volatile("in eax,dx" : "=a"(data) : "d"(port));
 return data;
}

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void kreboot();
extern uint32_t * restore_kernel_addr;
extern void PROGA();

extern unsigned int *externalProgram;
# 2 "pic.c" 2
# 1 "./include/pic.h" 1
# 44 "./include/pic.h"
void pic_acknowledge(unsigned int interrupt);
void pic_remap(int offset1, int offset2);
# 3 "pic.c" 2

void pic_acknowledge(unsigned int interrupt){
    if ( interrupt < 0x20 || interrupt > 0x28 + 7){
        return;
    }

    if(interrupt < 0x28){
        outb(0x20, 0x20);
    } else{
        outb(0xA0, 0x20);
    }
}
void pic_remap(int offset1, int offset2)
{
 outb(0x20, 0x10 + 0x01);
 outb(0xA0, 0x10 + 0x01);
 outb((0x20 +1), offset1);
 outb((0xA0 +1), offset2);
 outb((0x20 +1), 4);
 outb((0xA0 +1), 2);

 outb((0x20 +1), 0x01);
 outb((0xA0 +1), 0x01);


 outb((0x20 +1), 0xFD);
 outb((0xA0 +1), 0xFF);

 asm("sti");
}
