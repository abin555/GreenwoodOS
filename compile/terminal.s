	.file	"terminal.c"
	.text
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.comm	INT_Software_Value,4,4
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
	.comm	Terminal_Buffer,80,32
	.comm	Terminal_OUT_Buffer,3200,32
	.comm	Terminal_Arguments,80,32
	.globl	Terminal_OUT_pointer
	.bss
	.align 4
	.type	Terminal_OUT_pointer, @object
	.size	Terminal_OUT_pointer, 4
Terminal_OUT_pointer:
	.zero	4
	.globl	Terminal_Buffer_Pointer
	.align 2
	.type	Terminal_Buffer_Pointer, @object
	.size	Terminal_Buffer_Pointer, 2
Terminal_Buffer_Pointer:
	.zero	2
	.globl	previousASCII_pointer
	.align 4
	.type	previousASCII_pointer, @object
	.size	previousASCII_pointer, 4
previousASCII_pointer:
	.zero	4
	.globl	previousKEY_pointer
	.align 4
	.type	previousKEY_pointer, @object
	.size	previousKEY_pointer, 4
previousKEY_pointer:
	.zero	4
	.globl	Terminal_Y
	.data
	.align 4
	.type	Terminal_Y, @object
	.size	Terminal_Y, 4
Terminal_Y:
	.long	24
	.text
	.globl	terminal_renderer
	.type	terminal_renderer, @function
terminal_renderer:
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
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$12, %esp
	pushl	$0
	call	fb_clear@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$1
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	subl	$4, %esp
	pushl	$62
	pushl	%eax
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$12, %esp
	pushl	%edx
	pushl	$1
	pushl	%eax
	pushl	$80
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	terminal_renderer, .-terminal_renderer
	.globl	terminal_console
	.type	terminal_console, @function
terminal_console:
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
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L3
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %ecx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %edx
	movzbl	(%edx,%ecx), %ecx
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
	jmp	.L4
