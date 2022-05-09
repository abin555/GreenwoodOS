	.file	"frame_buffer.c"
	.intel_syntax noprefix
	.text
	.comm	memory_used,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.section	.rodata
	.align 32
	.type	FONT, @object
	.size	FONT, 1024
FONT:
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
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\030<<\030\030"
	.string	"\030"
	.string	"66"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"66\1776\17766"
	.string	"\f>\003\0360\037\f"
	.string	""
	.string	"c3\030\ffc"
	.string	"\0346\034n;3n"
	.string	"\006\006\003"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\030\f\006\006\006\f\030"
	.string	"\006\f\030\030\030\f\006"
	.string	""
	.string	"f<\377<f"
	.string	""
	.string	""
	.string	"\f\f?\f\f"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.ascii	"\f\f\006"
	.string	""
	.string	""
	.string	""
	.string	"?"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\f\f"
	.string	"`0\030\f\006\003\001"
	.string	">cs{og>"
	.string	"\f\016\f\f\f\f?"
	.string	"\03630\034\0063?"
	.string	"\03630\03403\036"
	.string	"8<63\1770x"
	.string	"?\003\037003\036"
	.string	"\034\006\003\03733\036"
	.string	"?30\030\f\f\f"
	.string	"\03633\03633\036"
	.string	"\03633>0\030\016"
	.string	""
	.string	"\f\f"
	.string	""
	.string	"\f\f"
	.string	""
	.string	"\f\f"
	.string	""
	.ascii	"\f\f\006"
	.string	"\030\f\006\003\006\f\030"
	.string	""
	.string	""
	.string	"?"
	.string	""
	.string	"?"
	.string	""
	.string	"\006\f\0300\030\f\006"
	.string	"\03630\030\f"
	.string	"\f"
	.string	">c{{{\003\036"
	.string	"\f\03633?33"
	.string	"?ff>ff?"
	.string	"<f\003\003\003f<"
	.string	"\0376fff6\037"
	.string	"\177F\026\036\026F\177"
	.string	"\177F\026\036\026\006\017"
	.string	"<f\003\003sf|"
	.string	"333?333"
	.string	"\036\f\f\f\f\f\036"
	.string	"x00033\036"
	.string	"gf6\0366fg"
	.string	"\017\006\006\006Ff\177"
	.string	"cw\177\177kcc"
	.string	"cgo{scc"
	.string	"\0346ccc6\034"
	.string	"?ff>\006\006\017"
	.string	"\036333;\0368"
	.string	"?ff>6fg"
	.string	"\0363\007\01683\036"
	.string	"?-\f\f\f\f\036"
	.string	"333333?"
	.string	"33333\036\f"
	.string	"ccck\177wc"
	.string	"cc6\034\0346c"
	.string	"333\036\f\f\036"
	.string	"\177c1\030Lf\177"
	.string	"\036\006\006\006\006\006\036"
	.string	"\003\006\f\0300`@"
	.string	"\036\030\030\030\030\030\036"
	.string	"\b\0346c"
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
	.ascii	"\377"
	.string	"\f\f\030"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\0360>3n"
	.string	"\007\006\006>ff;"
	.string	""
	.string	""
	.string	"\0363\0033\036"
	.string	"800>33n"
	.string	""
	.string	""
	.string	"\0363?\003\036"
	.string	"\0346\006\017\006\006\017"
	.string	""
	.string	""
	.ascii	"n33>0\037"
	.string	"\007\0066nffg"
	.string	"\f"
	.string	"\016\f\f\f\036"
	.string	"0"
	.ascii	"00033\036"
	.string	"\007\006f6\0366g"
	.string	"\016\f\f\f\f\f\036"
	.string	""
	.string	""
	.string	"3\177\177kc"
	.string	""
	.string	""
	.string	"\0373333"
	.string	""
	.string	""
	.string	"\036333\036"
	.string	""
	.string	""
	.ascii	";ff>\006\017"
	.string	""
	.string	""
	.ascii	"n33>0x"
	.string	""
	.string	""
	.string	";nf\006\017"
	.string	""
	.string	""
	.string	">\003\0360\037"
	.string	"\b\f>\f\f,\030"
	.string	""
	.string	""
	.string	"3333n"
	.string	""
	.string	""
	.string	"333\036\f"
	.string	""
	.string	""
	.string	"ck\177\1776"
	.string	""
	.string	""
	.string	"c6\0346c"
	.string	""
	.string	""
	.ascii	"333>0\037"
	.string	""
	.string	""
	.string	"?\031\f&?"
	.string	"8\f\f\007\f\f8"
	.string	"\030\030\030"
	.string	"\030\030\030"
	.string	"\007\f\f8\f\f\007"
	.string	"n;"
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
	.globl	fb_cursor
	.bss
	.align 4
	.type	fb_cursor, @object
	.size	fb_cursor, 4
