global loader
extern kmain
extern interrupt_handler
extern kmain_loop

section .multiboot
header_start:
    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    dw 0
    dw 0
    dd 8
header_end:

section .text
bits 32
jmp loader

loader:
    cli
    push ebx
    push eax
    call kmain
    hlt
    jmp .loop

.loop:
    jmp .loop

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