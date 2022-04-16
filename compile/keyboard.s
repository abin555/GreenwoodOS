	.file	"keyboard.c"
	.text
	.comm	INT_Software_Value,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.globl	KYBRD_SHIFT
	.bss
	.type	KYBRD_SHIFT, @object
	.size	KYBRD_SHIFT, 1
KYBRD_SHIFT:
	.zero	1
	.comm	keyboard_KEYBUFFER,100,32
	.comm	keyboard_ASCIIBuffer,100,32
	.globl	keyboard_KEYBUFFER_POINTER
	.align 4
	.type	keyboard_KEYBUFFER_POINTER, @object
	.size	keyboard_KEYBUFFER_POINTER, 4
keyboard_KEYBUFFER_POINTER:
	.zero	4
	.globl	keyboard_ascii_pointer
	.align 4
	.type	keyboard_ascii_pointer, @object
	.size	keyboard_ascii_pointer, 4
keyboard_ascii_pointer:
	.zero	4
	.globl	prev_Scancode
	.type	prev_Scancode, @object
	.size	prev_Scancode, 1
prev_Scancode:
	.zero	1
	.comm	char_scancode,1,1
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.comm	FG,1,1
	.comm	BG,1,1
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
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
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.dx
	addl	$_GLOBAL_OFFSET_TABLE_, %edx
	movl	8(%ebp), %eax
	movb	%al, -28(%ebp)
	movzbl	-28(%ebp), %eax
	cmpl	$211, %eax
	je	.L41
	cmpl	$211, %eax
	jg	.L48
	cmpl	$170, %eax
	je	.L43
	cmpl	$170, %eax
	jg	.L48
	cmpl	$91, %eax
	je	.L44
	cmpl	$91, %eax
	jg	.L48
	cmpl	$29, %eax
	je	.L45
	cmpl	$42, %eax
	jne	.L48
	movb	$1, KYBRD_SHIFT@GOTOFF(%edx)
	subl	$4, %esp
	pushl	$0
	pushl	$1
	pushl	$0
	call	keyboard_set_leds
	addl	$16, %esp
	jmp	.L42
.L43:
	movb	$0, KYBRD_SHIFT@GOTOFF(%edx)
	subl	$4, %esp
	pushl	$0
	pushl	$0
	pushl	$0
	call	keyboard_set_leds
	addl	$16, %esp
	jmp	.L42
.L45:
	subl	$12, %esp
	pushl	$1
	movl	%edx, %ebx
	call	software_interrupt@PLT
	addl	$16, %esp
	jmp	.L42
.L44:
	subl	$12, %esp
	pushl	$4
	movl	%edx, %ebx
	call	software_interrupt@PLT
	addl	$16, %esp
	jmp	.L42
.L41:
	movl	$0, -12(%ebp)
	jmp	.L46
.L47:
	movl	keyboard_KEYBUFFER@GOT(%edx), %ecx
	movl	-12(%ebp), %eax
	addl	%ecx, %eax
	movb	$-86, (%eax)
	movl	keyboard_ASCIIBuffer@GOT(%edx), %ecx
	movl	-12(%ebp), %eax
	addl	%ecx, %eax
	movb	$88, (%eax)
	addl	$1, -12(%ebp)
.L46:
	cmpl	$199, -12(%ebp)
	jle	.L47
	nop
.L42:
.L48:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	keyboard_flag_handler, .-keyboard_flag_handler
	.globl	convertascii
	.type	convertascii, @function
convertascii:
.LFB10:
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
	movl	kbd_US@GOT(%eax), %ecx
	movzbl	(%ecx,%edx), %edx
	testb	%dl, %dl
	je	.L50
	movzbl	KYBRD_SHIFT@GOTOFF(%eax), %edx
	testb	%dl, %dl
	je	.L51
	movzbl	-4(%ebp), %edx
	movl	kbd_US_shift@GOT(%eax), %eax
	movzbl	(%eax,%edx), %eax
	jmp	.L52
.L51:
	movzbl	-4(%ebp), %edx
	movl	kbd_US@GOT(%eax), %eax
	movzbl	(%eax,%edx), %eax
	jmp	.L52
.L50:
	movl	$0, %eax
.L52:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	convertascii, .-convertascii
	.globl	keyboard_handle_interrupt
	.type	keyboard_handle_interrupt, @function
keyboard_handle_interrupt:
.LFB11:
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
	cmpb	$0, -9(%ebp)
	je	.L54
	movzbl	-9(%ebp), %eax
	movl	kbd_US@GOT(%ebx), %edx
	movzbl	(%edx,%eax), %eax
	testb	%al, %al
	je	.L55
	movzbl	KYBRD_SHIFT@GOTOFF(%ebx), %eax
	testb	%al, %al
	je	.L56
	movzbl	-9(%ebp), %edx
	movl	keyboard_ascii_pointer@GOTOFF(%ebx), %eax
	movl	kbd_US_shift@GOT(%ebx), %ecx
	movzbl	(%ecx,%edx), %ecx
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
	jmp	.L57
.L56:
	movzbl	-9(%ebp), %edx
	movl	keyboard_ascii_pointer@GOTOFF(%ebx), %eax
	movl	kbd_US@GOT(%ebx), %ecx
	movzbl	(%ecx,%edx), %ecx
	movl	keyboard_ASCIIBuffer@GOT(%ebx), %edx
	movb	%cl, (%edx,%eax)
.L57:
	movl	keyboard_ascii_pointer@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movl	%eax, keyboard_ascii_pointer@GOTOFF(%ebx)
	jmp	.L58
.L55:
	movzbl	-9(%ebp), %eax
	subl	$12, %esp
	pushl	%eax
	call	keyboard_flag_handler
	addl	$16, %esp
.L58:
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	movl	keyboard_KEYBUFFER@GOT(%ebx), %edx
	movzbl	-9(%ebp), %ecx
	movb	%cl, (%edx,%eax)
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	addl	$1, %eax
	movl	%eax, keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx)
.L54:
	movl	keyboard_ascii_pointer@GOTOFF(%ebx), %eax
	cmpl	$99, %eax
	jbe	.L59
	movl	$0, keyboard_ascii_pointer@GOTOFF(%ebx)
.L59:
	movl	keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx), %eax
	cmpl	$99, %eax
	jbe	.L61
	movl	$0, keyboard_KEYBUFFER_POINTER@GOTOFF(%ebx)
.L61:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	keyboard_handle_interrupt, .-keyboard_handle_interrupt
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB12:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE12:
	.section	.text.__x86.get_pc_thunk.dx,"axG",@progbits,__x86.get_pc_thunk.dx,comdat
	.globl	__x86.get_pc_thunk.dx
	.hidden	__x86.get_pc_thunk.dx
	.type	__x86.get_pc_thunk.dx, @function
__x86.get_pc_thunk.dx:
.LFB13:
	.cfi_startproc
	movl	(%esp), %edx
	ret
	.cfi_endproc
.LFE13:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB14:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE14:
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
