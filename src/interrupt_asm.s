;Produce Interrupt Call System | interrupts_new.c
extern interrupt_handler

global saved_stack_ebp
saved_stack_ebp:
	dd 0
global saved_stack_esp
saved_stack_esp:
	dd 0

extern kernel_stack_base

%macro stack_switch 0
	mov [saved_stack_ebp], ebp
	mov [saved_stack_esp], esp
;	mov ebp, [kernel_stack_base]
;	add ebp, 0x80000-(1*4)
;	mov esp, ebp
;	sub esp, (2*4)

%endmacro

%macro no_err_int 1
global int_handler_%1
int_handler_%1:
	cli
	push dword 0 	;Give a 0 for an errorless interrupt, prevents kernel panics
	push dword %1	;Pass ID of interrupt to interrupt handler
	jmp common_interrupt_handler ;jump to the interrupt handler
%endmacro

%macro err_int_handler 1
global int_handler_%1
int_handler_%1:
	cli
	push dword %1 ;Pass ID of interrupt to interrupt handler
	jmp common_interrupt_handler ;jump to the interrupt handler
%endmacro

common_interrupt_handler:
	;save registers
	push eax
	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi

	push ds
	push es
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax

	push esp

	;Call C function handler
	call interrupt_handler
	mov esp, eax

	;restore registers
	pop gs
	pop fs
	pop es
	pop ds

	pop edi
	pop esi
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax

	;restore stack pointer
	add esp, 8

	;return the system
	iret

no_err_int 0
no_err_int 1
no_err_int 2
no_err_int 3
no_err_int 4
no_err_int 5
no_err_int 6
no_err_int 7
err_int_handler 8
no_err_int 9
err_int_handler 10
err_int_handler 11
err_int_handler 12
err_int_handler 13 ; General Protection Fault
err_int_handler 14 ; Page Fault
no_err_int 15
no_err_int 16
err_int_handler 17
err_int_handler 18
err_int_handler 19
err_int_handler 20
err_int_handler 21
no_err_int 22
no_err_int 23
no_err_int 24
no_err_int 25
no_err_int 26
no_err_int 27
no_err_int 28
no_err_int 29
no_err_int 30
no_err_int 31
no_err_int 32 ; handler for timer interrupt.
no_err_int 33 ; handler for interrupt 1 (keyboard)
no_err_int 34 ; kernel interrupt handler
no_err_int 35
no_err_int 36
no_err_int 37 ;OHCI IRQ: 5
no_err_int 38 ;UHCI IRQ: 6
no_err_int 39 ;EHCI IRQ: 7
no_err_int 40 ;XHCI IRQ: 8
no_err_int 41 ;AHCI IRQ: 9
no_err_int 42
no_err_int 43 ; interrupt 11 - e1000(?)
no_err_int 44 ; handler for interrupt 12 (mouse)
no_err_int 45
no_err_int 46
no_err_int 47
no_err_int 48
no_err_int 49
no_err_int 50 
no_err_int 51 
no_err_int 52 
no_err_int 53 

no_err_int 128 ; SYSTEM CALL INTERRUPT HANDLER

global load_idt
;load_idt - Loads the interrupt descriptor table
; stack: [esp + 4] the first entry in the IDT
;		 [esp	 ] the return address
load_idt:
	mov eax, [esp + 4]
	lidt [eax]
	ret
