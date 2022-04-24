	.file	"framebuffer.c"
	.text
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.section	.rodata
	.align 32
	.type	FONT, @object
	.size	FONT, 1024
FONT:
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\030<<\030\030"
	.string	"\030"
	.string	"66"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"66\1776\17766"
	.string	"\f>\003\0360\037\f"
	.string	""
	.string	"c3\030\ffc"
	.string	"\0346\034n;3n"
	.string	"\006\006\003"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\030\f\006\006\006\f\030"
	.string	"\006\f\030\030\030\f\006"
	.string	""
	.string	"f<\377<f"
	.string	""
	.string	""
	.string	"\f\f?\f\f"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.ascii	"\f\f\006"
	.string	""
	.string	""
	.string	""
	.string	"?"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\f\f"
	.string	"`0\030\f\006\003\001"
	.string	">cs{og>"
	.string	"\f\016\f\f\f\f?"
	.string	"\03630\034\0063?"
	.string	"\03630\03403\036"
	.string	"8<63\1770x"
	.string	"?\003\037003\036"
	.string	"\034\006\003\03733\036"
	.string	"?30\030\f\f\f"
	.string	"\03633\03633\036"
	.string	"\03633>0\030\016"
	.string	""
	.string	"\f\f"
	.string	""
	.string	"\f\f"
	.string	""
	.string	"\f\f"
	.string	""
	.ascii	"\f\f\006"
	.string	"\030\f\006\003\006\f\030"
	.string	""
	.string	""
	.string	"?"
	.string	""
	.string	"?"
	.string	""
	.string	"\006\f\0300\030\f\006"
	.string	"\03630\030\f"
	.string	"\f"
	.string	">c{{{\003\036"
	.string	"\f\03633?33"
	.string	"?ff>ff?"
	.string	"<f\003\003\003f<"
	.string	"\0376fff6\037"
	.string	"\177F\026\036\026F\177"
	.string	"\177F\026\036\026\006\017"
	.string	"<f\003\003sf|"
	.string	"333?333"
	.string	"\036\f\f\f\f\f\036"
	.string	"x00033\036"
	.string	"gf6\0366fg"
	.string	"\017\006\006\006Ff\177"
	.string	"cw\177\177kcc"
	.string	"cgo{scc"
	.string	"\0346ccc6\034"
	.string	"?ff>\006\006\017"
	.string	"\036333;\0368"
	.string	"?ff>6fg"
	.string	"\0363\007\01683\036"
	.string	"?-\f\f\f\f\036"
	.string	"333333?"
	.string	"33333\036\f"
	.string	"ccck\177wc"
	.string	"cc6\034\0346c"
	.string	"333\036\f\f\036"
	.string	"\177c1\030Lf\177"
	.string	"\036\006\006\006\006\006\036"
	.string	"\003\006\f\0300`@"
	.string	"\036\030\030\030\030\030\036"
	.string	"\b\0346c"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.ascii	"\377"
	.string	"\f\f\030"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\0360>3n"
	.string	"\007\006\006>ff;"
	.string	""
	.string	""
	.string	"\0363\0033\036"
	.string	"800>33n"
	.string	""
	.string	""
	.string	"\0363?\003\036"
	.string	"\0346\006\017\006\006\017"
	.string	""
	.string	""
	.ascii	"n33>0\037"
	.string	"\007\0066nffg"
	.string	"\f"
	.string	"\016\f\f\f\036"
	.string	"0"
	.ascii	"00033\036"
	.string	"\007\006f6\0366g"
	.string	"\016\f\f\f\f\f\036"
	.string	""
	.string	""
	.string	"3\177\177kc"
	.string	""
	.string	""
	.string	"\0373333"
	.string	""
	.string	""
	.string	"\036333\036"
	.string	""
	.string	""
	.ascii	";ff>\006\017"
	.string	""
	.string	""
	.ascii	"n33>0x"
	.string	""
	.string	""
	.string	";nf\006\017"
	.string	""
	.string	""
	.string	">\003\0360\037"
	.string	"\b\f>\f\f,\030"
	.string	""
	.string	""
	.string	"3333n"
	.string	""
	.string	""
	.string	"333\036\f"
	.string	""
	.string	""
	.string	"ck\177\1776"
	.string	""
	.string	""
	.string	"c6\0346c"
	.string	""
	.string	""
	.ascii	"333>0\037"
	.string	""
	.string	""
	.string	"?\031\f&?"
	.string	"8\f\f\007\f\f8"
	.string	"\030\030\030"
	.string	"\030\030\030"
	.string	"\007\f\f8\f\f\007"
	.string	"n;"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.globl	fb_cursor
	.bss
	.align 4
	.type	fb_cursor, @object
	.size	fb_cursor, 4
