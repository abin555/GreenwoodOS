	.file	"grapher.c"
	.intel_syntax noprefix
	.text
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.comm	INT_Software_Value,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	KYBRD_CTRL,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	settings_data,36,32
	.comm	formulas,336,32
	.comm	previousAscii_Pointer,4,4
	.comm	previousKey_Pointer,4,4
	.comm	axis_center_x,4,4
	.comm	axis_center_y,4,4
	.globl	draw_regions
	.type	draw_regions, @function
draw_regions:
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
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -858993459
	mul	edx
	mov	eax, edx
	shr	eax, 2
	sub	ecx, eax
	mov	edx, ecx
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax, 2
	sub	ecx, eax
	mov	eax, ecx
	push	16777215
	push	edx
	push	eax
	push	0
	call	gfx_vline@PLT
	add	esp, 16
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax, 2
	sub	edx, eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	16777215
	push	edx
	push	eax
	push	0
	call	gfx_hline@PLT
	add	esp, 16
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	draw_regions, .-draw_regions
	.globl	draw_axis
	.type	draw_axis, @function
draw_axis:
.LFB1:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	edi
	push	esi
	push	ebx
	sub	esp, 28
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 4
	push	16777215
	push	edx
	push	eax
	call	fb_setPixel@PLT
	add	esp, 16
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	esi, eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edi, eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	mov	eax, edx
	shr	eax
	sub	edi, eax
	mov	eax, edi
	push	16777215
	push	esi
	push	ecx
	push	eax
	call	gfx_hline@PLT
	add	esp, 16
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	esi, eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	ecx, [edx+eax]
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edi, eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	mov	eax, edx
	shr	eax
	sub	edi, eax
	mov	eax, edi
	push	16777215
	push	esi
	push	ecx
	push	eax
	call	gfx_vline@PLT
	add	esp, 16
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -28[ebp], eax
	jmp	.L3
.L4:
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 4[eax]
	mov	ecx, eax
	mov	eax, edx
	mov	edx, 0
	div	ecx
	mov	edx, eax
	mov	eax, DWORD PTR -28[ebp]
	imul	edx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	edx, eax
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 10
	mov	ecx, eax
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 10
	push	16777215
	push	edx
	push	ecx
	push	eax
	call	gfx_vline@PLT
	add	esp, 16
	add	DWORD PTR -28[ebp], 1
.L3:
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 4[eax]
	cmp	DWORD PTR -28[ebp], eax
	jle	.L4
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 12[eax]
	mov	DWORD PTR -32[ebp], eax
	jmp	.L5
.L6:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 8[eax]
	mov	ecx, eax
	mov	eax, edx
	mov	edx, 0
	div	ecx
	mov	edx, eax
	mov	eax, DWORD PTR -32[ebp]
	imul	edx, eax
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	edx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 10
	mov	ecx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 10
	push	16777215
	push	edx
	push	ecx
	push	eax
	call	gfx_hline@PLT
	add	esp, 16
	add	DWORD PTR -32[ebp], 1
