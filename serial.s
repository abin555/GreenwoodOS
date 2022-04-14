	.file	"serial.c"
	.text
	.comm	INT_Software_Value,1,1
	.globl	serial_configure_baud_rate
	.type	serial_configure_baud_rate, @function
serial_configure_baud_rate:
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
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movw	%dx, -12(%ebp)
	movw	%ax, -16(%ebp)
	movzwl	-12(%ebp), %eax
	addl	$3, %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	$128
	pushl	%eax
	call	outb@PLT
	addl	$16, %esp
	movzwl	-16(%ebp), %eax
	shrw	$8, %ax
	movzbl	%al, %edx
	movzwl	-12(%ebp), %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	outb@PLT
	addl	$16, %esp
	movzwl	-16(%ebp), %eax
	movzbl	%al, %edx
	movzwl	-12(%ebp), %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	outb@PLT
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	serial_configure_baud_rate, .-serial_configure_baud_rate
	.globl	serial_configure_line
	.type	serial_configure_line, @function
serial_configure_line:
.LFB1:
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
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %edx
	movw	%dx, -12(%ebp)
	movzwl	-12(%ebp), %edx
	addl	$3, %edx
	movzwl	%dx, %edx
	subl	$8, %esp
	pushl	$3
	pushl	%edx
	movl	%eax, %ebx
	call	outb@PLT
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	serial_configure_line, .-serial_configure_line
	.globl	serial_is_transmit_fifo_empty
	.type	serial_is_transmit_fifo_empty, @function
serial_is_transmit_fifo_empty:
.LFB2:
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
	movl	8(%ebp), %edx
	addl	$5, %edx
	movzwl	%dx, %edx
	subl	$12, %esp
	pushl	%edx
	movl	%eax, %ebx
	call	inb@PLT
	addl	$16, %esp
	movzbl	%al, %eax
	andl	$32, %eax
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	serial_is_transmit_fifo_empty, .-serial_is_transmit_fifo_empty
	.globl	serial_write
	.type	serial_write, @function
serial_write:
.LFB3:
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
	movl	12(%ebp), %eax
	movb	%al, -12(%ebp)
	nop
.L6:
	subl	$12, %esp
	pushl	8(%ebp)
	call	serial_is_transmit_fifo_empty
	addl	$16, %esp
	testl	%eax, %eax
	je	.L6
	movzbl	-12(%ebp), %eax
	movzbl	%al, %edx
	movl	8(%ebp), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	outb@PLT
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	serial_write, .-serial_write
	.globl	init_serial
	.type	init_serial, @function
init_serial:
.LFB4:
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
	pushl	$0
	pushl	$1017
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$128
	pushl	$1019
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$3
	pushl	$1016
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$1017
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$3
	pushl	$1019
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$199
	pushl	$1018
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$11
	pushl	$1020
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$30
	pushl	$1020
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$174
	pushl	$1016
	call	outb@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$1016
	call	inb@PLT
	addl	$16, %esp
	cmpb	$-82, %al
	je	.L8
	movl	$1, %eax
	jmp	.L9
.L8:
	subl	$8, %esp
	pushl	$15
	pushl	$1020
	call	outb@PLT
	addl	$16, %esp
	movl	$0, %eax
.L9:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	init_serial, .-init_serial
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB5:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE5:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB6:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE6:
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