fb_cursor:
	.zero	4
	.text
	.globl	fb_setPixel
	.type	fb_setPixel, @function
fb_setPixel:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	fb_width@GOT(%eax), %edx
	movl	(%edx), %edx
	cmpl	%edx, 8(%ebp)
	jnb	.L5
	movl	fb_height@GOT(%eax), %edx
	movl	(%edx), %edx
	cmpl	%edx, 12(%ebp)
	jnb	.L5
	movl	fb@GOT(%eax), %edx
	movl	(%edx), %edx
	movl	fb_width@GOT(%eax), %eax
	movl	(%eax), %eax
	imull	12(%ebp), %eax
	movl	%eax, %ecx
	movl	8(%ebp), %eax
	addl	%ecx, %eax
	sall	$2, %eax
	addl	%eax, %edx
	movl	16(%ebp), %eax
	movl	%eax, (%edx)
	jmp	.L1
.L5:
	nop
.L1:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	fb_setPixel, .-fb_setPixel
	.globl	init_fb
	.type	init_fb, @function
init_fb:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %edx
	movl	8(%edx), %ecx
	movl	12(%edx), %ebx
	movl	%ecx, %edx
	movl	%edx, %ecx
	movl	fb@GOT(%eax), %edx
	movl	%ecx, (%edx)
	movl	8(%ebp), %edx
	movl	24(%edx), %ecx
	movl	fb_height@GOT(%eax), %edx
	movl	%ecx, (%edx)
	movl	8(%ebp), %edx
	movl	20(%edx), %ecx
	movl	fb_width@GOT(%eax), %edx
	movl	%ecx, (%edx)
	movl	fb_width@GOT(%eax), %edx
	movl	(%edx), %edx
	shrl	$3, %edx
	movl	%edx, %ecx
	movl	fb_terminal_w@GOT(%eax), %edx
	movl	%ecx, (%edx)
	movl	fb_height@GOT(%eax), %edx
	movl	(%edx), %edx
	shrl	$3, %edx
	movl	fb_terminal_h@GOT(%eax), %eax
	movl	%edx, (%eax)
	nop
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	init_fb, .-init_fb
	.globl	fb_write_cell
	.type	fb_write_cell, @function
