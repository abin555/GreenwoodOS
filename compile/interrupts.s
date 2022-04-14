	.file	"interrupts.c"
	.text
	.comm	INT_Software_Value,1,1
	.globl	SYS_MODE
	.data
	.type	SYS_MODE, @object
	.size	SYS_MODE, 1
SYS_MODE:
	.byte	4
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,100,32
	.comm	keyboard_ASCIIBuffer,100,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	fb,4,4
	.comm	fb_cursor,4,4
	.comm	FG,1,1
	.comm	BG,1,1
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.comm	Terminal_Buffer,80,32
	.comm	Terminal_OUT_Buffer,3200,32
	.comm	Terminal_Arguments,80,32
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
	.globl	interrupt_handler
	.type	interrupt_handler, @function
interrupt_handler:
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
	cmpl	$33, 36(%ebp)
	je	.L11
	cmpl	$34, 36(%ebp)
	je	.L12
	jmp	.L14
.L11:
	call	keyboard_handle_interrupt@PLT
	subl	$12, %esp
	pushl	36(%ebp)
	call	pic_acknowledge@PLT
	addl	$16, %esp
	jmp	.L14
.L12:
	movzbl	SYS_MODE@GOTOFF(%ebx), %eax
	movzbl	%al, %eax
	subl	$4, %esp
	pushl	$83
	pushl	$10
	pushl	%eax
	call	printChar@PLT
	addl	$16, %esp
	call	KERNEL_INTERRUPT
	nop
.L14:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	interrupt_handler, .-interrupt_handler
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB4:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE4:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB5:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE5:
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
