#include "interrupts.h"

struct IDTDescriptor idt_descriptors[0xFF];
struct IDT idt;

unsigned int BUFFER_COUNT;
struct cpu_state (*interrupt_handlers[0xFF])(struct cpu_state, struct stack_state);

void interrupt_add_handle(uint8_t interrupt, void* handler){
	interrupt_handlers[interrupt] = handler;
}

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

void interrupts_install_idt()
{
	IRQ_OFF;
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

	interrupts_init_descriptor(0, (unsigned int) int_handler_0);
	interrupts_init_descriptor(1, (unsigned int) int_handler_1);
	interrupts_init_descriptor(2, (unsigned int) int_handler_2);
	interrupts_init_descriptor(3, (unsigned int) int_handler_3);
	interrupts_init_descriptor(4, (unsigned int) int_handler_4);
	interrupts_init_descriptor(5, (unsigned int) int_handler_5);
	interrupts_init_descriptor(6, (unsigned int) int_handler_6);
	interrupts_init_descriptor(7, (unsigned int) int_handler_7);
	interrupts_init_descriptor(8, (unsigned int) int_handler_8);
	interrupts_init_descriptor(9, (unsigned int) int_handler_9);
	interrupts_init_descriptor(10, (unsigned int) int_handler_10);
	interrupts_init_descriptor(11, (unsigned int) int_handler_11);
	interrupts_init_descriptor(12, (unsigned int) int_handler_12);
	interrupts_init_descriptor(13, (unsigned int) int_handler_13);
	interrupts_init_descriptor(14, (unsigned int) int_handler_14);
	interrupts_init_descriptor(15, (unsigned int) int_handler_15);
	interrupts_init_descriptor(16, (unsigned int) int_handler_16);
	interrupts_init_descriptor(17, (unsigned int) int_handler_17);
	interrupts_init_descriptor(18, (unsigned int) int_handler_18);
	interrupts_init_descriptor(32, (unsigned int) int_handler_32);
	interrupts_init_descriptor(33, (unsigned int) int_handler_33);
	interrupts_init_descriptor(34, (unsigned int) int_handler_34);
	interrupts_init_descriptor(35, (unsigned int) int_handler_35);
	interrupts_init_descriptor(36, (unsigned int) int_handler_36);
	interrupts_init_descriptor(37, (unsigned int) int_handler_37);
	interrupts_init_descriptor(38, (unsigned int) int_handler_38);
	//interrupts_init_descriptor(39, (unsigned int) int_handler_39);
	interrupts_init_descriptor(40, (unsigned int) int_handler_40);
	interrupts_init_descriptor(41, (unsigned int) int_handler_41);
	interrupts_init_descriptor(42, (unsigned int) int_handler_42);
	interrupts_init_descriptor(43, (unsigned int) int_handler_43);
	interrupts_init_descriptor(44, (unsigned int) int_handler_44);
	
	interrupts_init_descriptor(50, (unsigned int) int_handler_50);
	interrupts_init_descriptor(51, (unsigned int) int_handler_51);
	interrupts_init_descriptor(52, (unsigned int) int_handler_52);
	interrupts_init_descriptor(53, (unsigned int) int_handler_53);

	interrupts_init_descriptor(128, (unsigned int) int_handler_128);

	idt.address = (int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPT_DESCRIPTOR_COUNT;

	load_idt((int) &idt);

	IRQ_RES;
}

struct cpu_state most_recent_int_cpu_state;
struct stack_state most_recent_int_stack_state;
bool override_state_return = false;

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, __attribute__((unused)) unsigned int interrupt, __attribute__((unused)) struct stack_state stack){
	most_recent_int_cpu_state = cpu;
	most_recent_int_stack_state = stack;

	if((uint32_t) interrupt_handlers[interrupt]){
		cpu = interrupt_handlers[interrupt](cpu, stack);
		if(override_state_return == true){
			//printk("Interrupt: %2x\n", interrupt);
			//printk("EFLAG: 0x%x\nCS: 0x%x\nEIP: 0x%x\nERRCODE: 0x%x\nINT: 0x%2x\n", stack.eflags, stack.cs, stack.eip, stack.error_code, interrupt);

			//printk("EIP Original: %x\n", stack.eip);
			//printk("ESP: %x EBP %x\n", cpu.esp, cpu.ebp);
			/*
			printk("Stack Dump:\n");
			for(unsigned int i = 0; i < 15; i++){
				uint32_t *stack = (uint32_t *) cpu.esp;
				printk("%d - 0x%x\n", i, stack[i]);
			}
			*/
			stack = most_recent_int_stack_state;
			cpu = most_recent_int_cpu_state;
			//printk("EFLAG: 0x%x\nCS: 0x%x\nEIP: 0x%x\nERRCODE: 0x%x\nINT: 0x%2x\n", stack.eflags, stack.cs, stack.eip, stack.error_code, interrupt);
			//printk("Changing EIP to %x\n", stack.eip);
			//printk("Changing ESP %x EBP %x\n", cpu.esp, cpu.ebp);
			uint32_t *fix_the_dang_stack = (uint32_t *) cpu.esp;
			fix_the_dang_stack[0] = interrupt;
			fix_the_dang_stack[1] = stack.error_code;
			fix_the_dang_stack[2] = stack.eip;
			fix_the_dang_stack[3] = stack.cs;
			fix_the_dang_stack[4] = stack.eflags;
			/*
			printk("New Stack Dump:\n");
			for(unsigned int i = 0; i < 15; i++){
				uint32_t *stack = (uint32_t *) cpu.esp;
				printk("%d - 0x%x\n", i, stack[i]);
			}
			*/
			override_state_return = false;
			return;
		}
	}
	else if(interrupt == 0xD){
		printk("General Protection Fault @ 0x%x\n", stack.eip);
		printk("ESP: 0x%x\nEBP: 0x%x\n", cpu.esp, cpu.ebp);
		asm volatile ("hlt");
	}
	else{
		printk("[CPU INT] Uninitialized Interrupt %x\n", interrupt);
	}
}

