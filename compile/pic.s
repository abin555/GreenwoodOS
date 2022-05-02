	.file	"pic.c"
	.intel_syntax noprefix
	.text
	.comm	INT_Software_Value,4,4
	.globl	pic_acknowledge
	.type	pic_acknowledge, @function
pic_acknowledge:
.LFB0:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 4
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 31
	jbe	.L6
	cmp	DWORD PTR 8[ebp], 47
	ja	.L6
	cmp	DWORD PTR 8[ebp], 39
	ja	.L5
	sub	esp, 8
	push	32
	push	32
	mov	ebx, eax
	call	outb@PLT
	add	esp, 16
	jmp	.L1
.L5:
	sub	esp, 8
	push	32
	push	160
	mov	ebx, eax
	call	outb@PLT
	add	esp, 16
	jmp	.L1
.L6:
	nop
.L1:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	pic_acknowledge, .-pic_acknowledge
	.globl	pic_remap
	.type	pic_remap, @function
pic_remap:
.LFB1:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 4
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 8
	push	17
	push	32
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	17
	push	160
	call	outb@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	movzx	eax, al
	sub	esp, 8
	push	eax
	push	33
	call	outb@PLT
	add	esp, 16
	mov	eax, DWORD PTR 12[ebp]
	movzx	eax, al
	sub	esp, 8
	push	eax
	push	161
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	4
	push	33
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	2
	push	161
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	1
	push	33
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	1
	push	161
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	253
	push	33
	call	outb@PLT
	add	esp, 16
	sub	esp, 8
	push	255
	push	161
	call	outb@PLT
	add	esp, 16
#APP
# 31 "pic.c" 1
	sti
# 0 "" 2
#NO_APP
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	pic_remap, .-pic_remap
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB2:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE2:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB3:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
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
