#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "stdbool.h"
#include "stdint.h"
#include "io.h"
#include "framebuffer.h"
#include "serial.h"
#include "processor_state.h"
#include "tasking.h"



#define INTERRUPT_DESCRIPTOR_COUNT 256
#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

// The PIC interrupts have been remapped
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   PIC2_START_INTERRUPT + 7

#define PIC_ACK     0x20

#define PIC_1		0x20		/* IO base address for master PIC */
#define PIC_2		0xA0		/* IO base address for slave PIC */
#define PIC_1_COMMAND	PIC_1
#define PIC_1_DATA	(PIC_1+1)
#define PIC_2_COMMAND	PIC_2
#define PIC_2_DATA	(PIC_2+1)

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28
#define PIC_2_END PIC_2_OFFSET + 7

#define PIC_1_COMMAND_PORT 0x20
#define PIC_2_COMMAND_PORT 0xA0
#define PIC_ACKNOWLEDGE 0x20

#define PIC_ICW1_ICW4            0x01	/* ICW4 (not) needed */
#define PIC_ICW1_SINGLE          0x02	/* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4       0x04	/* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL           0x08	/* Level triggered (edge) mode */
#define PIC_ICW1_INIT            0x10	/* Initialization - required! */

#define PIC_ICW4_8086            0x01	/* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO            0x02	/* Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE       0x08	/* Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER      0x0C	/* Buffered mode/master */
#define PIC_ICW4_SFNM            0x10	/* Special fully nested (not) */

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
	unsigned int esi; 
	unsigned int edi;
	unsigned int ebp;  
	unsigned int esp;
} __attribute__((packed));

struct stack_state {
  /* the state of the stack */
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

void pic_acknowledge(unsigned int interrupt);
void pic_remap(int offset1, int offset2);
void IRQ_clear_mask(unsigned char IRQline);
void IRQ_set_mask(unsigned char IRQline);

void irq_remap();

void interrupts_init_desciptor(uint32_t index, void *address);
void interrupts_install_idt();

extern void (*interrupt_handlers[INTERRUPT_DESCRIPTOR_COUNT])(struct gwos_task *state);

void interrupt_add_handle(uint8_t interrupt, void (*handler)(void *state));
extern void load_idt(unsigned int idt_address);

extern struct cpu_state most_recent_int_cpu_state;
extern struct stack_state most_recent_int_stack_state;
extern bool override_state_return;
struct processor_state *interrupt_handler(struct processor_state *state);
extern unsigned int INT_currentInterrupt;
void IDT_dump();

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

extern void int_handler_50();
extern void int_handler_51();
extern void int_handler_52();
extern void int_handler_53();

extern void int_handler_128();

#endif