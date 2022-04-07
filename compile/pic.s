	.file	"pic.c"
	.text
	.globl	pic_acknowledge
	.type	pic_acknowledge, @function
pic_acknowledge:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	cmpl	$31, 8(%ebp)
	jbe	.L6
	cmpl	$47, 8(%ebp)
	ja	.L6
	cmpl	$39, 8(%ebp)
	ja	.L5
	subl	$8, %esp
	pushl	$32
	pushl	$32
	movl	%eax, %ebx
	call	outb@PLT
	addl	$16, %esp
	jmp	.L1
.L5:
	subl	$8, %esp
	pushl	$32
	pushl	$160
	movl	%eax, %ebx
	call	outb@PLT
	addl	$16, %esp
	jmp	.L1
.L6:
	nop
.L1:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	pic_acknowledge, .-pic_acknowledge
	.globl	pic_remap
	.type	pic_remap, @function
pic_remap:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$8, %esp
	pushl	$17
	pushl	$32
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$17
	pushl	$160
	call	outb@PLT
	addl	$16, %esp
	movl	8(%ebp), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$33
	call	outb@PLT
	addl	$16, %esp
	movl	12(%ebp), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$161
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$4
	pushl	$33
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$2
	pushl	$161
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$33
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$161
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$253
	pushl	$33
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$255
	pushl	$161
	call	outb@PLT
	addl	$16, %esp
#APP
# 31 "pic.c" 1
	sti
# 0 "" 2
#NO_APP
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	pic_remap, .-pic_remap
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB2:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE2:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB3:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE3:
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
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
