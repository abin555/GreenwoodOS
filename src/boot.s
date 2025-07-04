global loader
extern kmain

section .text

bits 32

extern kernel_start
extern kernel_end

global boot_page_directory_physical
boot_page_directory_physical equ (boot_page_directory - KERNEL_VIRTUAL_BASE)

loader:
	cli
	mov ecx, boot_page_directory_physical 
	mov cr3, ecx

	;ENABLE PSE for 4MB Pages
	mov ecx, cr4
	or ecx, 0x00000010
	mov cr4, ecx

	mov ecx, cr0
	or ecx, 0x80000001
	mov cr0, ecx

	lea ecx, [higher_half]
	jmp ecx

higher_half:
	mov dword [boot_page_directory], 0
	invlpg [0]
	mov esp, kernel_stack+KERNEL_STACK_SIZE
	mov ebp, esp

	add ebx, KERNEL_VIRTUAL_BASE

	push ebx			;push multiboot info struct pointer
	push eax			;push magic number
	call kmain			;Call kernel Main C Function *main.c*
	hlt
	jmp .loop			;If the kernel main ends, jump to infinite loop
	
.loop:
	jmp .loop


section .data
KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernels 4MB PTE.

align 0x1000

global boot_page_directory
boot_page_directory:
	dd 0x00000083

	times (KERNEL_PAGE_NUMBER - 1) dd 0
	dd 0x00000083
	dd 0x00400083
	dd 0x00800083
	times (1024 - KERNEL_PAGE_NUMBER - 3) dd 0


KERNEL_STACK_SIZE equ 0x400
SECTION .bss
align 4
global kernel_stack
kernel_stack:
	resb KERNEL_STACK_SIZE

global kernel_stack_base
kernel_stack_base:
	dw kernel_stack