fb_cursor:
	.zero	4
	.text
	.globl	fb_setPixel
	.type	fb_setPixel, @function
fb_setPixel:
.LFB0:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	edx, DWORD PTR [edx]
	cmp	DWORD PTR 8[ebp], edx
	jnb	.L5
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	cmp	DWORD PTR 12[ebp], edx
	jnb	.L5
	mov	edx, DWORD PTR fb@GOT[eax]
	mov	edx, DWORD PTR [edx]
	mov	eax, DWORD PTR fb_width@GOT[eax]
	mov	eax, DWORD PTR [eax]
	imul	eax, DWORD PTR 12[ebp]
	mov	ecx, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, ecx
	sal	eax, 2
	add	edx, eax
	mov	eax, DWORD PTR 16[ebp]
	mov	DWORD PTR [edx], eax
	jmp	.L1
.L5:
	nop
.L1:
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	fb_setPixel, .-fb_setPixel
	.globl	init_fb
	.type	init_fb, @function
init_fb:
.LFB1:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	mov	ecx, DWORD PTR 8[edx]
	mov	ebx, DWORD PTR 12[edx]
	mov	edx, ecx
	mov	ecx, edx
	mov	edx, DWORD PTR fb@GOT[eax]
	mov	DWORD PTR [edx], ecx
	mov	edx, DWORD PTR 8[ebp]
	mov	ecx, DWORD PTR 24[edx]
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	DWORD PTR [edx], ecx
	mov	edx, DWORD PTR 8[ebp]
	mov	ecx, DWORD PTR 20[edx]
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	DWORD PTR [edx], ecx
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	edx, DWORD PTR [edx]
	shr	edx, 3
	mov	ecx, edx
	mov	edx, DWORD PTR fb_terminal_w@GOT[eax]
	mov	DWORD PTR [edx], ecx
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	shr	edx, 3
	mov	eax, DWORD PTR fb_terminal_h@GOT[eax]
	mov	DWORD PTR [eax], edx
	nop
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	init_fb, .-init_fb
	.globl	fb_write_cell
	.type	fb_write_cell, @function
fb_write_cell:
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
	sub	esp, 20
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 12[ebp]
	mov	BYTE PTR -28[ebp], al
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	ecx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, 0
	div	ecx
	mov	eax, edx
	sal	eax, 3
	mov	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	ecx, eax
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, 0
	div	ecx
	sal	eax, 3
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -12[ebp], 0
	jmp	.L8
.L13:
	mov	DWORD PTR -16[ebp], 0
	jmp	.L9
.L12:
	movsx	edx, BYTE PTR -28[ebp]
	lea	eax, FONT@GOTOFF[ebx]
	sal	edx, 3
	add	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	movzx	edx, al
	mov	eax, DWORD PTR -16[ebp]
	mov	ecx, eax
	sar	edx, cl
	mov	eax, edx
	and	eax, 1
	test	eax, eax
	je	.L10
	mov	eax, DWORD PTR 16[ebp]
	jmp	.L11
.L10:
	mov	eax, DWORD PTR 20[ebp]
.L11:
	mov	edx, DWORD PTR fb@GOT[ebx]
	mov	ecx, DWORD PTR [edx]
	mov	esi, DWORD PTR -12[ebp]
	mov	edx, DWORD PTR -24[ebp]
	add	esi, edx
	mov	edx, DWORD PTR fb_width@GOT[ebx]
	mov	edx, DWORD PTR [edx]
	imul	esi, edx
	mov	edx, DWORD PTR -20[ebp]
	add	esi, edx
	mov	edx, DWORD PTR -16[ebp]
	add	edx, esi
	sal	edx, 2
	add	edx, ecx
	mov	DWORD PTR [edx], eax
	add	DWORD PTR -16[ebp], 1
