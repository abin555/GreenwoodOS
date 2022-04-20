global loader
extern sum_of_three
extern main	
extern interrupt_handler

;%define Guaranteed_Work
%ifdef Guaranteed_Work
MAGIC_NUMBER 	equ 0x1BADB002
FLAGS			equ 0x0
CHECKSUM 		equ -MAGIC_NUMBER
%endif

;;Begin Executing C Code


%ifdef Guaranteed_Work
section .text
align 4
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM
%endif

%ifndef Guaranteed_Work
section .multiboot
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
%endif

section .text
bits 32
jmp loader


loader:
	cli

	mov dword [0xb8000], 0x2f4b2f4f
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


;GLOBAL DESCRIPTOR TABLE SETUP
global load_gdt

; GDT with a NULL Descriptor, a 32-Bit code Descriptor
; and a 32-bit Data Descriptor
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

; GDT descriptor record
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Load GDT and set selectors for a flat memory model
load_gdt:
    lgdt [gdt_descriptor]
    jmp CODE_SEG:.setcs              ; Set CS selector with far JMP
.setcs:
    mov eax, DATA_SEG                ; Set the Data selectors to defaults
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ret


;EXTERNAL TEST PROGRAMS

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