#include "io.h"
#include "interrupts.h"
#include "keyboard.h"
#include "frame_buffer.h"
#include "pic.h"
#include "terminal.h"
#include "console.h"
#include "mouse.h"

#define INTERRUPT_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
#define INTERRUPTS_KERNEL 34
#define INTERRUPTS_MOUSE 44
#define INTERRUPTS_SYSCALL 0x80

unsigned char SYS_MODE = 1;

struct IDTDescriptor idt_descriptors[INTERRUPT_DESCRIPTOR_COUNT];
struct IDT idt;

unsigned int BUFFER_COUNT;

void interrupts_init_descriptor(int index, unsigned int address)
{
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (address & 0xFFFF); // offset bits 16..31

	idt_descriptors[index].segment_selector = 0x08; // The second (code) segment selector in GDT: one segment is 64b.
	idt_descriptors[index].reserved = 0x00; // Reserved.

	/*
	   Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
	   Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
		P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
		DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
		S	Storage Segment. Set to 0 for interrupt gates.
		D	Size of gate, (1 = 32 bits, 0 = 16 bits).
	*/
	idt_descriptors[index].type_and_attr =	(0x01 << 7) |			// P
											(0x00 << 6) | (0x00 << 5) |	// DPL
											0xe;				// 0b1110=0xE 32-bit interrupt gate
}

void interrupt_install_idt()
{
	asm("cli");
	//irq_remap();
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
	
	//IRQ_clear_mask(0);
	//IRQ_clear_mask(1);
	IRQ_clear_mask(2);
	//IRQ_clear_mask(3);
	//IRQ_clear_mask(4);
	//IRQ_clear_mask(5);
	//IRQ_clear_mask(6);
	//IRQ_clear_mask(7);
	IRQ_clear_mask(12);

	interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (unsigned int) int_handler_33);
	interrupts_init_descriptor(INTERRUPTS_KERNEL, (unsigned int) int_handler_34);
	//interrupts_init_descriptor(INTERRUPTS_MOUSE, (unsigned int) int_handler_44);
	interrupts_init_descriptor(INTERRUPTS_SYSCALL, (unsigned int) int_handler_128);

	idt.address = (int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPT_DESCRIPTOR_COUNT;
	load_idt((int) &idt);

	/*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
	asm("sti");

}

void KERNEL_INTERRUPT(){
	switch(INT_Software_Value[0]){
		case 1:
			SYS_MODE = 1;
			break;
		case 2:
			SYS_MODE = 2;
			break;
		case 3:
			SYS_MODE = 3;
			break;
		case 4:
			SYS_MODE = 4;
			break;
	}
}
void SYS_CALL(struct cpu_state cpu){
	switch(cpu.eax){
		case SYS_PRINT_CHAR:
			fb_write_cell(cpu.ebx, cpu.ecx, FG, BG);
			break;
		case SYS_PRINT_F:
			fb_write_start((char *) cpu.ebx, cpu.ecx, cpu.edx);
			break;
		case SYS_SET_CURSOR:
			fb_move_cursor(cpu.ebx);
			break;
		case SYS_YIELD:
			break;
		case SYS_PIXEL:
			fb_setPixel(cpu.ebx, cpu.ecx, cpu.edx);
			break;
		case SYS_BREAK:
			restore_kernel();
			break;
	}
}

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, unsigned int interrupt, __attribute__((unused)) struct stack_state stack)
{
	//unsigned char scan_code;
	//unsigned char ascii;;
	//printk("Interrupt: %2h\n", interrupt);
	switch (interrupt){
		case INTERRUPTS_KEYBOARD:
            keyboard_handle_interrupt(interrupt);
			break;
		case INTERRUPTS_KERNEL:
			KERNEL_INTERRUPT();
			break;
		case INTERRUPTS_SYSCALL:
			SYS_CALL(cpu);
			break;
		case INTERRUPTS_MOUSE:
			//printk("Mouse Interrupt\n");
			mouse_interrupt_handler();
			pic_acknowledge(interrupt); 
			break;
		default:
			break;
    }
}