.L5:
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 8[eax]
	cmp	DWORD PTR -32[ebp], eax
	jle	.L6
	nop
	nop
	lea	esp, -12[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	edi
	.cfi_restore 7
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	draw_axis, .-draw_axis
	.globl	plot_point
	.type	plot_point, @function
plot_point:
.LFB2:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 16
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.cx
	add	ecx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR settings_data@GOT[ecx]
	movzx	eax, BYTE PTR 24[eax]
	movsx	eax, al
	and	eax, 1
	test	eax, eax
	je	.L8
	mov	eax, DWORD PTR axis_center_y@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -24[ebp], eax
	fild	DWORD PTR -24[ebp]
	fld	DWORD PTR 12[ebp]
	fchs
	mov	eax, DWORD PTR fb_height@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ecx]
	mov	eax, DWORD PTR 8[eax]
	mov	ebx, eax
	mov	eax, edx
	mov	edx, 0
	div	ebx
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -20[ebp], 0
	fild	QWORD PTR -24[ebp]
	fmulp	st(1), st
	faddp	st(1), st
	fnstcw	WORD PTR -10[ebp]
	movzx	eax, WORD PTR -10[ebp]
	or	ah, 12
	mov	WORD PTR -12[ebp], ax
	fldcw	WORD PTR -12[ebp]
	fistp	QWORD PTR -24[ebp]
	fldcw	WORD PTR -10[ebp]
	mov	ebx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR axis_center_x@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -24[ebp], eax
	fild	DWORD PTR -24[ebp]
	mov	eax, DWORD PTR fb_width@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ecx]
	mov	eax, DWORD PTR 4[eax]
	mov	esi, eax
	mov	eax, edx
	mov	edx, 0
	div	esi
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -20[ebp], 0
	fild	QWORD PTR -24[ebp]
	fmul	DWORD PTR 8[ebp]
	faddp	st(1), st
	fldcw	WORD PTR -12[ebp]
	fistp	QWORD PTR -24[ebp]
	fldcw	WORD PTR -10[ebp]
	mov	eax, DWORD PTR -24[ebp]
	push	16711935
	push	3
	push	ebx
	push	eax
	mov	ebx, ecx
	call	pixelScaled@PLT
	add	esp, 16
	jmp	.L10
.L8:
	mov	eax, DWORD PTR axis_center_y@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -24[ebp], eax
	fild	DWORD PTR -24[ebp]
	fld	DWORD PTR 12[ebp]
	fchs
	mov	eax, DWORD PTR fb_height@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ecx]
	mov	eax, DWORD PTR 8[eax]
	mov	ebx, eax
	mov	eax, edx
	mov	edx, 0
	div	ebx
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -20[ebp], 0
	fild	QWORD PTR -24[ebp]
	fmulp	st(1), st
	faddp	st(1), st
	fnstcw	WORD PTR -10[ebp]
	movzx	eax, WORD PTR -10[ebp]
	or	ah, 12
	mov	WORD PTR -12[ebp], ax
	fldcw	WORD PTR -12[ebp]
	fistp	QWORD PTR -24[ebp]
	fldcw	WORD PTR -10[ebp]
	mov	ebx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR axis_center_x@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -24[ebp], eax
	fild	DWORD PTR -24[ebp]
	mov	eax, DWORD PTR fb_width@GOT[ecx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	shr	edx
	mov	eax, DWORD PTR settings_data@GOT[ecx]
	mov	eax, DWORD PTR 4[eax]
	mov	esi, eax
	mov	eax, edx
	mov	edx, 0
	div	esi
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -20[ebp], 0
	fild	QWORD PTR -24[ebp]
	fmul	DWORD PTR 8[ebp]
	faddp	st(1), st
	fldcw	WORD PTR -12[ebp]
	fistp	QWORD PTR -24[ebp]
	fldcw	WORD PTR -10[ebp]
	mov	eax, DWORD PTR -24[ebp]
	sub	esp, 4
	push	16711935
	push	ebx
	push	eax
	mov	ebx, ecx
	call	fb_setPixel@PLT
	add	esp, 16
.L10:
	nop
	lea	esp, -8[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	plot_point, .-plot_point
	.globl	draw_graph
	.type	draw_graph, @function
draw_graph:
.LFB3:
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
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -28[ebp], eax
	fild	DWORD PTR -28[ebp]
	fstp	DWORD PTR -12[ebp]
	jmp	.L12
.L16:
	fld	DWORD PTR -12[ebp]
	fld	QWORD PTR .LC0@GOTOFF[ebx]
	fmulp	st(1), st
	fld	DWORD PTR -12[ebp]
	fmulp	st(1), st
	fld	DWORD PTR -12[ebp]
	fld	DWORD PTR .LC1@GOTOFF[ebx]
	fmulp	st(1), st
	fsubp	st(1), st
	fstp	DWORD PTR -16[ebp]
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 12[eax]
	mov	DWORD PTR -28[ebp], eax
	fild	DWORD PTR -28[ebp]
	fld	DWORD PTR -16[ebp]
	fcomip	st, st(1)
	fstp	st(0)
	jbe	.L13
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 8[eax]
	mov	DWORD PTR -28[ebp], eax
	fild	DWORD PTR -28[ebp]
	fld	DWORD PTR -16[ebp]
	fxch	st(1)
	fcomip	st, st(1)
	fstp	st(0)
	jbe	.L13
	sub	esp, 8
	push	DWORD PTR -16[ebp]
	push	DWORD PTR -12[ebp]
	call	plot_point
	add	esp, 16
.L13:
	fld	DWORD PTR -12[ebp]
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	fld	QWORD PTR 28[eax]
	faddp	st(1), st
	fstp	DWORD PTR -12[ebp]
.L12:
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 4[eax]
	mov	DWORD PTR -28[ebp], eax
	fild	DWORD PTR -28[ebp]
	fld	DWORD PTR -12[ebp]
	fxch	st(1)
	fcomip	st, st(1)
	fstp	st(0)
	jnb	.L16
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	draw_graph, .-draw_graph
	.globl	grapher_entry
	.type	grapher_entry, @function
grapher_entry:
.LFB4:
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
	sub	esp, 12
	push	0
	call	fb_clear@PLT
	add	esp, 16
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -858993459
	mul	edx
	mov	eax, edx
	shr	eax, 2
	sub	ecx, eax
	mov	eax, ecx
	shr	eax
	mov	edx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax, 2
	sub	edx, eax
	mov	eax, edx
	shr	eax
	mov	edx, eax
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 16[eax], 1
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 20[eax], 1
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR [eax], -10
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 4[eax], 10
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 8[eax], 10
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 12[eax], -10
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	movzx	edx, BYTE PTR 24[eax]
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	BYTE PTR 24[eax], dl
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	fld	QWORD PTR .LC2@GOTOFF[ebx]
	fstp	QWORD PTR 28[eax]
	call	draw_regions
	call	draw_axis
	call	draw_graph
.L22:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 47
	je	.L24
	jmp	.L22
.L24:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	grapher_entry, .-grapher_entry
	.globl	sqrt
	.type	sqrt, @function
sqrt:
.LFB5:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	fld	DWORD PTR 8[ebp]
	fld	DWORD PTR .LC3@GOTOFF[eax]
	fmulp	st(1), st
	fstp	DWORD PTR -4[ebp]
	lea	edx, 8[ebp]
	mov	edx, DWORD PTR [edx]
	mov	DWORD PTR -8[ebp], edx
	mov	edx, DWORD PTR -8[ebp]
	sar	edx
	mov	ecx, 1597463007
	sub	ecx, edx
	mov	edx, ecx
	mov	DWORD PTR -8[ebp], edx
	lea	edx, -8[ebp]
	fld	DWORD PTR [edx]
	fstp	DWORD PTR 8[ebp]
	fld	DWORD PTR 8[ebp]
	fmul	DWORD PTR -4[ebp]
	fld	DWORD PTR 8[ebp]
	fmulp	st(1), st
	fld	DWORD PTR .LC4@GOTOFF[eax]
	fsubrp	st(1), st
	fld	DWORD PTR 8[ebp]
	fmulp	st(1), st
	fstp	DWORD PTR 8[ebp]
	fld	DWORD PTR 8[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	sqrt, .-sqrt
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1071644672
	.align 4
.LC1:
	.long	1082130432
	.align 8
.LC2:
	.long	3944497965
	.long	1058682594
	.align 4
.LC3:
	.long	1056964608
	.align 4
.LC4:
	.long	1069547520
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB6:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE6:
	.section	.text.__x86.get_pc_thunk.cx,"axG",@progbits,__x86.get_pc_thunk.cx,comdat
	.globl	__x86.get_pc_thunk.cx
	.hidden	__x86.get_pc_thunk.cx
	.type	__x86.get_pc_thunk.cx, @function
__x86.get_pc_thunk.cx:
.LFB7:
	.cfi_startproc
	mov	ecx, DWORD PTR [esp]
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
	mov	ebx, DWORD PTR [esp]
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
