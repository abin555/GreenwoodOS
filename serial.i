# 1 "serial.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "serial.c"
# 1 "./include/serial.h" 1


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
# 4 "./include/serial.h" 2

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write(unsigned int com, char b);
int init_serial();
# 2 "serial.c" 2
# 22 "serial.c"
void serial_configure_baud_rate(unsigned short com, unsigned short divisor){
  outb((com + 3),
       0x80);
  outb((com),
       (divisor >> 8) & 0x00FF);
  outb((com),
       divisor & 0x00FF);
}
# 38 "serial.c"
void serial_configure_line(unsigned short com){




  outb((com + 3), 0x03);
}
# 54 "serial.c"
int serial_is_transmit_fifo_empty(unsigned int com){
  return inb((com + 5)) & 0x20;
}

void serial_write(unsigned int com, char b){
  while(serial_is_transmit_fifo_empty(com) == 0){}
  outb(com, b);
}



int init_serial(){
   outb(0x3F8 + 1, 0x00);
   outb(0x3F8 + 3, 0x80);
   outb(0x3F8 + 0, 0x03);
   outb(0x3F8 + 1, 0x00);
   outb(0x3F8 + 3, 0x03);
   outb(0x3F8 + 2, 0xC7);
   outb(0x3F8 + 4, 0x0B);
   outb(0x3F8 + 4, 0x1E);
   outb(0x3F8 + 0, 0xAE);


   if(inb(0x3F8 + 0) != 0xAE) {
      return 1;
   }



   outb(0x3F8 + 4, 0x0F);
   return 0;
}
