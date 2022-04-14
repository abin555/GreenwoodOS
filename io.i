# 1 "io.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "io.c"
# 1 "./include/io.h" 1



extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
# 2 "io.c" 2

void software_interrupt(unsigned char interrupt){
 INT_Software_Value = interrupt;
 software_int();
}