.L9:
	cmp	DWORD PTR -16[ebp], 7
	jle	.L12
	add	DWORD PTR -12[ebp], 1
.L8:
	cmp	DWORD PTR -12[ebp], 7
	jle	.L13
	nop
	nop
	add	esp, 20
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
	.size	fb_write_cell, .-fb_write_cell
	.globl	printChar
	.type	printChar, @function
printChar:
.LFB3:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 20
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 16[ebp]
	mov	BYTE PTR -28[ebp], dl
	mov	DWORD PTR -12[ebp], 0
	jmp	.L15
.L20:
	mov	DWORD PTR -16[ebp], 0
	jmp	.L16
.L19:
	movsx	ecx, BYTE PTR -28[ebp]
	lea	edx, FONT@GOTOFF[eax]
	sal	ecx, 3
	add	ecx, edx
	mov	edx, DWORD PTR -12[ebp]
	add	edx, ecx
	movzx	edx, BYTE PTR [edx]
	movzx	ebx, dl
	mov	edx, DWORD PTR -16[ebp]
	mov	ecx, edx
	sar	ebx, cl
	mov	edx, ebx
	and	edx, 1
	test	edx, edx
	je	.L17
	mov	edx, DWORD PTR FG@GOT[eax]
	mov	edx, DWORD PTR [edx]
	jmp	.L18
.L17:
	mov	edx, DWORD PTR BG@GOT[eax]
	mov	edx, DWORD PTR [edx]
.L18:
	mov	ecx, DWORD PTR fb@GOT[eax]
	mov	ebx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR 12[ebp]
	lea	esi, 0[0+ecx*8]
	mov	ecx, DWORD PTR -12[ebp]
	add	esi, ecx
	mov	ecx, DWORD PTR fb_width@GOT[eax]
	mov	ecx, DWORD PTR [ecx]
	imul	ecx, esi
	mov	esi, DWORD PTR 8[ebp]
	sal	esi, 3
	add	esi, ecx
	mov	ecx, DWORD PTR -16[ebp]
	add	ecx, esi
	sal	ecx, 2
	add	ecx, ebx
	mov	DWORD PTR [ecx], edx
	add	DWORD PTR -16[ebp], 1
.L16:
	cmp	DWORD PTR -16[ebp], 7
	jle	.L19
	add	DWORD PTR -12[ebp], 1
.L15:
	cmp	DWORD PTR -12[ebp], 7
	jle	.L20
	nop
	nop
	add	esp, 20
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	printChar, .-printChar
	.globl	printChar_Scaled
	.type	printChar_Scaled, @function
printChar_Scaled:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 32
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 16[ebp]
	mov	BYTE PTR -28[ebp], al
	mov	DWORD PTR -12[ebp], 0
	jmp	.L22
.L27:
	mov	DWORD PTR -16[ebp], 0
	jmp	.L23
.L26:
	movsx	edx, BYTE PTR -28[ebp]
	lea	eax, FONT@GOTOFF[ebx]
	sal	edx, 3
	add	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	movzx	edx, al
	mov	eax, DWORD PTR -16[ebp]
	mov	ecx, eax
	sar	edx, cl
	mov	eax, edx
	and	eax, 1
	test	eax, eax
	je	.L24
	mov	eax, DWORD PTR FG@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	jmp	.L25
.L24:
	mov	eax, DWORD PTR BG@GOT[ebx]
	mov	eax, DWORD PTR [eax]
