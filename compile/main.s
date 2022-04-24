	.file	"main.c"
	.text
	.comm	INT_Software_Value,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	vga_width,4,4
	.comm	vga_height,4,4
	.comm	screen,4,4
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
	cmpl	$920085129, 8(%ebp)
	je	.L2
	movl	$255, %eax
	jmp	.L3
.L2:
	movl	12(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -12(%ebp)
	jmp	.L4
.L6:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$8, %eax
	jne	.L5
	movl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	subl	$12, %esp
	pushl	-16(%ebp)
	call	init_fb@PLT
	addl	$16, %esp
.L5:
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	addl	$7, %eax
	andl	$-8, %eax
	addl	%eax, -12(%ebp)
.L4:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L6
	call	load_gdt@PLT
	subl	$4, %esp
	pushl	$16777215
	pushl	$1
	pushl	$1
	call	fb_setPixel@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$0
	pushl	$16777215
	pushl	$65
	pushl	$1
	pushl	$1
	call	fb_write_cell@PLT
	addl	$32, %esp
	movl	$0, %eax
.L3:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
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
