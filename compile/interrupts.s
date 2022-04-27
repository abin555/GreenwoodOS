	.file	"interrupts.c"
	.text
	.comm	INT_Software_Value,4,4
	.globl	SYS_MODE
	.data
	.type	SYS_MODE, @object
	.size	SYS_MODE, 1
SYS_MODE:
	.byte	1
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.comm	Terminal_Buffer,128,32
	.comm	Terminal_OUT_Buffer,5120,32
	.comm	Terminal_Arguments,128,32
	.comm	idt_descriptors,2048,32
	.comm	idt,6,4
	.comm	BUFFER_COUNT,4,4
	.text
	.globl	interrupts_init_descriptor
	.type	interrupts_init_descriptor, @function
interrupts_init_descriptor:
.LFB0:
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
	movl	12(%ebp), %edx
	shrl	$16, %edx
	movl	%edx, %ebx
	movl	idt_descriptors@GOT(%eax), %edx
	movl	8(%ebp), %ecx
	movw	%bx, 6(%edx,%ecx,8)
	movl	12(%ebp), %edx
	movl	%edx, %ebx
	movl	idt_descriptors@GOT(%eax), %edx
	movl	8(%ebp), %ecx
	movw	%bx, (%edx,%ecx,8)
	movl	idt_descriptors@GOT(%eax), %edx
	movl	8(%ebp), %ecx
	movw	$8, 2(%edx,%ecx,8)
	movl	idt_descriptors@GOT(%eax), %edx
	movl	8(%ebp), %ecx
	movb	$0, 4(%edx,%ecx,8)
	movl	idt_descriptors@GOT(%eax), %eax
	movl	8(%ebp), %edx
	movb	$-114, 5(%eax,%edx,8)
	nop
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	interrupts_init_descriptor, .-interrupts_init_descriptor
	.globl	interrupt_install_idt
	.type	interrupt_install_idt, @function
interrupt_install_idt:
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
	movl	int_handler_33@GOT(%ebx), %eax
	pushl	%eax
	pushl	$33
	call	interrupts_init_descriptor
	addl	$8, %esp
	movl	int_handler_34@GOT(%ebx), %eax
	pushl	%eax
	pushl	$34
	call	interrupts_init_descriptor
	addl	$8, %esp
	movl	int_handler_128@GOT(%ebx), %eax
	pushl	%eax
	pushl	$128
	call	interrupts_init_descriptor
	addl	$8, %esp
	movl	idt_descriptors@GOT(%ebx), %eax
	movl	%eax, %edx
	movl	idt@GOT(%ebx), %eax
	movl	%edx, 2(%eax)
	movl	idt@GOT(%ebx), %eax
	movw	$2048, (%eax)
	movl	idt@GOT(%ebx), %eax
	subl	$12, %esp
	pushl	%eax
	call	load_idt@PLT
	addl	$16, %esp
	subl	$8, %esp
	pushl	$40
	pushl	$32
	call	pic_remap@PLT
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
	.size	interrupt_install_idt, .-interrupt_install_idt
	.globl	KERNEL_INTERRUPT
	.type	KERNEL_INTERRUPT, @function
KERNEL_INTERRUPT:
.LFB2:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	INT_Software_Value@GOT(%eax), %edx
	movl	(%edx), %edx
	movzbl	(%edx), %edx
	movzbl	%dl, %edx
	cmpl	$4, %edx
	je	.L4
	cmpl	$4, %edx
	jg	.L9
	cmpl	$3, %edx
	je	.L6
	cmpl	$3, %edx
	jg	.L9
	cmpl	$1, %edx
	je	.L7
	cmpl	$2, %edx
	je	.L8
	jmp	.L9
.L7:
	movb	$1, SYS_MODE@GOTOFF(%eax)
	jmp	.L5
.L8:
	movb	$2, SYS_MODE@GOTOFF(%eax)
	jmp	.L5
.L6:
	movb	$3, SYS_MODE@GOTOFF(%eax)
	jmp	.L5
.L4:
	movb	$4, SYS_MODE@GOTOFF(%eax)
	nop
.L5:
.L9:
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	KERNEL_INTERRUPT, .-KERNEL_INTERRUPT
	.globl	SYS_CALL
	.type	SYS_CALL, @function
SYS_CALL:
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
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	subl	$4, %esp
	pushl	$83
	pushl	$1
	pushl	$79
	call	printChar@PLT
	addl	$16, %esp
	movl	8(%ebp), %eax
	pushl	$0
	pushl	$32
	pushl	%eax
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	decodeHex@PLT
	addl	$16, %esp
	subl	$12, %esp
	pushl	$21
	pushl	$0
	pushl	$0
	pushl	$9
	movl	STR_edit@GOT(%ebx), %eax
	pushl	%eax
	call	fb_write_xy@PLT
	addl	$32, %esp
	movl	8(%ebp), %eax
	cmpl	$5, %eax
	ja	.L18
	sall	$2, %eax
	movl	.L13@GOTOFF(%eax,%ebx), %eax
	addl	%ebx, %eax
	notrack jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L13:
	.long	.L18@GOTOFF
	.long	.L17@GOTOFF
	.long	.L16@GOTOFF
	.long	.L19@GOTOFF
	.long	.L19@GOTOFF
	.long	.L12@GOTOFF
	.text
.L17:
	subl	$4, %esp
	pushl	$80
	pushl	$2
	pushl	$79
	call	printChar@PLT
	addl	$16, %esp
	movl	BG@GOT(%ebx), %eax
	movl	(%eax), %esi
	movl	FG@GOT(%ebx), %eax
	movl	(%eax), %ecx
	movl	16(%ebp), %eax
	movsbl	%al, %edx
	movl	12(%ebp), %eax
	pushl	%esi
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	call	fb_write_cell@PLT
	addl	$16, %esp
	jmp	.L11
.L16:
	movl	20(%ebp), %edx
	movl	16(%ebp), %eax
	movl	12(%ebp), %ecx
	subl	$4, %esp
	pushl	%edx
	pushl	%eax
	pushl	%ecx
	call	fb_write_start@PLT
	addl	$16, %esp
	jmp	.L11
.L12:
	movl	20(%ebp), %ecx
	movl	16(%ebp), %edx
	movl	12(%ebp), %eax
	subl	$4, %esp
	pushl	%ecx
	pushl	%edx
	pushl	%eax
	call	fb_setPixel@PLT
	addl	$16, %esp
	jmp	.L11
.L19:
	nop
.L11:
.L18:
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
.LFE3:
	.size	SYS_CALL, .-SYS_CALL
	.globl	interrupt_handler
	.type	interrupt_handler, @function
interrupt_handler:
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
	cmpl	$128, 36(%ebp)
	je	.L21
	cmpl	$128, 36(%ebp)
	ja	.L26
	cmpl	$33, 36(%ebp)
	je	.L23
	cmpl	$34, 36(%ebp)
	je	.L24
	jmp	.L26
.L23:
	call	keyboard_handle_interrupt@PLT
	subl	$12, %esp
	pushl	36(%ebp)
	call	pic_acknowledge@PLT
	addl	$16, %esp
	jmp	.L25
.L24:
	call	KERNEL_INTERRUPT
	jmp	.L25
.L21:
	subl	$4, %esp
	pushl	32(%ebp)
	pushl	28(%ebp)
	pushl	24(%ebp)
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	SYS_CALL
	addl	$32, %esp
	jmp	.L25
.L26:
	nop
.L25:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	interrupt_handler, .-interrupt_handler
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
