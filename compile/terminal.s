	.file	"terminal.c"
	.text
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.comm	FG,1,1
	.comm	BG,1,1
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
	subl	$8, %esp
	pushl	$0
	pushl	$2
	call	fb_set_color@PLT
	addl	$16, %esp
	subl	$4, %esp
	pushl	$62
	pushl	$20
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$15
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
	.globl	terminal_interpret
	.type	terminal_interpret, @function
terminal_interpret:
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
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -12(%ebp)
	movl	$0, -16(%ebp)
	jmp	.L4
.L6:
	movl	Terminal_Arguments@GOT(%eax), %ecx
	movl	-16(%ebp), %edx
	addl	%ecx, %edx
	movb	$0, (%edx)
	movl	Terminal_Buffer@GOT(%eax), %ecx
	movl	-16(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%edx), %edx
	cmpb	$32, %dl
	jne	.L5
	movl	-16(%ebp), %edx
	movl	%edx, %ebx
	movl	Terminal_Arguments@GOT(%eax), %ecx
	movl	-12(%ebp), %edx
	addl	%ecx, %edx
	movb	%bl, (%edx)
	addl	$1, -12(%ebp)
.L5:
	addl	$1, -16(%ebp)
.L4:
	cmpl	$79, -16(%ebp)
	jle	.L6
	movl	Terminal_Arguments@GOT(%eax), %edx
	movzbl	(%edx), %edx
	movsbl	%dl, %edx
	subl	$12, %esp
	pushl	$21
	pushl	$2
	pushl	$0
	pushl	%edx
	movl	Terminal_Buffer@GOT(%eax), %edx
	pushl	%edx
	movl	%eax, %ebx
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
.LFE2:
	.size	terminal_interpret, .-terminal_interpret
	.globl	terminal_enter
	.type	terminal_enter, @function
terminal_enter:
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
	call	terminal_interpret
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
	subl	$12, %esp
	pushl	$0
	pushl	$0
	pushl	%eax
	pushl	$1520
	movl	Terminal_OUT_Buffer@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	addl	$80, %eax
	movl	%eax, Terminal_OUT_pointer@GOTOFF(%ebx)
	movw	$0, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	movl	$0, -12(%ebp)
	jmp	.L8
.L9:
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
.L8:
	cmpl	$79, -12(%ebp)
	jle	.L9
	subl	$8, %esp
	pushl	$20
	pushl	$1
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$2
	call	fb_set_color@PLT
	addl	$16, %esp
	subl	$4, %esp
	pushl	$62
	pushl	$20
	pushl	$0
	call	printChar@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$15
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
.LFE3:
	.size	terminal_enter, .-terminal_enter
	.globl	terminal_handler
	.type	terminal_handler, @function
terminal_handler:
.LFB4:
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
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	previousASCII_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L11
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$79, %ax
	ja	.L11
	call	terminal_console
	movl	keyboard_ascii_pointer@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$1, %eax
	movl	%eax, previousASCII_pointer@GOTOFF(%ebx)
	jmp	.L12
.L11:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	previousKEY_pointer@GOTOFF(%ebx), %eax
	cmpl	%eax, %edx
	je	.L13
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	-1(%eax), %edx
	movl	keyboard_KEYBUFFER@GOT(%ebx), %eax
	movzbl	(%eax,%edx), %eax
	movzbl	%al, %eax
	cmpl	$205, %eax
	je	.L14
	cmpl	$205, %eax
	jg	.L15
	cmpl	$203, %eax
	je	.L16
	cmpl	$203, %eax
	jg	.L15
	cmpl	$200, %eax
	je	.L17
	cmpl	$200, %eax
	jg	.L15
	cmpl	$14, %eax
	je	.L18
	cmpl	$156, %eax
	je	.L19
	jmp	.L15
.L18:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L27
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
	je	.L27
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	subl	$1, %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	jmp	.L27
.L19:
	call	terminal_enter
	jmp	.L15
.L16:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	testw	%ax, %ax
	je	.L28
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
	jmp	.L28
.L14:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$79, %ax
	ja	.L29
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
	jmp	.L29
.L17:
	movl	$0, -12(%ebp)
	jmp	.L23
.L26:
	movl	Terminal_OUT_pointer@GOTOFF(%ebx), %eax
	leal	-80(%eax), %edx
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
	movl	-12(%ebp), %edx
	addl	$1, %edx
	subl	$4, %esp
	pushl	%eax
	pushl	$20
	pushl	%edx
	call	printChar@PLT
	addl	$16, %esp
	movl	Terminal_Buffer@GOT(%ebx), %edx
	movl	-12(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L24
	movl	-12(%ebp), %eax
	addl	$1, %eax
	subl	$8, %esp
	pushl	$20
	pushl	%eax
	call	fb_move_cursor_xy@PLT
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movw	%ax, Terminal_Buffer_Pointer@GOTOFF(%ebx)
	nop
	jmp	.L15
.L24:
	addl	$1, -12(%ebp)
.L23:
	cmpl	$79, -12(%ebp)
	jle	.L26
	jmp	.L15
.L27:
	nop
	jmp	.L15
.L28:
	nop
	jmp	.L15
.L29:
	nop
.L15:
	movl	keyboard_KEYBUFFER_POINTER@GOT(%ebx), %eax
	movl	(%eax), %eax
	movl	%eax, previousKEY_pointer@GOTOFF(%ebx)
	jmp	.L12
.L13:
	movzwl	Terminal_Buffer_Pointer@GOTOFF(%ebx), %eax
	cmpw	$16384, %ax
	jbe	.L12
	movw	$0, Terminal_Buffer_Pointer@GOTOFF(%ebx)
.L12:
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
	pushl	$23
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
.LFE4:
	.size	terminal_handler, .-terminal_handler
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
