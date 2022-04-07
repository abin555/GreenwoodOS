	.file	"main.c"
	.text
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	kbd_US,128,32
	.comm	keyboard_KEYBUFFER,100,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	prev_Scancode,1,1
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.globl	sum_of_three
	.type	sum_of_three, @function
sum_of_three:
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
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	addl	%eax, %edx
	movl	16(%ebp), %eax
	addl	%edx, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	sum_of_three, .-sum_of_three
	.globl	delay
	.type	delay, @function
delay:
.LFB1:
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
	jmp	.L4
.L7:
	movl	$0, -8(%ebp)
	jmp	.L5
.L6:
	addl	$1, -8(%ebp)
.L5:
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	cmpl	%eax, -8(%ebp)
	jl	.L6
	addl	$1, -4(%ebp)
.L4:
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	cmpl	%eax, -4(%ebp)
	jl	.L7
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	delay, .-delay
	.comm	decode,100,32
	.globl	decodeData
	.type	decodeData, @function
decodeData:
.LFB2:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -8(%ebp)
	jmp	.L9
.L12:
	movl	-8(%ebp), %edx
	movl	8(%ebp), %ebx
	movl	%edx, %ecx
	sarl	%cl, %ebx
	movl	%ebx, %edx
	andl	$1, %edx
	testl	%edx, %edx
	je	.L10
	movl	12(%ebp), %edx
	movl	%edx, %ecx
	subl	-8(%ebp), %ecx
	movl	16(%ebp), %edx
	addl	%edx, %ecx
	movl	decode@GOT(%eax), %edx
	movb	$49, (%edx,%ecx)
	jmp	.L11
.L10:
	movl	12(%ebp), %edx
	movl	%edx, %ecx
	subl	-8(%ebp), %ecx
	movl	16(%ebp), %edx
	addl	%edx, %ecx
	movl	decode@GOT(%eax), %edx
	movb	$48, (%edx,%ecx)
.L11:
	addl	$1, -8(%ebp)
.L9:
	movl	12(%ebp), %edx
	addl	$1, %edx
	cmpl	%edx, -8(%ebp)
	jle	.L12
	nop
	nop
	addl	$16, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	decodeData, .-decodeData
	.globl	quadtoHex
	.type	quadtoHex, @function
quadtoHex:
.LFB3:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$4, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %edx
	movb	%dl, -4(%ebp)
	movsbl	-4(%ebp), %edx
	cmpl	$15, %edx
	ja	.L14
	sall	$2, %edx
	movl	.L16@GOTOFF(%edx,%eax), %edx
	addl	%edx, %eax
	notrack jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L16:
	.long	.L31@GOTOFF
	.long	.L30@GOTOFF
	.long	.L29@GOTOFF
	.long	.L28@GOTOFF
	.long	.L27@GOTOFF
	.long	.L26@GOTOFF
	.long	.L25@GOTOFF
	.long	.L24@GOTOFF
	.long	.L23@GOTOFF
	.long	.L22@GOTOFF
	.long	.L21@GOTOFF
	.long	.L20@GOTOFF
	.long	.L19@GOTOFF
	.long	.L18@GOTOFF
	.long	.L17@GOTOFF
	.long	.L15@GOTOFF
	.text
.L31:
	movl	$48, %eax
	jmp	.L32
.L30:
	movl	$49, %eax
	jmp	.L32
.L29:
	movl	$50, %eax
	jmp	.L32
.L28:
	movl	$51, %eax
	jmp	.L32
.L27:
	movl	$52, %eax
	jmp	.L32
.L26:
	movl	$53, %eax
	jmp	.L32
.L25:
	movl	$54, %eax
	jmp	.L32
.L24:
	movl	$55, %eax
	jmp	.L32
.L23:
	movl	$56, %eax
	jmp	.L32
.L22:
	movl	$57, %eax
	jmp	.L32
.L21:
	movl	$97, %eax
	jmp	.L32
.L20:
	movl	$98, %eax
	jmp	.L32
.L19:
	movl	$99, %eax
	jmp	.L32
.L18:
	movl	$100, %eax
	jmp	.L32
.L17:
	movl	$101, %eax
	jmp	.L32
.L15:
	movl	$102, %eax
	jmp	.L32
.L14:
	movl	$120, %eax
.L32:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	quadtoHex, .-quadtoHex
	.globl	decodeHex
	.type	decodeHex, @function
decodeHex:
.LFB4:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -12(%ebp)
	jmp	.L34
.L35:
	movl	-12(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%eax, %ecx
	sarl	%cl, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	andl	$15, %eax
	movl	%eax, %edx
	movl	12(%ebp), %eax
	subl	-12(%ebp), %eax
	movl	%eax, %ecx
	movl	16(%ebp), %eax
	leal	(%ecx,%eax), %esi
	pushl	%edx
	call	quadtoHex
	addl	$4, %esp
	movl	decode@GOT(%ebx), %edx
	movb	%al, (%edx,%esi)
	addl	$4, -12(%ebp)
.L34:
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	.L35
	nop
	nop
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	decodeHex, .-decodeHex
	.globl	Phrase
	.data
	.align 4
	.type	Phrase, @object
	.size	Phrase, 17
Phrase:
	.string	"Congrats Pharos!"
	.globl	String
	.align 32
	.type	String, @object
	.size	String, 33
String:
	.string	"Okay now, lets see if this works"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	endbr32
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	subl	$16, %esp
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	call	interrupt_install_idt@PLT
	subl	$4, %esp
	pushl	$0
	pushl	$15
	pushl	$32
	call	fb_clear@PLT
	addl	$16, %esp
	movl	$10, -12(%ebp)
	jmp	.L37
.L38:
	movl	-12(%ebp), %eax
	pushl	$0
	pushl	$15
	pushl	$66
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	addl	$1, -12(%ebp)
.L37:
	cmpl	$19, -12(%ebp)
	jle	.L38
	subl	$12, %esp
	pushl	$80
	call	fb_move_cursor@PLT
	addl	$16, %esp
	pushl	$0
	pushl	$15
	pushl	$88
	pushl	$0
	call	fb_write_cell@PLT
	addl	$16, %esp
	pushl	$0
	pushl	$15
	pushl	$89
	pushl	$1
	call	fb_write_cell@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$16
	leal	Phrase@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fb_write@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$160
	call	fb_move_cursor@PLT
	addl	$16, %esp
	subl	$4, %esp
	pushl	$66
	pushl	$15
	pushl	$20
	call	printChar@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$32
	leal	String@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fb_write@PLT
	addl	$16, %esp
.L39:
	subl	$12, %esp
	pushl	$240
	call	fb_move_cursor@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$100
	movl	keyboard_KEYBUFFER@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$400
	call	fb_move_cursor@PLT
	addl	$16, %esp
	movl	prev_Scancode@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	subl	$4, %esp
	pushl	$0
	pushl	$8
	pushl	%eax
	call	decodeData
	addl	$16, %esp
	subl	$8, %esp
	pushl	$8
	movl	decode@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write@PLT
	addl	$16, %esp
	movl	prev_Scancode@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	subl	$4, %esp
	pushl	$20
	pushl	$8
	pushl	%eax
	call	decodeHex
	addl	$16, %esp
	subl	$4, %esp
	pushl	$20
	pushl	$2
	movl	decode@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_start@PLT
	addl	$16, %esp
	jmp	.L39
	.cfi_endproc
.LFE5:
	.size	main, .-main
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
