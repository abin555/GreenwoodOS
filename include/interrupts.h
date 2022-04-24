#ifndef INTERRUPT_H
#define INTERRUPT_H

unsigned char SYS_MODE;
#include "system_calls.h"

struct IDT 
{
	unsigned short size;
	unsigned int address;
} __attribute__((packed));

struct IDTDescriptor {
  /* An entry in the IDT for an interrupt handler consists of 64 bits. */
	/* The lowest 32 bits */
	unsigned short offset_low; // offset bits 0..15
	unsigned short segment_selector; // a code segment selector in GDT or LDT
	
	/* The highest 32 bits */
	unsigned char reserved; // Just 0.
	unsigned char type_and_attr; // type and attributes
	unsigned short offset_high; // offset bits 16..31
} __attribute__((packed));

struct cpu_state {
  /* the state of the registers */
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebp; 
	unsigned int esi; 
	unsigned int edi; 
} __attribute__((packed));

struct stack_state {
  /* the state of the stack */
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

void interrupt_install_idt();
extern void int_handler_33();
extern void int_handler_34();
extern void int_handler_35();
extern void int_handler_128();

void load_idt(unsigned int idt_address);

void KERNEL_INTERRUPT();
void SYS_CALL(
	struct cpu_state cpu;
);


void interrupt_handler(
    struct cpu_state cpu, 
    unsigned int interrupt, 
    struct stack_state stack);  //add the number of the interrupt as an argument.

#endif