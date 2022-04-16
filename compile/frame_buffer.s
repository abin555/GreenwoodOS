	.file	"frame_buffer.c"
	.text
	.globl	fb
	.data
	.align 4
	.type	fb, @object
	.size	fb, 4
fb:
	.long	753664
	.globl	fb_cursor
	.bss
	.align 4
	.type	fb_cursor, @object
	.size	fb_cursor, 4
fb_cursor:
	.zero	4
	.globl	FG
	.data
	.type	FG, @object
	.size	FG, 1
FG:
	.byte	15
	.globl	BG
	.bss
	.type	BG, @object
	.size	BG, 1
BG:
	.zero	1
	.comm	INT_Software_Value,4,4
	.text
	.globl	fb_set_color
	.type	fb_set_color, @function
fb_set_color:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %ecx
	movl	12(%ebp), %edx
	movb	%cl, -4(%ebp)
	movb	%dl, -8(%ebp)
	movzbl	-4(%ebp), %edx
	movb	%dl, FG@GOTOFF(%eax)
	movzbl	-8(%ebp), %edx
	movb	%dl, BG@GOTOFF(%eax)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	fb_set_color, .-fb_set_color
	.globl	fb_write_cell
	.type	fb_write_cell, @function
fb_write_cell:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$12, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	12(%ebp), %ebx
	movl	16(%ebp), %ecx
	movl	20(%ebp), %edx
	movb	%bl, -8(%ebp)
	movb	%cl, -12(%ebp)
	movb	%dl, -16(%ebp)
	movl	fb@GOTOFF(%eax), %edx
	movl	8(%ebp), %ecx
	addl	%ecx, %ecx
	addl	%edx, %ecx
	movzbl	-8(%ebp), %edx
	movb	%dl, (%ecx)
	movzbl	-16(%ebp), %edx
	sall	$4, %edx
	movl	%edx, %ebx
	movzbl	-12(%ebp), %edx
	movl	%edx, %ecx
	andl	$15, %ecx
	movl	fb@GOTOFF(%eax), %eax
	movl	8(%ebp), %edx
	addl	%edx, %edx
	addl	$1, %edx
	addl	%edx, %eax
	orl	%ecx, %ebx
	movl	%ebx, %edx
	movb	%dl, (%eax)
	nop
	addl	$12, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	fb_write_cell, .-fb_write_cell
	.globl	printChar
	.type	printChar, @function
printChar:
.LFB2:
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
	call	__x86.get_pc_thunk.dx
	addl	$_GLOBAL_OFFSET_TABLE_, %edx
	movl	16(%ebp), %eax
	movb	%al, -24(%ebp)
	movl	12(%ebp), %ecx
	movl	%ecx, %eax
	sall	$2, %eax
	addl	%ecx, %eax
	sall	$4, %eax
	movl	%eax, %ecx
	movl	8(%ebp), %eax
	addl	%ecx, %eax
	movl	%eax, -8(%ebp)
	movzbl	BG@GOTOFF(%edx), %eax
	movzbl	%al, %ebx
	movzbl	FG@GOTOFF(%edx), %eax
	movzbl	%al, %ecx
	movsbl	-24(%ebp), %edx
	movl	-8(%ebp), %eax
	pushl	%ebx
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	call	fb_write_cell
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	printChar, .-printChar
	.globl	fb_clear
	.type	fb_clear, @function
fb_clear:
.LFB3:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$28, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %ecx
	movl	12(%ebp), %edx
	movl	16(%ebp), %eax
	movb	%cl, -24(%ebp)
	movb	%dl, -28(%ebp)
	movb	%al, -32(%ebp)
	movl	$0, -8(%ebp)
	jmp	.L5
.L6:
	movzbl	-32(%ebp), %ebx
	movzbl	-28(%ebp), %ecx
	movsbl	-24(%ebp), %edx
	movl	-8(%ebp), %eax
	pushl	%ebx
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	call	fb_write_cell
	addl	$16, %esp
	addl	$1, -8(%ebp)
.L5:
	cmpl	$1919, -8(%ebp)
	jle	.L6
	nop
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	fb_clear, .-fb_clear
	.globl	fb_write
	.type	fb_write, @function
fb_write:
.LFB4:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -16(%ebp)
	jmp	.L8
.L9:
	movzbl	BG@GOTOFF(%ebx), %eax
	movzbl	%al, %ecx
	movzbl	FG@GOTOFF(%ebx), %eax
	movzbl	%al, %edx
	movl	8(%ebp), %esi
	movl	-16(%ebp), %eax
	addl	%esi, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	fb_cursor@GOTOFF(%ebx), %esi
	movl	%esi, %edi
	movl	-16(%ebp), %esi
	addl	%edi, %esi
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	pushl	%esi
	call	fb_write_cell
	addl	$16, %esp
	addl	$1, -16(%ebp)