.L3:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %edx
	movzbl	254(%edx), %ecx
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
.L4:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movzbl	(%edx,%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Y@GOTOFF(%ebx), %edx
	movl	%edx, %ecx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %edx
	movzwl	%dx, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	addl	$2, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	subl	$8, %esp
	pushl	$0
	pushl	$65280
	call	fb_set_color@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	subl	$4, %esp
	pushl	$62
	pushl	%eax
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$16777215
	call	fb_set_color@PLT
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
	.size	terminal_console, .-terminal_console
	.globl	terminal_output
	.type	terminal_output, @function
terminal_output:
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
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	$0
	pushl	$16711680
	pushl	$45
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	movl	16(%ebp), %edx
	movl	16(%ebp), %eax
	subl	12(%ebp), %eax
	subl	$12, %esp
	pushl	$0
	pushl	%edx
	pushl	12(%ebp)
	pushl	%eax
	pushl	8(%ebp)
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	terminal_output, .-terminal_output
	.globl	terminal_compare
	.type	terminal_compare, @function
terminal_compare:
.LFB3:
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
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	movl	Terminal_Buffer@GOT(%eax), %edx
	pushl	%edx
	movl	%eax, %ebx
	call	STR_Compare@PLT
	addl	$16, %esp
	cmpl	%eax, 20(%ebp)
	jne	.L7
	movl	$1, %eax
	jmp	.L8
.L7:
	movl	$0, %eax
.L8:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	terminal_compare, .-terminal_compare
	.section	.rodata
.LC0:
	.string	"print"
.LC1:
	.string	"peek"
.LC2:
	.string	"poke"
.LC3:
	.string	"load"
.LC4:
	.string	"exec"
.LC5:
	.string	"wipe"
.LC6:
	.string	"check"
.LC7:
	.string	"swap_PROG"
.LC8:
	.string	"set_PROG"
	.text
	.globl	terminal_interpret
	.type	terminal_interpret, @function
terminal_interpret:
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
	subl	$32, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	$0, -12(%ebp)
	movl	$0, -16(%ebp)
	jmp	.L10
.L13:
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	cmpb	$32, %al
	je	.L11
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L26
.L11:
	movl	-16(%ebp), %eax
	movl	%eax, %ecx
	movl	Terminal_Arguments@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movb	%cl, (%eax)
	addl	$1, -12(%ebp)
.L26:
	addl	$1, -16(%ebp)
.L10:
	cmpl	$79, -16(%ebp)
	jle	.L13
	movl	-12(%ebp), %eax
	leal	1(%eax), %edx
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movb	$-1, (%eax,%edx)
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$5
	pushl	%eax
	pushl	$0
	leal	.LC0@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L14
	subl	$4, %esp
	pushl	$80
	pushl	$0
	pushl	$79
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	$0
	pushl	$16711680
	pushl	$45
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movl	%eax, %ecx
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	leal	1(%eax), %edx
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	$80, %esi
	subl	%eax, %esi
	movl	%esi, %eax
	subl	$12, %esp
	pushl	$0
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
.L14:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$4
	pushl	%eax
	pushl	$0
	leal	.LC1@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L15
	subl	$4, %esp
	pushl	$77
	pushl	$0
	pushl	$79
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	$0
	pushl	$16711680
	pushl	$45
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	1(%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Arguments@GOT(%ebx), %edx
	movzbl	(%edx), %edx
	movsbl	%dl, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	encodeHex@PLT
	addl	$16, %esp
	movl	%eax, -24(%ebp)
	subl	$12, %esp
	pushl	-24(%ebp)
	call	ReadMem@PLT
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	movl	-28(%ebp), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	subl	$12, %esp
	pushl	$0
	pushl	%eax
	pushl	$0
	pushl	$9
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
.L15:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$4
	pushl	%eax
	pushl	$0
	leal	.LC2@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L16
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	$0
	pushl	$16711680
	pushl	$45
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	1(%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Arguments@GOT(%ebx), %edx
	movzbl	(%edx), %edx
	movsbl	%dl, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	encodeHex@PLT
	addl	$16, %esp
	movl	%eax, -32(%ebp)
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	2(%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Arguments@GOT(%ebx), %edx
	movzbl	1(%edx), %edx
	movsbl	%dl, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	encodeHex@PLT
	addl	$16, %esp
	movl	%eax, -36(%ebp)
	subl	$8, %esp
	pushl	-36(%ebp)
	pushl	-32(%ebp)
	call	WriteMem@PLT
	addl	$16, %esp
	movl	-32(%ebp), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	movl	-36(%ebp), %eax
	pushl	$9
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	movl	STR_edit@GOT(%ebx), %eax
	movb	$32, 9(%eax)
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	subl	$12, %esp
	pushl	$0
	pushl	%eax
	pushl	$0
	pushl	$18
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
.L16:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$4
	pushl	%eax
	pushl	$0
	leal	.LC3@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L17
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	$440
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	$5, %eax
	subl	$8, %esp
	pushl	$61371
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	$10, %eax
	subl	$8, %esp
	pushl	$21689
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	$15, %eax
	subl	$8, %esp
	pushl	$32973
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	$17, %eax
	subl	$8, %esp
	pushl	$195
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
.L17:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$4
	pushl	%eax
	pushl	$0
	leal	.LC4@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L18
	call	PROGA@PLT
.L18:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$4
	pushl	%eax
	pushl	$0
	leal	.LC5@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L19
	movl	$0, -20(%ebp)
	jmp	.L20
.L21:
	movl	-20(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	subl	$8, %esp
	pushl	$0
	pushl	%eax
	call	WriteMem@PLT
	addl	$16, %esp
	addl	$1, -20(%ebp)
.L20:
	cmpl	$49, -20(%ebp)
	jle	.L21
.L19:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$5
	pushl	%eax
	pushl	$0
	leal	.LC6@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L22
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	subl	$12, %esp
	pushl	$0
	pushl	%eax
	pushl	$0
	pushl	$9
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	$0
	pushl	$16711680
	pushl	$45
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
.L22:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$9
	pushl	%eax
	pushl	$0
	leal	.LC7@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L23
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	cmpl	$16777216, %eax
	jne	.L24
	movl	externalProgram@GOT(%ebx), %eax
	movl	$33554432, (%eax)
	jmp	.L23
.L24:
	movl	externalProgram@GOT(%ebx), %eax
	movl	(%eax), %eax
	cmpl	$33554432, %eax
	jne	.L23
	movl	externalProgram@GOT(%ebx), %eax
	movl	$16777216, (%eax)
.L23:
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	pushl	$8
	pushl	%eax
	pushl	$0
	leal	.LC8@GOTOFF(%ebx), %eax
	pushl	%eax
	call	terminal_compare
	addl	$16, %esp
	testl	%eax, %eax
	je	.L27
	movl	Terminal_Arguments@GOT(%ebx), %eax
	movzbl	1(%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Arguments@GOT(%ebx), %edx
	movzbl	(%edx), %edx
	movsbl	%dl, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	encodeHex@PLT
	addl	$16, %esp
	movl	%eax, -40(%ebp)
	movl	-40(%ebp), %edx
	movl	externalProgram@GOT(%ebx), %eax
	movl	%edx, (%eax)
.L27:
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
	.size	terminal_interpret, .-terminal_interpret
	.globl	terminal_enter
	.type	terminal_enter, @function
terminal_enter:
.LFB5:
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
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	%eax
	pushl	$80
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	pushl	%eax
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	STR_INSERT@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	subl	$12, %esp
	pushl	$0
	pushl	%edx
	pushl	%eax
	pushl	$80
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %ecx
	movl	$80, %eax
	cltd
	idivl	%ecx
	movl	%edx, %eax
	leal	80(%eax), %edx
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	%edx, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
	movw	$0, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	call	terminal_interpret
	movl	$0, -12(%ebp)
	jmp	.L29
.L30:
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	$1, %eax
	subl	$4, %esp
	pushl	$32
	pushl	%edx
	pushl	%eax
	call	printChar@PLT
	addl	$16, %esp
	addl	$1, -12(%ebp)
.L29:
	cmpl	$79, -12(%ebp)
	jle	.L30
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$1
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$65280
	call	fb_set_color@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	subl	$4, %esp
	pushl	$62
	pushl	%eax
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$16777215
	call	fb_set_color@PLT
	addl	$16, %esp
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	leal	-1(%eax), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	imull	%eax, %edx
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	jge	.L32
	movl	$0, Terminal_OUT_pointer@GOTOFF(%ebx)
	subl	$12, %esp
	pushl	$0
	call	fb_clear@PLT
	addl	$16, %esp
.L32:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	terminal_enter, .-terminal_enter
	.globl	terminal_handler
	.type	terminal_handler, @function
terminal_handler:
.LFB6:
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
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	previousASCII_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L34
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$79, %ax
	ja	.L34
	call	terminal_console
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, previousASCII_pointer@GOTOFF(%ebx)
	jmp	.L49
.L34:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	previousKEY_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L49
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	keyboard_KEYBUFFER@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	movzbl	%al, %eax
	cmpl	$205, %eax
	je	.L36
	cmpl	$205, %eax
	jg	.L37
	cmpl	$203, %eax
	je	.L38
	cmpl	$203, %eax
	jg	.L37
	cmpl	$200, %eax
	je	.L39
	cmpl	$200, %eax
	jg	.L37
	cmpl	$14, %eax
	je	.L40
	cmpl	$156, %eax
	je	.L41
	jmp	.L37
.L40:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L50
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	leal	-1(%eax), %edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	movb	$0, (%eax,%edx)
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$4, %esp
	pushl	$32
	pushl	%edx
	pushl	%eax
	call	printChar@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L50
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	subl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	jmp	.L50
.L41:
	call	terminal_enter
	jmp	.L37
.L38:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L51
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	subl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	jmp	.L51
.L36:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$79, %ax
	ja	.L52
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	jmp	.L52
.L39:
	movl	$0, -12(%ebp)
	jmp	.L45
.L48:
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %edx
	movl	fb_terminal_w@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	%eax, %edx
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	movl	Terminal_Buffer@GOT(%ebx), %ecx
	movl	-12(%ebp), %edx
	addl	%ecx, %edx
	movb	%al, (%edx)
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	Terminal_Y@GOTOFF(%ebx), %edx
	movl	%edx, %ecx
	movl	-12(%ebp), %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	%ecx
	pushl	%edx
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L46
	movl	Terminal_Y@GOTOFF(%ebx), %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	addl	$1, %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	nop
	jmp	.L37
.L46:
	addl	$1, -12(%ebp)
.L45:
	cmpl	$79, -12(%ebp)
	jle	.L48
	jmp	.L37
.L50:
	nop
	jmp	.L37
.L51:
	nop
	jmp	.L37
.L52:
	nop
.L37:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, previousKEY_pointer@GOTOFF(%ebx)
.L49:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	terminal_handler, .-terminal_handler
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB7:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE7:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB8:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE8:
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
