;Produce Interrupt Call System | interrupts_new.c
extern interrupt_handler


%macro no_err_int 1
global int_handler_%1
int_handler_%1:
	push dword 0 	;Give a 0 for an errorless interrupt, prevents kernel panics
	push dword %1	;Pass ID of interrupt to interrupt handler
	jmp common_interrupt_handler ;jump to the interrupt handler
%endmacro

%macro err_int_handler 1
global int_handler_%1
int_handler_%1:
	push dword %1 ;Pass ID of interrupt to interrupt handler
	jmp common_interrupt_handler ;jump to the interrupt handler
%endmacro

common_interrupt_handler:
	;save registers
	push ebp
	push esp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax

	;Call C function handler
	call interrupt_handler

	;restore registers
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop esp
	pop ebp

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
err_int_handler 13
err_int_handler 14
no_err_int 15
no_err_int 16
no_err_int 17
no_err_int 18

no_err_int 32
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
no_err_int 43
no_err_int 44 ; handler for interrupt 12 (mouse)

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