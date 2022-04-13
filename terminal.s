	.file	"terminal.c"
	.text
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.comm	INT_Software_Value,1,1
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,100,32
	.comm	keyboard_ASCIIBuffer,100,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.comm	Terminal_Buffer,70,32
	.comm	Terminal_OUT_Buffer,2800,32
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
	subl	$4, %esp
	pushl	$0
	pushl	$15
	pushl	$32
	call	fb_clear@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$20
	pushl	$1
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	subl	$4, %esp
	pushl	$62
	pushl	$20
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$12, %esp
	pushl	$20
	pushl	$1
	pushl	%eax
	pushl	$70
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
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	keyboard_KEYBUFFER@GOT(%ebx), %edx
	movzbl	(%edx,%eax), %eax
	movzbl	%al, %eax
	pushl	$0
	pushl	$9
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeData@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$21
	pushl	$1
	pushl	$0
	pushl	$9
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %ecx
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %edx
	movzbl	(%edx,%ecx), %ecx
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movzbl	(%edx,%eax), %eax
	movsbl	%al, %eax
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %edx
	movzwl	%dx, %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	$20
	pushl	%edx
	call	printChar@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	addl	$2, %eax
	subl	$8, %esp
	pushl	$20
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	subl	$4, %esp
	pushl	$62
	pushl	$20
	pushl	$0
	call	printChar@PLT
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
	.globl	terminal_enter
	.type	terminal_enter, @function
terminal_enter:
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
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	pushl	%eax
	pushl	$70
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	pushl	%eax
	movl	Terminal_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	STR_INSERT@PLT
	addl	$16, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	subl	$12, %esp
	pushl	$0
	pushl	$0
	pushl	%eax
	pushl	$1330
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$80, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
	movw	$0, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	movl	$0, -12(%ebp)
	jmp	.L4
.L5:
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
	movl	-12(%ebp), %eax
	addl	$1, %eax
	subl	$4, %esp
	pushl	$32
	pushl	$20
	pushl	%eax
	call	printChar@PLT
	addl	$16, %esp
	addl	$1, -12(%ebp)
.L4:
	cmpl	$69, -12(%ebp)
	jle	.L5
	subl	$8, %esp
	pushl	$20
	pushl	$1
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	subl	$4, %esp
	pushl	$62
	pushl	$20
	pushl	$0
	call	printChar@PLT
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
	.size	terminal_enter, .-terminal_enter
	.globl	terminal_handler
	.type	terminal_handler, @function
terminal_handler:
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
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	previousASCII_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L7
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$69, %ax
	ja	.L7
	call	terminal_console
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$1, %eax
	movl	%eax, previousASCII_pointer@GOTOFF(%ebx)
	jmp	.L8
.L7:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	previousKEY_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L9
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	keyboard_KEYBUFFER@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	movzbl	%al, %eax
	cmpl	$205, %eax
	je	.L10
	cmpl	$205, %eax
	jg	.L11
	cmpl	$203, %eax
	je	.L12
	cmpl	$203, %eax
	jg	.L11
	cmpl	$14, %eax
	je	.L13
	cmpl	$156, %eax
	je	.L14
	jmp	.L11
.L13:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L17
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	$20
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	leal	-1(%eax), %edx
	movl	Terminal_Buffer@GOT(%ebx), %eax
	movb	$0, (%eax,%edx)
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$4, %esp
	pushl	$32
	pushl	$20
	pushl	%eax
	call	printChar@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L17
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	subl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	jmp	.L17
.L14:
	call	terminal_enter
	jmp	.L11
.L12:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L18
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	$20
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	subl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	jmp	.L18
.L10:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$69, %ax
	ja	.L11
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	subl	$8, %esp
	pushl	$20
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	jmp	.L11
.L17:
	nop
	jmp	.L11
.L18:
	nop
.L11:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, previousKEY_pointer@GOTOFF(%ebx)
	jmp	.L8
.L9:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$16384, %ax
	jbe	.L8
	movw	$0, Terminal_Buffer_Pointer@GOTOFF(%ebx)
.L8:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	movzwl	%ax, %eax
	addl	%eax, %eax
	pushl	$0
	pushl	$16
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeData@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$22
	pushl	$0
	pushl	$0
	pushl	$16
	movl	STR_edit@GOT(%ebx), %eax
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
.LFE3:
	.size	terminal_handler, .-terminal_handler
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB4:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE4:
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
