	.file	"keyboard.c"
	.text
	.comm	KYBRD_CAPS_LOCK,1,1
	.globl	KYBRD_SHIFT
	.bss
	.type	KYBRD_SHIFT, @object
	.size	KYBRD_SHIFT, 1
KYBRD_SHIFT:
	.zero	1
	.globl	kbd_US
	.data
	.align 32
	.type	kbd_US, @object
	.size	kbd_US, 128
kbd_US:
	.string	""
	.string	""
	.string	"1234567890-=\b\tqwertyuiop[]\n"
	.string	"asdfghjkl;'`"
	.string	"\\zxcvbnm,./"
	.string	"*"
	.string	" "
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"-"
	.string	""
	.string	""
	.string	"+"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.zero	37
	.comm	keyboard_KEYBUFFER,100,32
	.globl	keyboard_KEYBUFFER_POINTER
	.bss
	.align 4
	.type	keyboard_KEYBUFFER_POINTER, @object
	.size	keyboard_KEYBUFFER_POINTER, 4
keyboard_KEYBUFFER_POINTER:
	.zero	4
	.globl	prev_Scancode
	.type	prev_Scancode, @object
	.size	prev_Scancode, 1
prev_Scancode:
	.zero	1
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.globl	_keyboard_disable
	.data
	.type	_keyboard_disable, @object
	.size	_keyboard_disable, 1
_keyboard_disable:
	.byte	1
	.text
	.globl	keyboard_ctrl_read_status
	.type	keyboard_ctrl_read_status, @function
keyboard_ctrl_read_status:
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
	subl	$12, %esp
	pushl	$100
	movl	%eax, %ebx
	call	inb@PLT
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	keyboard_ctrl_read_status, .-keyboard_ctrl_read_status
	.globl	keyboard_ctrl_send_cmd
	.type	keyboard_ctrl_send_cmd, @function
keyboard_ctrl_send_cmd:
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
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
.L6:
	call	keyboard_ctrl_read_status
	movsbl	%al, %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	.L8
	jmp	.L6
.L8:
	nop
	movzbl	-12(%ebp), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$100
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
	.size	keyboard_ctrl_send_cmd, .-keyboard_ctrl_send_cmd
	.globl	keyboard_enc_read_buf
	.type	keyboard_enc_read_buf, @function
keyboard_enc_read_buf:
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
	subl	$12, %esp
	pushl	$96
	movl	%eax, %ebx
	call	inb@PLT
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	keyboard_enc_read_buf, .-keyboard_enc_read_buf
	.globl	keyboard_enc_send_cmd
	.type	keyboard_enc_send_cmd, @function
keyboard_enc_send_cmd:
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
	movl	8(%ebp), %eax
	movb	%al, -12(%ebp)
.L14:
	call	keyboard_ctrl_read_status
	movsbl	%al, %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	.L16
	jmp	.L14
.L16:
	nop
	movzbl	-12(%ebp), %eax
	movzbl	%al, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$96
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
	.size	keyboard_enc_send_cmd, .-keyboard_enc_send_cmd
	.globl	keyboard_set_leds
	.type	keyboard_set_leds, @function
keyboard_set_leds:
.LFB4:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %ecx
	movl	12(%ebp), %edx
	movl	16(%ebp), %eax
	movb	%cl, -28(%ebp)
	movb	%dl, -32(%ebp)
	movb	%al, -36(%ebp)
	movb	$0, -9(%ebp)
	cmpb	$0, -36(%ebp)
	je	.L18
	movzbl	-9(%ebp), %eax
	orl	$1, %eax
	jmp	.L19
.L18:
	movzbl	-9(%ebp), %eax
	andl	$1, %eax
.L19:
	movb	%al, -9(%ebp)
	cmpb	$0, -28(%ebp)
	je	.L20
	movzbl	-28(%ebp), %eax
	orl	$2, %eax
	jmp	.L21
.L20:
	movzbl	-28(%ebp), %eax
	andl	$2, %eax
