#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"
#include "io.h"
#include "framebuffer.h"
#include "pic.h"
#include "console.h"

#define INTERRUPT_DESCRIPTOR_COUNT 256

struct IDT{
    uint16_t size;
    uint32_t address;
} __attribute__((packed));

struct IDTDescriptor{
    uint16_t offset_low;
    uint16_t segment_selector;

    uint8_t reserved;
    uint8_t type_and_attr;
    uint16_t offset_high;
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
	unsigned int esp;
} __attribute__((packed));

struct stack_state {
  /* the state of the stack */
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

void irq_remap();
void interrupts_init_desciptor(uint32_t index, uint32_t address);
void interrupts_install_idt();
extern struct cpu_state (*interrupt_handlers[0xFF])(struct cpu_state, struct stack_state);
void interrupt_add_handle(uint8_t interrupt, void *handler);
extern void load_idt(unsigned int idt_address);

extern struct cpu_state most_recent_int_cpu_state;
extern struct stack_state most_recent_int_stack_state;
void interrupt_handler(
    struct cpu_state cpu, 
    unsigned int interrupt, 
    struct stack_state stack);  //add the number of the interrupt as an argument.

void interrupt_add_handler(uint8_t interrupt, void* handler);

extern void int_handler_0();
extern void int_handler_1();
extern void int_handler_2();
extern void int_handler_3();
extern void int_handler_4();
extern void int_handler_5();
extern void int_handler_6();
extern void int_handler_7();
extern void int_handler_8();
extern void int_handler_9();
extern void int_handler_10();
extern void int_handler_11();
extern void int_handler_12();
extern void int_handler_13();
extern void int_handler_14();
extern void int_handler_15();
extern void int_handler_16();
extern void int_handler_17();
extern void int_handler_18();
extern void int_handler_32();
extern void int_handler_33();
extern void int_handler_34();

extern void int_handler_35();
extern void int_handler_36();
extern void int_handler_37();
extern void int_handler_38();
extern void int_handler_39();
extern void int_handler_40();
extern void int_handler_41();
extern void int_handler_42();
extern void int_handler_43();
extern void int_handler_44();

extern void int_handler_45();

extern void int_handler_46();
extern void int_handler_47();
extern void int_handler_48();
extern void int_handler_49();
extern void int_handler_50();
extern void int_handler_51();
extern void int_handler_52();
extern void int_handler_53();
extern void int_handler_54();
extern void int_handler_55();
extern void int_handler_56();

extern void int_handler_128();

#endif