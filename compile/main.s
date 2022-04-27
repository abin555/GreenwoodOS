	.file	"main.c"
	.text
	.comm	INT_Software_Value,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.comm	Terminal_Buffer,128,32
	.comm	Terminal_OUT_Buffer,5120,32
	.comm	Terminal_Arguments,128,32
	.comm	SYS_MODE,1,1
	.comm	decode,500,32
	.comm	vga_width,4,4
	.comm	vga_height,4,4
	.comm	screen,4,4
	.globl	flyingDot
	.type	flyingDot, @function
flyingDot:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$52, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$50, -12(%ebp)
	movl	$50, -16(%ebp)
	movl	$3, -20(%ebp)
	movl	$5, -24(%ebp)
	movl	$255, -28(%ebp)
.L25:
	movl	$0, -32(%ebp)
	jmp	.L2
.L19:
	movl	-20(%ebp), %eax
	addl	%eax, -12(%ebp)
	movl	-24(%ebp), %eax
	addl	%eax, -16(%ebp)
	movl	-16(%ebp), %edx
	movl	-12(%ebp), %eax
	subl	$4, %esp
	pushl	$16777215
	pushl	%edx
	pushl	%eax
	call	fb_setPixel@PLT
	addl	$16, %esp
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$10, %eax
	cmpl	%eax, -12(%ebp)
	jge	.L3
	cmpl	$10, -12(%ebp)
	jg	.L4
.L3:
	negl	-20(%ebp)
	cmpl	$0, -20(%ebp)
	jns	.L5
	subl	$1, -20(%ebp)
.L5:
	cmpl	$0, -20(%ebp)
	jle	.L4
	addl	$1, -20(%ebp)
.L4:
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$10, %eax
	cmpl	%eax, -16(%ebp)
	jge	.L6
	cmpl	$10, -16(%ebp)
	jg	.L7
.L6:
	negl	-24(%ebp)
	cmpl	$0, -24(%ebp)
	jns	.L8
	subl	$1, -24(%ebp)
.L8:
	cmpl	$0, -24(%ebp)
	jle	.L7
	addl	$1, -24(%ebp)
.L7:
	cmpl	$50, -20(%ebp)
	jg	.L9
	cmpl	$-50, -20(%ebp)
	jge	.L10
.L9:
	movl	$1, -20(%ebp)
.L10:
	cmpl	$50, -24(%ebp)
	jg	.L11
	cmpl	$-50, -24(%ebp)
	jge	.L12
.L11:
	movl	$1, -24(%ebp)
.L12:
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, -12(%ebp)
	jg	.L13
	cmpl	$0, -12(%ebp)
	jns	.L14
.L13:
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -12(%ebp)
.L14:
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, -16(%ebp)
	jg	.L15
	cmpl	$0, -16(%ebp)
	jns	.L16
.L15:
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -16(%ebp)
.L16:
	movl	$0, -36(%ebp)
	jmp	.L17
.L18:
	addl	$1, -36(%ebp)
.L17:
	cmpl	$65534, -36(%ebp)
	jbe	.L18
	addl	$1, -32(%ebp)
.L2:
	cmpl	$254, -32(%ebp)
	jle	.L19
	movl	$0, -40(%ebp)
	jmp	.L20
.L24:
	movl	$0, -44(%ebp)
	jmp	.L21
.L23:
	movl	-28(%ebp), %eax
	sall	$16, %eax
	andl	$16711680, %eax
	movl	%eax, %edx
	movl	-28(%ebp), %eax
	sall	$8, %eax
	movzwl	%ax, %eax
	orl	%edx, %eax
	orl	-28(%ebp), %eax
	movl	%eax, %ecx
	movl	-44(%ebp), %edx
	movl	-40(%ebp), %eax
	subl	$4, %esp
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	call	fb_setPixel@PLT
	addl	$16, %esp
	subl	$1, -28(%ebp)
	cmpl	$0, -28(%ebp)
	jne	.L22
	movl	$255, -28(%ebp)
.L22:
	addl	$1, -44(%ebp)
.L21:
	cmpl	$499, -44(%ebp)
	jle	.L23
	addl	$1, -40(%ebp)
