;Give kernal entry point a global access
global loader
;External C functions
extern kmain	;Main Kernal Function

;Main code section, all kernal software will be loaded here.
section .text
bits 32		;System operates at 32 bits, all opcodes must match

extern kernel_start
extern kernel_end

global boot_page_directory_physical
boot_page_directory_physical equ (boot_page_directory - KERNEL_VIRTUAL_BASE)


loader:					;Kernal Load entry point.
	cli					;Clear interrupts
	;jmp higher_half
	mov ecx, boot_page_directory_physical 
	mov cr3, ecx

	mov ecx, cr4
	or ecx, 0x00000010
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx

	lea ecx, [higher_half]
	jmp ecx

higher_half:
	mov dword [boot_page_directory], 0
	invlpg [0]
	mov esp, kernel_stack+KERNEL_STACK_SIZE

	add ebx, KERNEL_VIRTUAL_BASE

	push ebx			;push multiboot info struct pointer
	push eax			;push magic number
	call kmain			;Call Kernal Main C Function *main.c*
	hlt
	jmp .loop			;If the kernal main ends, jump to infinite loop
	
.loop:
	jmp .loop


section .data
KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

MULTIBOOT_VIRTUAL_BASE equ 0x87400000
MULTIBOOT_PAGE_NUMBER equ (MULTIBOOT_VIRTUAL_BASE >> 22)

FB_VB equ 0xFE400000
FB_PN equ (FB_VB >> 22)

AHCI_VB equ 0xfe800000
AHCI_PN equ (AHCI_VB >> 22)

align 0x1000

global boot_page_directory
boot_page_directory:
	dd 0x00000083

	times (KERNEL_PAGE_NUMBER - 1) dd 0
	dd 0x00000083
	dd 0x00400083
	dd 0x00800083
	dd 0x00C00083
	dd 0x01000083
	dd 0x01400083
	times (AHCI_PN - KERNEL_PAGE_NUMBER - 6) dd 0
	;dd 0xfe800083
	dd 0
	times (1024 - AHCI_PN - 1) dd 0


KERNEL_STACK_SIZE equ 0x2000
section .bss
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE