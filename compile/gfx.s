	.file	"gfx.c"
	.intel_syntax noprefix
	.text
	.comm	memory_used,4,4
	.comm	heap_begin,4,4
	.comm	heap_end,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.globl	gfx_line
	.type	gfx_line, @function
gfx_line:
.LFB0:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 36
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 20[ebp]
	sub	eax, DWORD PTR 12[ebp]
	mov	edx, DWORD PTR 16[ebp]
	mov	ecx, edx
	sub	ecx, DWORD PTR 8[ebp]
	mov	edx, 0
	div	ecx
	mov	DWORD PTR -40[ebp], eax
	mov	DWORD PTR -36[ebp], 0
	fild	QWORD PTR -40[ebp]
	fstp	QWORD PTR -24[ebp]
	mov	eax, DWORD PTR 8[ebp]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L2
.L3:
	mov	eax, DWORD PTR -12[ebp]
	mov	edx, 0
	mov	DWORD PTR -40[ebp], eax
	mov	DWORD PTR -36[ebp], edx
	fild	QWORD PTR -40[ebp]
	fmul	QWORD PTR -24[ebp]
	mov	eax, DWORD PTR 12[ebp]
	mov	edx, 0
	mov	DWORD PTR -40[ebp], eax
	mov	DWORD PTR -36[ebp], edx
	fild	QWORD PTR -40[ebp]
	faddp	st(1), st
	fnstcw	WORD PTR -26[ebp]
	movzx	eax, WORD PTR -26[ebp]
	or	ah, 12
	mov	WORD PTR -28[ebp], ax
	fldcw	WORD PTR -28[ebp]
	fistp	QWORD PTR -40[ebp]
	fldcw	WORD PTR -26[ebp]
	mov	eax, DWORD PTR -40[ebp]
	sub	esp, 4
	push	DWORD PTR 24[ebp]
	push	eax
	push	DWORD PTR -12[ebp]
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -12[ebp], 1
.L2:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 16[ebp]
	jb	.L3
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	gfx_line, .-gfx_line
	.globl	gfx_hline
	.type	gfx_hline, @function
gfx_hline:
.LFB1:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 8[ebp]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L5
.L6:
	sub	esp, 4
	push	DWORD PTR 20[ebp]
	push	DWORD PTR 16[ebp]
	push	DWORD PTR -12[ebp]
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -12[ebp], 1
.L5:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L6
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	gfx_hline, .-gfx_hline
	.globl	gfx_vline
	.type	gfx_vline, @function
gfx_vline:
.LFB2:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 20
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 8[ebp]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L8
.L9:
	sub	esp, 4
	push	DWORD PTR 20[ebp]
	push	DWORD PTR -12[ebp]
	push	DWORD PTR 16[ebp]
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -12[ebp], 1
.L8:
	mov	eax, DWORD PTR -12[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L9
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	gfx_vline, .-gfx_vline
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
