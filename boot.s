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