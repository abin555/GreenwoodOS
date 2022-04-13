# 1 "pic.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "pic.c"
# 1 "./include/io.h" 1



extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
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
