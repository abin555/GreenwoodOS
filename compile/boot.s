;Give kernal entry point a global access
global loader
;External C functions
extern kmain	;Main Kernal Function
extern interrupt_handler ; Interrupt Handler External

;%define FORMER_SYS
;;Section to define GRUB system boot headers
section .multiboot
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0	                     ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here
;%define FORMER_SYS
%ifndef FORMER_SYS
align 8
framebuffer_tag_start:  
	DW 5 ;MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	DW 1 ;MULTIBOOT_HEADER_TAG_OPTIONAL
	DD framebuffer_tag_end - framebuffer_tag_start
	DD 1024
	DD 768
	DD 32
framebuffer_tag_end:
align 8
%endif
; required end tag
	dw 0    ; type
	dw 0    ; flags
	dd 8    ; size
header_end:



;Main code section, all kernal software will be loaded here.
section .text
bits 32		;System operates at 32 bits, all opcodes must match
jmp loader	;Immediately jump to kernal loader


loader:					;Kernal Load entry point.
	cli					;Clear interrupts
	push ebx			;push multiboot info struct pointer
	push eax			;push magic number
	call kmain			;Call Kernal Main C Function *main.c*
	hlt
	jmp .loop			;If the kernal main ends, jump to infinite loop
	
.loop:
	jmp .loop


;SECTION FOR I/O ASM INTERFACE
global outb			;Make OUTB function global to C kernal
outb:
	mov al, [esp+8] ;Load contents of stack into lower A register | OUTPUT PORT
	mov dx, [esp+4] ;Load contents of stack into lower B register | OUTPUT DATA
	out dx, al 		;Send OUTPUT
	ret				;Return to caller

global inb			;Make INB function global to C kernal
inb:
	mov dx, [esp + 4] 	;Load contents of stack into lower D register | POS
	in al, dx			;Recieve input and put into al register as 8 bit value (C lang Char)
	ret					;Return to caller



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

global externalProgram ;Pointer to the address of program regions
externalProgram: dd program_A ;Used in C code to change the execution region of code.

global PROGA: 					;Global function to call and execute external program.
PROGA:
	jmp [externalProgram]		;Jump to program pointer
SYS_STATE: DB 0 ;0 = Kernel 1 = PROG



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
;*DEFUNCT*
global software_int
software_int:
	INT 34

global restore_kernel
restore_kernel:
	jmp loader


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
	mov ebx, 0x00000100
	mov ecx, 'T'
	int 0x80
	mov eax, 4
	mov ebx, 130
	int 0x80
	mov eax, 0x00000002
	mov ebx, Message_PROGB
	mov ecx, 28
	mov edx, 0
	int 0x80
	mov eax, 5
	mov ebx, 600
	mov ecx, 600
	mov edx, 0x2EFFE2
	int 0x80
	ret
Message_PROGB: DB "HELLO WORLD SERIOUSLY THOUGH"