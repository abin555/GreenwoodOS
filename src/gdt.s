;GLOBAL DESCRIPTOR TABLE SETUP
global load_gdt

; GDT with a NULL Descriptor, a 32-Bit code Descriptor
; and a 32-bit Data Descriptor
gdt_start:
gdt_null:
    dw 0, 0, 0, 0
gdt_kcode:
    dw 0xffff, 0x0000, 0x9a00, 0x00cf
gdt_kdata:
    dw 0xffff, 0x0000, 0x9200, 0x00cf
gdt_ucode:
    dw 0xffff, 0x0000, 0xfa00, 0x00cf
gdt_udata:
    dw 0xffff, 0x0000, 0xf200, 0x00cf
gdt_tss:
    dw 0x0068, 0x0000, 0x8901, 0x00cf
gdt_end:

; GDT descriptor record
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

KERN_CODE_SEG equ gdt_kcode - gdt_start
KERN_DATA_SEG equ gdt_kdata - gdt_start
USER_CODE_SEG equ gdt_ucode - gdt_start
USER_DATA_SEG equ gdt_udata - gdt_start
TSS_SEG equ gdt_tss - gdt_start

; Load GDT and set selectors for a flat memory model
load_gdt:
    mov eax, tss
    mov word [gdt_tss + 2], ax
    shr eax, 16
    mov byte [gdt_tss + 4], al
    mov byte [gdt_tss + 7], ah
    lgdt [gdt_descriptor]
    jmp KERN_CODE_SEG:normal_setds              ; Set CS selector with far JMP
global normal_setds
normal_setds:
    mov eax, KERN_DATA_SEG                ; Set the Data selectors to defaults
    mov ds, eax
    mov es, eax
    mov ss, eax
    mov ax, TSS_SEG
    ltr ax
    mov eax, 0
    mov fs, eax
    mov gs, eax
    ret


align 16
global tss
tss:
    dd 0

extern kernel_stack
global interrupt_stack_pointer
interrupt_stack_pointer:
    dd kernel_stack
    dd 2 * 8
    times 22 dd 0