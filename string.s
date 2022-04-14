	.file	"string.c"
	.text
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
	.globl	STR_Compare
	.type	STR_Compare, @function
STR_Compare:
.LFB0:
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
	movl	16(%ebp), %eax
	movl	%eax, -8(%ebp)
	jmp	.L2
.L4:
	movl	-8(%ebp), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %edx
	movl	-8(%ebp), %ecx
	movl	12(%ebp), %eax
	addl	%ecx, %eax
	movzbl	(%eax), %eax
	cmpb	%al, %dl
	jne	.L3
	addl	$1, -4(%ebp)
.L3:
	addl	$1, -8(%ebp)
.L2:
	movl	-8(%ebp), %eax
	cmpl	20(%ebp), %eax
	jl	.L4
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	STR_Compare, .-STR_Compare
	.globl	STR_INSERT
	.type	STR_INSERT, @function
STR_INSERT:
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
	jmp	.L7
.L8:
	movl	-4(%ebp), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	-4(%ebp), %ecx
	movl	20(%ebp), %edx
	addl	%ecx, %edx
	movl	%edx, %ecx
	movl	12(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%eax), %eax
	movb	%al, (%edx)
	addl	$1, -4(%ebp)
.L7:
	movl	-4(%ebp), %eax
	cmpl	16(%ebp), %eax
	jl	.L8
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	STR_INSERT, .-STR_INSERT
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
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$1, -4(%ebp)
	jmp	.L10
.L13:
	movl	-4(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%eax, %ecx
	sarl	%cl, %edx
	movl	%edx, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L11
	movl	16(%ebp), %eax
	subl	-4(%ebp), %eax
	movl	%eax, %edx
	movl	20(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movb	$49, (%eax)
	jmp	.L12
.L11:
	movl	16(%ebp), %eax
	subl	-4(%ebp), %eax
	movl	%eax, %edx
	movl	20(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movb	$48, (%eax)
.L12:
	addl	$1, -4(%ebp)
.L10:
	movl	-4(%ebp), %eax
	cmpl	16(%ebp), %eax
	jl	.L13
	nop
	nop
	leave
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
	ja	.L15
	sall	$2, %edx
	movl	.L17@GOTOFF(%edx,%eax), %edx
	addl	%edx, %eax
	notrack jmp	*%eax
	.section	.rodata
	.align 4
	.align 4
.L17:
	.long	.L32@GOTOFF
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
	.long	.L16@GOTOFF
	.text
.L32:
	movl	$48, %eax
	jmp	.L33
.L31:
	movl	$49, %eax
	jmp	.L33
.L30:
	movl	$50, %eax
	jmp	.L33
.L29:
	movl	$51, %eax
	jmp	.L33
.L28:
	movl	$52, %eax
	jmp	.L33
.L27:
	movl	$53, %eax
	jmp	.L33
.L26:
	movl	$54, %eax
	jmp	.L33
.L25:
	movl	$55, %eax
	jmp	.L33
.L24:
	movl	$56, %eax
	jmp	.L33
.L23:
	movl	$57, %eax
	jmp	.L33
.L22:
	movl	$65, %eax
	jmp	.L33
.L21:
	movl	$66, %eax
	jmp	.L33
.L20:
	movl	$67, %eax
	jmp	.L33
.L19:
	movl	$68, %eax
	jmp	.L33
.L18:
	movl	$69, %eax
	jmp	.L33
.L16:
	movl	$70, %eax
	jmp	.L33
.L15:
	movl	$120, %eax
.L33:
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
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -8(%ebp)
	jmp	.L35
.L36:
	movl	-8(%ebp), %eax
	sall	$2, %eax
	movl	12(%ebp), %edx
	movl	%eax, %ecx
	sarl	%cl, %edx
	movl	%edx, %eax
	movsbl	%al, %eax
	andl	$15, %eax
	movl	%eax, %edx
	movl	16(%ebp), %eax
	leal	3(%eax), %ecx
	testl	%eax, %eax
	cmovs	%ecx, %eax
	sarl	$2, %eax
	subl	-8(%ebp), %eax
	movl	%eax, %ecx
	movl	20(%ebp), %eax
	addl	%ecx, %eax
	movl	%eax, %ecx
	movl	8(%ebp), %eax
	leal	(%ecx,%eax), %ebx
	pushl	%edx
	call	quadtoHex
	addl	$4, %esp
	movb	%al, (%ebx)
	addl	$1, -8(%ebp)
.L35:
	movl	16(%ebp), %eax
	leal	3(%eax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$2, %eax
	cmpl	%eax, -8(%ebp)
	jl	.L36
	nop
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	decodeHex, .-decodeHex
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
