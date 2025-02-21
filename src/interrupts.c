#include "interrupts.h"


struct IDTDescriptor idt_descriptors[INTERRUPT_DESCRIPTOR_COUNT] = {0};
struct IDT idt;

unsigned int BUFFER_COUNT;
struct cpu_state (*interrupt_handlers[INTERRUPT_DESCRIPTOR_COUNT])(struct cpu_state, struct stack_state);

unsigned int INT_currentInterrupt;

extern uint32_t saved_stack_ebp;
extern uint32_t saved_stack_esp;

void pic_acknowledge(unsigned int interrupt){

	if(interrupt >= 0x28){
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

void pic_remap(int offset1, int offset2){
	outb(PIC_1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);	// starts the initialization sequence (in cascade mode)
	outb(PIC_2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	outb(PIC_1_DATA, offset1);				// ICW2: Master PIC vector offset
	outb(PIC_2_DATA, offset2);				// ICW2: Slave PIC vector offset
	outb(PIC_1_DATA, 4);					// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC_2_DATA, 2);					// ICW3: tell Slave PIC its cascade identity (0000 0010)

	outb(PIC_1_DATA, PIC_ICW4_8086);
	outb(PIC_2_DATA, PIC_ICW4_8086);

        // Setup Interrupt Mask Register (IMR)
	outb(PIC_1_DATA, 0xFF); // 1111 1111 - Disable Everything
	outb(PIC_2_DATA, 0xFF);

	/*
    outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, offset1);
	outb(0xA1, offset2);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
    //asm("sti");    
	*/
}

void IRQ_set_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC_1_DATA;
    } else {
        port = PIC_2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
 
void IRQ_clear_mask(unsigned char IRQline) {
    print_serial("IRQ Clear Mask %d\n", IRQline);
        uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC_1_DATA;
    } else {
        port = PIC_2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);     
}

void interrupt_add_handle(uint8_t interrupt, void* handler){
	interrupt_handlers[interrupt] = handler;
}

void interrupts_init_descriptor(int index, void *address)
{
	idt_descriptors[index].offset_high = (((uint32_t) address) >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (((uint32_t) address) & 0xFFFF); // offset bits 16..31

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

void interrupts_install_idt()
{
	IRQ_OFF;
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

	interrupts_init_descriptor(0,  int_handler_0);
	interrupts_init_descriptor(1,  int_handler_1);
	interrupts_init_descriptor(2,  int_handler_2);
	interrupts_init_descriptor(3,  int_handler_3);
	interrupts_init_descriptor(4,  int_handler_4);
	interrupts_init_descriptor(5,  int_handler_5);
	interrupts_init_descriptor(6,  int_handler_6);
	interrupts_init_descriptor(7,  int_handler_7);
	interrupts_init_descriptor(8,  int_handler_8);
	interrupts_init_descriptor(9,  int_handler_9);
	interrupts_init_descriptor(10,  int_handler_10);
	interrupts_init_descriptor(11,  int_handler_11);
	interrupts_init_descriptor(12,  int_handler_12);
	interrupts_init_descriptor(13,  int_handler_13);
	interrupts_init_descriptor(14,  int_handler_14);
	interrupts_init_descriptor(15,  int_handler_15);
	interrupts_init_descriptor(16,  int_handler_16);
	interrupts_init_descriptor(17,  int_handler_17);
	interrupts_init_descriptor(18,  int_handler_18);
	interrupts_init_descriptor(32,  int_handler_32);
	interrupts_init_descriptor(33,  int_handler_33);
	interrupts_init_descriptor(34,  int_handler_34);
	interrupts_init_descriptor(35,  int_handler_35);
	interrupts_init_descriptor(36,  int_handler_36);
	interrupts_init_descriptor(37,  int_handler_37);
	interrupts_init_descriptor(38,  int_handler_38);
	interrupts_init_descriptor(39,  int_handler_39);
	interrupts_init_descriptor(40,  int_handler_40);
	interrupts_init_descriptor(41,  int_handler_41);
	interrupts_init_descriptor(42,  int_handler_42);
	interrupts_init_descriptor(43,  int_handler_43);
	interrupts_init_descriptor(44,  int_handler_44);
	interrupts_init_descriptor(45,  int_handler_45);
	
	interrupts_init_descriptor(50,  int_handler_50);
	interrupts_init_descriptor(51,  int_handler_51);
	interrupts_init_descriptor(52,  int_handler_52);
	interrupts_init_descriptor(53,  int_handler_53);

	interrupts_init_descriptor(128,  int_handler_128);

	idt.address = (uint32_t) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPT_DESCRIPTOR_COUNT;
	print_serial("Loading IDT at 0x%x of size %x\n", idt.address, idt.size);
	load_idt((uint32_t) &idt);

	IRQ_RES;
}

#include "console.h"
#include "multitasking.h"
struct cpu_state most_recent_int_cpu_state;
struct stack_state most_recent_int_stack_state;
bool override_state_return = false;

void interrupt_handler(){
	struct cpu_state *cpu = (struct cpu_state *) saved_stack_esp; 
	unsigned int interrupt = *(unsigned int *)(saved_stack_esp+sizeof(struct cpu_state)); 
	struct stack_state *stack = (struct stack_state *)(saved_stack_esp+sizeof(struct cpu_state)+sizeof(unsigned int));
	most_recent_int_cpu_state = *cpu;
	most_recent_int_stack_state = *stack;
	INT_currentInterrupt = interrupt;

	
	uint32_t *funny_stack = (uint32_t *) saved_stack_esp;
	for(int i = 0; i < 13; i++){
		print_serial("%d - 0x%x\n", i, funny_stack[i]);
	}
	
	//stack.eip = funny_stack->eip;
	//stack.error_code = funny_stack->error_code;
	//stack.cs = funny_stack->cs;
	//stack.eflags = funny_stack->eflags;

	#ifdef OS_DEBUG
	print_serial("Interrupt %d\n", interrupt);
	#endif
	print_serial("Saved ESP: 0x%x Saved EBP: 0x%x Int: %d Current EBP: 0x%x Current ESP: 0x%x EIP: 0x%x\n", saved_stack_esp, saved_stack_ebp, interrupt, cpu->ebp, cpu->esp, stack->eip);
	//print_serial("Saved ESP: 0x%x Saved EBP: 0x%x Int: %d Current EBP: 0x%x Current ESP: 0x%x EIP: 0x%x Saved EIP: 0x%x\n", saved_stack_esp, saved_stack_ebp, interrupt, cpu.ebp, cpu.esp, stack.eip, funny_stack->eip);
	if((uint32_t) interrupt_handlers[interrupt]){
		*cpu = interrupt_handlers[interrupt](*cpu, *stack);
		
		if(override_state_return == true){
			*(struct stack_state *)(saved_stack_esp+sizeof(struct cpu_state)+sizeof(unsigned int)) = most_recent_int_stack_state;
			*(struct cpu_state *) saved_stack_esp = most_recent_int_cpu_state;
			uint32_t *fix_the_dang_stack = (uint32_t *) cpu->esp;
			fix_the_dang_stack[0] = interrupt;
			fix_the_dang_stack[1] = stack->error_code;
			fix_the_dang_stack[2] = stack->eip;
			fix_the_dang_stack[3] = stack->cs;
			fix_the_dang_stack[4] = stack->eflags;
			override_state_return = false;
			return;
		}
		
	}
	else{
		print_serial("[CPU INT] Uninitialized Interrupt %d from 0x%x\n", interrupt, stack->eip);
		print_console(tasks[task_running_idx].console, "[CPU INT] Uninitialized Interrupt %x\n", interrupt);
		//printk("[CPU INT] Uninitialized Interrupt %x\n", interrupt);
	}
	//print_serial("[Interrupt] Return\n");
}

void IDT_dump(){
	print_serial("IDT Dump:\n");
	for(uint16_t vector = 0; vector < INTERRUPT_DESCRIPTOR_COUNT; vector++){
		if(idt_descriptors[vector].type_and_attr){
			uint32_t addr = (uint32_t) (idt_descriptors[vector].offset_high << 16) | (idt_descriptors[vector].offset_low);
			print_serial("(%x) at 0x%x\n", vector, addr);
		}
	}
}