.L20:
	cmpl	$499, -40(%ebp)
	jle	.L24
	jmp	.L25
	.cfi_endproc
.LFE0:
	.size	flyingDot, .-flyingDot
	.globl	pong
	.type	pong, @function
pong:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -12(%ebp)
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -16(%ebp)
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -20(%ebp)
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -24(%ebp)
	movl	$-10, -28(%ebp)
	movl	$10, -32(%ebp)
.L38:
	subl	$12, %esp
	pushl	$0
	call	fb_clearBackBuffer@PLT
	addl	$16, %esp
	movl	-28(%ebp), %eax
	addl	%eax, -20(%ebp)
	movl	-32(%ebp), %eax
	addl	%eax, -24(%ebp)
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$49, %eax
	cmpl	%eax, -20(%ebp)
	jge	.L27
	cmpl	$99, -20(%ebp)
	jg	.L28
.L27:
	cmpl	$99, -20(%ebp)
	jg	.L29
	movl	-16(%ebp), %eax
	subl	$74, %eax
	cmpl	%eax, -24(%ebp)
	jl	.L30
	movl	-16(%ebp), %eax
	addl	$74, %eax
	cmpl	%eax, -24(%ebp)
	jg	.L30
	negl	-28(%ebp)
	jmp	.L28
.L30:
	movl	fb_width@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -20(%ebp)
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	shrl	%eax
	movl	%eax, -24(%ebp)
	negl	-28(%ebp)
	addl	$1, -28(%ebp)
	addl	$1, -32(%ebp)
	jmp	.L28
.L29:
	negl	-28(%ebp)
.L28:
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$49, %eax
	cmpl	%eax, -24(%ebp)
	jge	.L32
	cmpl	$49, -24(%ebp)
	jg	.L33
.L32:
	negl	-32(%ebp)
.L33:
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	cmpl	%eax, -12(%ebp)
	je	.L34
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	cmpb	$119, %al
	jne	.L35
	cmpl	$150, -16(%ebp)
	jle	.L34
	subl	$25, -16(%ebp)
	jmp	.L34
.L35:
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	cmpb	$115, %al
	jne	.L34
	movl	fb_height@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$150, %eax
	cmpl	%eax, -16(%ebp)
	jge	.L34
	addl	$25, -16(%ebp)
.L34:
	movl	-24(%ebp), %edx
	movl	-20(%ebp), %eax
	pushl	$16777215
	pushl	$10
	pushl	%edx
	pushl	%eax
	call	pixelScaled@PLT
	addl	$16, %esp
	movl	$0, -36(%ebp)
	jmp	.L36
.L37:
	movl	-16(%ebp), %eax
	leal	-75(%eax), %edx
	movl	-36(%ebp), %eax
	addl	%edx, %eax
	subl	$4, %esp
	pushl	$16777215
	pushl	%eax
	pushl	$100
	call	fb_setPixel@PLT
	addl	$16, %esp
	addl	$1, -36(%ebp)
.L36:
	cmpl	$149, -36(%ebp)
	jle	.L37
	call	fb_copyBuffer@PLT
	jmp	.L38
	.cfi_endproc
.LFE1:
	.size	pong, .-pong
	.globl	kmain
	.type	kmain, @function
kmain:
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
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	cmpl	$920085129, 8(%ebp)
	je	.L40
	movl	$255, %eax
	jmp	.L41
.L40:
	movl	12(%ebp), %eax
	addl	$8, %eax
	movl	%eax, -12(%ebp)
	jmp	.L42
.L44:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	$8, %eax
	jne	.L43
	movl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	subl	$12, %esp
	pushl	-16(%ebp)
	call	init_fb@PLT
	addl	$16, %esp
.L43:
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	addl	$7, %eax
	andl	$-8, %eax
	addl	%eax, -12(%ebp)
.L42:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L44
	call	load_gdt@PLT
	call	interrupt_install_idt@PLT
	subl	$8, %esp
	pushl	$0
	pushl	$16777215
	call	fb_set_color@PLT
	addl	$16, %esp
	call	pong
	movl	$0, %eax
.L41:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	kmain, .-kmain
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