.L21:
	movb	%al, -9(%ebp)
	cmpb	$0, -32(%ebp)
	je	.L22
	movzbl	-28(%ebp), %eax
	orl	$4, %eax
	jmp	.L23
.L22:
	movzbl	-28(%ebp), %eax
	andl	$4, %eax
.L23:
	movb	%al, -9(%ebp)
	subl	$12, %esp
	pushl	$-19
	call	keyboard_enc_send_cmd
	addl	$16, %esp
	movsbl	-9(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	keyboard_enc_send_cmd
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	keyboard_set_leds, .-keyboard_set_leds
	.globl	keyboard_self_test
	.type	keyboard_self_test, @function
keyboard_self_test:
.LFB5:
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
	subl	$12, %esp
	pushl	$-86
	call	keyboard_ctrl_send_cmd
	addl	$16, %esp
.L27:
	call	keyboard_ctrl_read_status
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L30
	jmp	.L27
.L30:
	nop
	call	keyboard_enc_read_buf
	cmpb	$85, %al
	sete	%al
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	keyboard_self_test, .-keyboard_self_test
	.globl	keyboard_disable
	.type	keyboard_disable, @function
keyboard_disable:
.LFB6:
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
	pushl	$-83
	call	keyboard_ctrl_send_cmd
	addl	$16, %esp
	movb	$1, _keyboard_disable@GOTOFF(%ebx)
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	keyboard_disable, .-keyboard_disable
	.globl	keyboard_enable
	.type	keyboard_enable, @function
keyboard_enable:
.LFB7:
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
	pushl	$-82
	call	keyboard_ctrl_send_cmd
	addl	$16, %esp
	movb	$0, _keyboard_disable@GOTOFF(%ebx)
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	keyboard_enable, .-keyboard_enable
	.globl	keyboard_keyread
	.type	keyboard_keyread, @function
keyboard_keyread:
.LFB8:
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
	subl	$12, %esp
	pushl	$-12
	call	keyboard_enc_send_cmd
	addl	$16, %esp
.L36:
	call	keyboard_ctrl_read_status
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L39
	jmp	.L36
.L39:
	nop
	call	keyboard_enc_read_buf
	movsbl	%al, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	keyboard_keyread, .-keyboard_keyread
	.globl	keyboard_flag_handler
	.type	keyboard_flag_handler, @function
keyboard_flag_handler:
.LFB9:
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
	movzbl	-4(%ebp), %edx
	cmpl	$42, %edx
	jne	.L42
	movb	$1, KYBRD_SHIFT@GOTOFF(%eax)
	nop
.L42:
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	keyboard_flag_handler, .-keyboard_flag_handler
	.globl	keyboard_handle_interrupt
	.type	keyboard_handle_interrupt, @function
keyboard_handle_interrupt:
.LFB10:
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
	call	keyboard_enc_read_buf
	movb	%al, -9(%ebp)
	movzbl	-9(%ebp), %eax
	movb	%al, prev_Scancode@GOTOFF(%ebx)
	movzbl	-9(%ebp), %eax
	testb	%al, %al
	js	.L46
	movzbl	-9(%ebp), %eax
	movzbl	kbd_US@GOTOFF(%ebx,%eax), %eax
	movb	%al, -10(%ebp)
	cmpb	$0, -10(%ebp)
	je	.L45
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	movzbl	-10(%ebp), %ecx
	movl	keyboard_KEYBUFFER@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movl	%eax, keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx)
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	movl	keyboard_KEYBUFFER@GOT(%ebx), %edx
	movb	$0, (%edx,%eax)
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	cmpl	$100, %eax
	jbe	.L46
	movl	$0, keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx)
	jmp	.L46
.L45:
	movzbl	-9(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	keyboard_flag_handler
	addl	$16, %esp
.L46:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	keyboard_handle_interrupt, .-keyboard_handle_interrupt
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB11:
	.cfi_startproc
	movl	(%esp), %eax
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