.L25:
	mov	ecx, DWORD PTR -12[ebp]
	mov	edx, DWORD PTR 12[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR 20[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR 20[ebp]
	imul	ecx, edx
	mov	esi, DWORD PTR -16[ebp]
	mov	edx, DWORD PTR 8[ebp]
	add	esi, edx
	mov	edx, DWORD PTR 20[ebp]
	add	esi, edx
	mov	edx, DWORD PTR 20[ebp]
	imul	edx, esi
	push	eax
	push	DWORD PTR 20[ebp]
	push	ecx
	push	edx
	call	pixelScaled
	add	esp, 16
	add	DWORD PTR -16[ebp], 1
.L23:
	cmp	DWORD PTR -16[ebp], 7
	jle	.L26
	add	DWORD PTR -12[ebp], 1
.L22:
	cmp	DWORD PTR -12[ebp], 7
	jle	.L27
	nop
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
.LFE4:
	.size	printChar_Scaled, .-printChar_Scaled
	.globl	pixelScaled
	.type	pixelScaled, @function
pixelScaled:
.LFB5:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 16
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -8[ebp], 0
	jmp	.L29
.L32:
	mov	DWORD PTR -12[ebp], 0
	jmp	.L30
.L31:
	mov	edx, DWORD PTR fb@GOT[eax]
	mov	ecx, DWORD PTR [edx]
	mov	ebx, DWORD PTR -12[ebp]
	mov	edx, DWORD PTR 12[ebp]
	add	ebx, edx
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	edx, DWORD PTR [edx]
	imul	ebx, edx
	mov	edx, DWORD PTR 8[ebp]
	add	ebx, edx
	mov	edx, DWORD PTR -8[ebp]
	add	edx, ebx
	sal	edx, 2
	add	ecx, edx
	mov	edx, DWORD PTR 20[ebp]
	mov	DWORD PTR [ecx], edx
	add	DWORD PTR -12[ebp], 1
.L30:
	mov	edx, DWORD PTR -12[ebp]
	cmp	edx, DWORD PTR 16[ebp]
	jl	.L31
	add	DWORD PTR -8[ebp], 1
.L29:
	mov	edx, DWORD PTR -8[ebp]
	cmp	edx, DWORD PTR 16[ebp]
	jl	.L32
	nop
	nop
	add	esp, 16
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	pixelScaled, .-pixelScaled
	.globl	fb_set_color
	.type	fb_set_color, @function
fb_set_color:
.LFB6:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR FG@GOT[eax]
	mov	ecx, DWORD PTR 8[ebp]
	mov	DWORD PTR [edx], ecx
	mov	eax, DWORD PTR BG@GOT[eax]
	mov	edx, DWORD PTR 12[ebp]
	mov	DWORD PTR [eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	fb_set_color, .-fb_set_color
	.globl	fb_clear
	.type	fb_clear, @function
fb_clear:
.LFB7:
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
	mov	DWORD PTR -4[ebp], 0
	jmp	.L35
.L36:
	mov	edx, DWORD PTR fb@GOT[eax]
	mov	edx, DWORD PTR [edx]
	mov	ecx, DWORD PTR -4[ebp]
	sal	ecx, 2
	add	ecx, edx
	mov	edx, DWORD PTR 8[ebp]
	mov	DWORD PTR [ecx], edx
	add	DWORD PTR -4[ebp], 1
.L35:
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	ecx, DWORD PTR [edx]
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	imul	edx, ecx
	cmp	DWORD PTR -4[ebp], edx
	jb	.L36
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	fb_clear, .-fb_clear
	.globl	fb_write
	.type	fb_write, @function
fb_write:
.LFB8:
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
	sub	esp, 16
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -16[ebp], 0
	jmp	.L38
.L39:
	mov	eax, DWORD PTR BG@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR FG@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	esi, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR -16[ebp]
	add	eax, esi
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	mov	esi, DWORD PTR fb_cursor@GOTOFF[ebx]
	mov	edi, esi
	mov	esi, DWORD PTR -16[ebp]
	add	esi, edi
	push	ecx
	push	edx
	push	eax
	push	esi
	call	fb_write_cell
	add	esp, 16
	add	DWORD PTR -16[ebp], 1
.L38:
	mov	eax, DWORD PTR -16[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L39
	mov	eax, 0
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
.LFE8:
	.size	fb_write, .-fb_write
	.globl	fb_write_start
	.type	fb_write_start, @function
fb_write_start:
.LFB9:
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
	sub	esp, 16
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -16[ebp], 0
	jmp	.L42
.L43:
	mov	eax, DWORD PTR BG@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR FG@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	esi, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR 16[ebp]
	add	esi, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, esi
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	mov	esi, DWORD PTR fb_cursor@GOTOFF[ebx]
	mov	edi, esi
	mov	esi, DWORD PTR -16[ebp]
	add	esi, edi
	push	ecx
	push	edx
	push	eax
	push	esi
	call	fb_write_cell
	add	esp, 16
	add	DWORD PTR -16[ebp], 1
.L42:
	mov	eax, DWORD PTR -16[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jb	.L43
	mov	eax, 0
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
.LFE9:
	.size	fb_write_start, .-fb_write_start
	.globl	fb_write_xy
	.type	fb_write_xy, @function
fb_write_xy:
.LFB10:
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
	sub	esp, 16
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -16[ebp], 0
	jmp	.L46
.L47:
	mov	eax, DWORD PTR BG@GOT[ebx]
	mov	ecx, DWORD PTR [eax]
	mov	eax, DWORD PTR FG@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	esi, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR 16[ebp]
	add	eax, esi
	mov	esi, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, esi
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	mov	esi, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	esi, DWORD PTR [esi]
	mov	edi, esi
	imul	edi, DWORD PTR 24[ebp]
	mov	esi, DWORD PTR 20[ebp]
	add	edi, esi
	mov	esi, DWORD PTR -16[ebp]
	add	esi, edi
	push	ecx
	push	edx
	push	eax
	push	esi
	call	fb_write_cell
	add	esp, 16
	add	DWORD PTR -16[ebp], 1
.L46:
	mov	eax, DWORD PTR -16[ebp]
	cmp	eax, DWORD PTR 12[ebp]
	jl	.L47
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
.LFE10:
	.size	fb_write_xy, .-fb_write_xy
	.globl	fb_move_cursor
	.type	fb_move_cursor, @function
fb_move_cursor:
.LFB11:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	mov	DWORD PTR fb_cursor@GOTOFF[eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	fb_move_cursor, .-fb_move_cursor
	.globl	fb_move_cursor_xy
	.type	fb_move_cursor_xy, @function
fb_move_cursor_xy:
.LFB12:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR fb_terminal_w@GOT[eax]
	mov	edx, DWORD PTR [edx]
	mov	ecx, edx
	imul	ecx, DWORD PTR 12[ebp]
	mov	edx, DWORD PTR 8[ebp]
	add	edx, ecx
	mov	DWORD PTR fb_cursor@GOTOFF[eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	fb_move_cursor_xy, .-fb_move_cursor_xy
	.globl	fb_copyBuffer
	.type	fb_copyBuffer, @function
fb_copyBuffer:
.LFB13:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 16
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -8[ebp], 0
	jmp	.L51
.L52:
	mov	edx, DWORD PTR fb@GOT[eax]
	mov	edx, DWORD PTR [edx]
	mov	ecx, DWORD PTR -8[ebp]
	sal	ecx, 2
	lea	ebx, [edx+ecx]
	mov	edx, DWORD PTR fb_backBuffer@GOT[eax]
	mov	ecx, DWORD PTR -8[ebp]
	mov	edx, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR [ebx], edx
	add	DWORD PTR -8[ebp], 1
.L51:
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	ecx, DWORD PTR [edx]
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	imul	edx, ecx
	cmp	DWORD PTR -8[ebp], edx
	jb	.L52
	nop
	nop
	add	esp, 16
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	fb_copyBuffer, .-fb_copyBuffer
	.globl	fb_clearBackBuffer
	.type	fb_clearBackBuffer, @function
fb_clearBackBuffer:
.LFB14:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 16
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -8[ebp], 0
	jmp	.L54
.L55:
	mov	edx, DWORD PTR fb_backBuffer@GOT[eax]
	mov	ecx, DWORD PTR -8[ebp]
	mov	ebx, DWORD PTR 8[ebp]
	mov	DWORD PTR [edx+ecx*4], ebx
	add	DWORD PTR -8[ebp], 1
.L54:
	mov	edx, DWORD PTR fb_width@GOT[eax]
	mov	ecx, DWORD PTR [edx]
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	imul	edx, ecx
	cmp	DWORD PTR -8[ebp], edx
	jb	.L55
	nop
	nop
	add	esp, 16
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	fb_clearBackBuffer, .-fb_clearBackBuffer
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB15:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE15:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB16:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE16:
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
