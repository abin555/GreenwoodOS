	.file	"main.c"
	.text
	.comm	INT_Software_Value,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	SYS_MODE,1,1
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.comm	FG,1,1
	.comm	BG,1,1
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.comm	Terminal_Buffer,80,32
	.comm	Terminal_OUT_Buffer,3200,32
	.comm	Terminal_Arguments,80,32
	.comm	decode,500,32
	.globl	kmain
	.type	kmain, @function
kmain:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	call	load_gdt@PLT
	call	interrupt_install_idt@PLT
	subl	$4, %esp
	pushl	$0
	pushl	$15
	pushl	$32
	call	fb_clear@PLT
	addl	$16, %esp
	movl	$655365, -12(%ebp)
	movl	-12(%ebp), %eax
	movb	$15, (%eax)
	movl	8(%ebp), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$2
	pushl	$0
	pushl	$1
	pushl	$8
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	12(%ebp), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$3
	pushl	$0
	pushl	$1
	pushl	$8
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
.L6:
	movl	SYS_MODE@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	cmpl	$1, %eax
	je	.L2
	cmpl	$4, %eax
	je	.L3
	jmp	.L7
.L2:
	call	terminal_handler@PLT
	jmp	.L5
.L3:
	call	KYBRD_DEBUG_DISPLAY@PLT
	jmp	.L5
.L7:
	call	KYBRD_DEBUG_DISPLAY@PLT
.L5:
	jmp	.L6
	.cfi_endproc
.LFE0:
	.size	kmain, .-kmain
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB1:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE1:
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
