bits 16
extern ap_startup
global ap_trampoline
extern apic_bspdone
extern aprunning
extern apic_stack_top
extern boot_page_directory_physical

ap_trampoline:
    cli
    cld
    jmp 0x0:0x8040

align 16
_L8010_GDT_table:
    dq 0                    ; null descriptor
    dd 0x0000FFFF           ; flat code descriptor (lower 4 bytes)
    dd 0x00CF9A00           ; flat code descriptor (upper 4 bytes)
    dd 0x0000FFFF           ; flat data descriptor (lower 4 bytes)
    dd 0x008F9200           ; flat data descriptor (upper 4 bytes)
    dd 0x00000068           ; tss descriptor (lower 4 bytes)
    dd 0x00CF8900           ; tss descriptor (upper 4 bytes)

_L8030_GDT_value:
    dw _L8030_GDT_value - _L8010_GDT_table - 1  ; size of GDT - 1
    dd _L8010_GDT_table                         ; address of GDT
    dd 0, 0                                     ; padding

align 64
_L8040:
    xor ax, ax
    mov ds, ax
    lgdt [dword 0x8030]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:0x8060

align 32
bits 32
_L8060:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    
    mov al, 'A'
    out 0xE9, al

    mov ecx, boot_page_directory_physical 
	mov cr3, ecx

	;ENABLE PSE for 4MB Pages
	mov ecx, cr4
	or ecx, 0x00000010
	mov cr4, ecx

    mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx

    ; get our Local APIC ID
    mov eax, 1
    cpuid
    shr ebx, 24
    mov edi, ebx

    ; setup 32k stack for this core
    shl ebx, 15
    mov esp, apic_stack_top
    sub esp, ebx

    push edi

.wait_bsp:
    pause
    ;cmp byte [apic_bspdone], 0
    ;je .wait_bsp

    lock inc byte [aprunning]

    ; jump into C code
    mov edi, ebx
    jmp 0x08:ap_startup