fb_write_cell:
.LFB2:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	12(%ebp), %eax
	movb	%al, -28(%ebp)
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	movl	8(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	movl	%edx, %eax
	sall	$3, %eax
	movl	%eax, -20(%ebp)
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, %ecx
	movl	8(%ebp), %eax
	movl	$0, %edx
	divl	%ecx
	sall	$3, %eax
	movl	%eax, -24(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L8
.L13:
	movl	$0, -16(%ebp)
	jmp	.L9
.L12:
	movsbl	-28(%ebp), %edx
	leal	FONT@GOTOFF(%ebx), %eax
	sall	$3, %edx
	addl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	movzbl	%al, %edx
	movl	-16(%ebp), %eax
	movl	%eax, %ecx
	sarl	%cl, %edx
	movl	%edx, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L10
	movl	16(%ebp), %eax
	jmp	.L11
.L10:
	movl	20(%ebp), %eax
.L11:
	movl	fb@GOT(%ebx), %edx
	movl	(%edx), %ecx
	movl	-12(%ebp), %esi
	movl	-24(%ebp), %edx
	addl	%edx, %esi
	movl	fb_width@GOT(%ebx), %edx
	movl	(%edx), %edx
	imull	%edx, %esi
	movl	-20(%ebp), %edx
	addl	%edx, %esi
	movl	-16(%ebp), %edx
	addl	%esi, %edx
	sall	$2, %edx
	addl	%ecx, %edx
	movl	%eax, (%edx)
	addl	$1, -16(%ebp)
.L9:
	cmpl	$7, -16(%ebp)
	jle	.L12
	addl	$1, -12(%ebp)
.L8:
	cmpl	$7, -12(%ebp)
	jle	.L13
	nop
	nop
	addl	$20, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	fb_write_cell, .-fb_write_cell
	.globl	printChar
	.type	printChar, @function
printChar:
.LFB3:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	16(%ebp), %edx
	movb	%dl, -28(%ebp)
	movl	$0, -12(%ebp)
	jmp	.L15
.L20:
	movl	$0, -16(%ebp)
	jmp	.L16
.L19:
	movsbl	-28(%ebp), %ecx
	leal	FONT@GOTOFF(%eax), %edx
	sall	$3, %ecx
	addl	%edx, %ecx
	movl	-12(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%edx), %edx
	movzbl	%dl, %ebx
	movl	-16(%ebp), %edx
	movl	%edx, %ecx
	sarl	%cl, %ebx
	movl	%ebx, %edx
	andl	$1, %edx
	testl	%edx, %edx
	je	.L17
	movl	FG@GOT(%eax), %edx
	movl	(%edx), %edx
	jmp	.L18
.L17:
	movl	BG@GOT(%eax), %edx
	movl	(%edx), %edx
.L18:
	movl	fb@GOT(%eax), %ecx
	movl	(%ecx), %ebx
	movl	-12(%ebp), %esi
	movl	12(%ebp), %ecx
	addl	%ecx, %esi
	movl	fb_width@GOT(%eax), %ecx
	movl	(%ecx), %ecx
	imull	%ecx, %esi
	movl	8(%ebp), %ecx
	addl	%ecx, %esi
	movl	-16(%ebp), %ecx
	addl	%esi, %ecx
	sall	$2, %ecx
	addl	%ebx, %ecx
	movl	%edx, (%ecx)
	addl	$1, -16(%ebp)
.L16:
	cmpl	$7, -16(%ebp)
	jle	.L19
	addl	$1, -12(%ebp)
.L15:
	cmpl	$7, -12(%ebp)
	jle	.L20
	nop
	nop
	addl	$20, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	printChar, .-printChar
	.globl	fb_set_color
	.type	fb_set_color, @function
fb_set_color:
.LFB4:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	FG@GOT(%eax), %edx
	movl	8(%ebp), %ecx
	movl	%ecx, (%edx)
	movl	BG@GOT(%eax), %eax
	movl	12(%ebp), %edx
	movl	%edx, (%eax)
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	fb_set_color, .-fb_set_color
	.globl	fb_clear
	.type	fb_clear, @function
fb_clear:
.LFB5:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -4(%ebp)
	jmp	.L23
.L24:
	movl	fb@GOT(%eax), %edx
	movl	(%edx), %edx
	movl	-4(%ebp), %ecx
	sall	$2, %ecx
	addl	%edx, %ecx
	movl	8(%ebp), %edx
	movl	%edx, (%ecx)
	addl	$1, -4(%ebp)
.L23:
	movl	fb_terminal_h@GOT(%eax), %edx
	movl	(%edx), %ecx
	movl	fb_terminal_w@GOT(%eax), %edx
	movl	(%edx), %edx
	imull	%ecx, %edx
	cmpl	%edx, -4(%ebp)
	jl	.L24
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	fb_clear, .-fb_clear
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB6:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE6:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB7:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE7:
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
