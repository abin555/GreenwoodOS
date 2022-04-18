global loader
extern sum_of_three
extern main	
extern interrupt_handler

MAGIC_NUMBER 	equ 0x1BADB002
FLAGS		equ 0x0
CHECKSUM 	equ -MAGIC_NUMBER




;CREATE GDT TABLE
gdtr DW 0 ; For limit storage
     DD 0 ; For base storage


;;Begin Executing C Code

section .text

align 4
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM

jmp loader

loader:
	cli
	call main
	jmp .loop
	
.loop:
	jmp .loop


;SECTION FOR I/O ASM INTERFACE
global outb
outb:
	mov al, [esp+8]
	mov dx, [esp+4]
	out dx, al
	ret

global inb
inb:
	mov dx, [esp + 4]
	in al, dx
	ret

global loadGDT:
	lgdt [esp]
	ret

global externalProgram
externalProgram: dd program_A

global PROGA:
PROGA:
	mov eax, externalProgram
	jmp [externalProgram]

;Produce Interrupt Call System | interrupts_new.c
extern interrupt_handler


%macro no_err_int 1
global int_handler_%1
int_handler_%1:
	push dword 0
	push dword %1
	jmp common_interrupt_handler
%endmacro

%macro err_int_handler 1
global int_handler_%1
int_handler_%1:
	push dword %1
	jmp common_interrupt_handler
%endmacro

common_interrupt_handler:
	;save registers
	push edi
	push esi
	push ebp
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
	pop ebp
	pop esi
	pop edi

	;restore stack pointer
	add esp, 8

	;return the system
	iret

no_err_int 33 ; handler for interrupt 1 (keyboard)
no_err_int 34 ; kernel interrupt handler
no_err_int 128 ; SYSTEM CALL INTERRUPT HANDLER

global load_idt
;load_idt - Loads the interrupt descriptor table
; stack: [esp + 4] the first entry in the IDT
;		 [esp	 ] the return address

load_idt:
	mov eax, [esp + 4]
	lidt [eax]
	ret

;Software Interrupt Function
global software_int
software_int:
	INT 34

global restore_kernel
restore_kernel:
	jmp loader

section .PROGA
program_A:
	mov eax, 0x00000001
	mov ebx, 0x00000000
	mov ecx, 'A'
	jmp program_A_loop
program_A_loop:
	cmp ecx, 'z'+1
	je program_A_end
	int 0x80
	add ebx, 1
	inc ecx
	jmp program_A_loop
program_A_end:
	ret

section .PROGB
program_B:
	mov eax, 0x00000001
	mov ebx, 0x0000004E
	mov ecx, 'T'
	int 0x80
	ret