.L8:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jb	.L9
	movl	$0, %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	fb_write, .-fb_write
	.globl	fb_write_start
	.type	fb_write_start, @function
fb_write_start:
.LFB5:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -16(%ebp)
	jmp	.L12
.L13:
	movzbl	BG@GOTOFF(%ebx), %eax
	movzbl	%al, %ecx
	movzbl	FG@GOTOFF(%ebx), %eax
	movzbl	%al, %edx
	movl	-16(%ebp), %esi
	movl	16(%ebp), %eax
	addl	%eax, %esi
	movl	8(%ebp), %eax
	addl	%esi, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	fb_cursor@GOTOFF(%ebx), %esi
	movl	%esi, %edi
	movl	-16(%ebp), %esi
	addl	%edi, %esi
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	pushl	%esi
	call	fb_write_cell
	addl	$16, %esp
	addl	$1, -16(%ebp)
.L12:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jb	.L13
	movl	$0, %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	fb_write_start, .-fb_write_start
	.globl	fb_write_xy
	.type	fb_write_xy, @function
fb_write_xy:
.LFB6:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -16(%ebp)
	jmp	.L16
.L17:
	movzbl	BG@GOTOFF(%ebx), %eax
	movzbl	%al, %edi
	movzbl	FG@GOTOFF(%ebx), %eax
	movzbl	%al, %esi
	movl	-16(%ebp), %edx
	movl	16(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %ecx
	movl	24(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	20(%ebp), %eax
	addl	%eax, %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	pushl	%edi
	pushl	%esi
	pushl	%ecx
	pushl	%eax
	call	fb_write_cell
	addl	$16, %esp
	addl	$1, -16(%ebp)
.L16:
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	.L17
	nop
	nop
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	fb_write_xy, .-fb_write_xy
	.globl	fb_print_buf
	.type	fb_print_buf, @function
fb_print_buf:
.LFB7:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	16(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	$0, -20(%ebp)
	jmp	.L19
.L21:
	addl	$1, -16(%ebp)
	movl	-16(%ebp), %eax
	cmpl	12(%ebp), %eax
	jbe	.L20
	movl	$0, -16(%ebp)
.L20:
	movzbl	BG@GOTOFF(%ebx), %eax
	movzbl	%al, %edi
	movzbl	FG@GOTOFF(%ebx), %eax
	movzbl	%al, %esi
	movl	8(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %ecx
	movl	24(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	20(%ebp), %eax
	addl	%edx, %eax
	pushl	%edi
	pushl	%esi
	pushl	%ecx
	pushl	%eax
	call	fb_write_cell
	addl	$16, %esp
	addl	$1, -20(%ebp)
.L19:
	movl	-20(%ebp), %eax
	cmpl	12(%ebp), %eax
	jb	.L21
	movl	$0, %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	fb_print_buf, .-fb_print_buf
	.globl	fb_move_cursor
	.type	fb_move_cursor, @function
fb_move_cursor:
.LFB8:
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
	movl	8(%ebp), %eax
	movw	%ax, -12(%ebp)
	movzwl	-12(%ebp), %eax
	movl	%eax, fb_cursor@GOTOFF(%ebx)
	subl	$8, %esp
	pushl	$14
	pushl	$980
	call	outb@PLT
	addl	$16, %esp
	movzwl	-12(%ebp), %eax
	shrw	$8, %ax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$981
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$15
	pushl	$980
	call	outb@PLT
	addl	$16, %esp
	movzwl	-12(%ebp), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$981
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
.LFE8:
	.size	fb_move_cursor, .-fb_move_cursor
	.globl	fb_move_cursor_xy
	.type	fb_move_cursor_xy, @function
fb_move_cursor_xy:
.LFB9:
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
	movl	12(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	sall	$4, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, fb_cursor@GOTOFF(%ebx)
	subl	$8, %esp
	pushl	$14
	pushl	$980
	call	outb@PLT
	addl	$16, %esp
	movl	fb_cursor@GOTOFF(%ebx), %eax
	sarl	$8, %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$981
	call	outb@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$15
	pushl	$980
	call	outb@PLT
	addl	$16, %esp
	movl	fb_cursor@GOTOFF(%ebx), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$981
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
.LFE9:
	.size	fb_move_cursor_xy, .-fb_move_cursor_xy
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB10:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE10:
	.section	.text.__x86.get_pc_thunk.dx,"axG",@progbits,__x86.get_pc_thunk.dx,comdat
	.globl	__x86.get_pc_thunk.dx
	.hidden	__x86.get_pc_thunk.dx
	.type	__x86.get_pc_thunk.dx, @function
__x86.get_pc_thunk.dx:
.LFB11:
	.cfi_startproc
	movl	(%esp), %edx
	ret
	.cfi_endproc
.LFE11:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB12:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE12:
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
