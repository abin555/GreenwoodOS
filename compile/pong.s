	.file	"pong.c"
	.intel_syntax noprefix
	.text
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
	.globl	pong
	.type	pong, @function
pong:
.LFB0:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 52
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -12[ebp], 0
	mov	DWORD PTR -16[ebp], 0
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -24[ebp], eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -28[ebp], eax
	mov	DWORD PTR -32[ebp], -1
	mov	DWORD PTR -36[ebp], 1
	mov	BYTE PTR -37[ebp], 0
	mov	WORD PTR -40[ebp], 0
	mov	DWORD PTR -44[ebp], 4095
	mov	DWORD PTR -48[ebp], 0
	sub	esp, 12
	push	0
	call	fb_clear@PLT
	add	esp, 16
.L24:
	movzx	eax, WORD PTR -40[ebp]
	add	eax, 1
	mov	WORD PTR -40[ebp], ax
	movzx	eax, WORD PTR -40[ebp]
	cdq
	idiv	DWORD PTR -44[ebp]
	mov	eax, edx
	test	eax, eax
	jne	.L2
	mov	edx, DWORD PTR -28[ebp]
	mov	eax, DWORD PTR -24[ebp]
	push	0
	push	10
	push	edx
	push	eax
	call	pixelScaled@PLT
	add	esp, 16
	mov	eax, DWORD PTR -32[ebp]
	add	DWORD PTR -24[ebp], eax
	mov	eax, DWORD PTR -36[ebp]
	add	DWORD PTR -28[ebp], eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 49
	cmp	DWORD PTR -24[ebp], eax
	jge	.L3
	cmp	DWORD PTR -24[ebp], 49
	jg	.L4
.L3:
	cmp	DWORD PTR -24[ebp], 49
	jg	.L5
	mov	eax, DWORD PTR -20[ebp]
	sub	eax, 49
	cmp	DWORD PTR -28[ebp], eax
	jl	.L6
	mov	eax, DWORD PTR -20[ebp]
	add	eax, 49
	cmp	DWORD PTR -28[ebp], eax
	jg	.L6
	neg	DWORD PTR -32[ebp]
	movzx	eax, BYTE PTR -37[ebp]
	sub	eax, 1
	mov	BYTE PTR -37[ebp], al
	jmp	.L4
.L6:
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -24[ebp], eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -28[ebp], eax
	neg	DWORD PTR -32[ebp]
	movzx	eax, BYTE PTR -37[ebp]
	add	eax, 1
	mov	BYTE PTR -37[ebp], al
	jmp	.L4
.L5:
	neg	DWORD PTR -32[ebp]
.L4:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 49
	cmp	DWORD PTR -28[ebp], eax
	jge	.L8
	cmp	DWORD PTR -28[ebp], 49
	jg	.L2
.L8:
	neg	DWORD PTR -36[ebp]
.L2:
	movzx	edx, WORD PTR -40[ebp]
	mov	eax, DWORD PTR -44[ebp]
	mov	ecx, eax
	shr	ecx, 31
	add	eax, ecx
	sar	eax
	mov	ecx, eax
	mov	eax, edx
	cdq
	idiv	ecx
	mov	eax, edx
	test	eax, eax
	jne	.L9
	cmp	DWORD PTR -48[ebp], 0
	jns	.L10
	cmp	DWORD PTR -20[ebp], 100
	jle	.L11
	mov	eax, DWORD PTR -48[ebp]
	add	DWORD PTR -20[ebp], eax
	jmp	.L11
.L10:
	cmp	DWORD PTR -48[ebp], 0
	jle	.L11
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 100
	cmp	DWORD PTR -20[ebp], eax
	jge	.L11
	mov	eax, DWORD PTR -48[ebp]
	add	DWORD PTR -20[ebp], eax
.L11:
	cmp	DWORD PTR -48[ebp], 0
	je	.L9
	mov	DWORD PTR -52[ebp], 0
	jmp	.L12
.L13:
	mov	eax, DWORD PTR -52[ebp]
	sub	esp, 4
	push	0
	push	eax
	push	50
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -52[ebp], 1
.L12:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -52[ebp], eax
	jl	.L13
.L9:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -16[ebp], eax
	je	.L14
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -16[ebp], eax
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 17
	jne	.L15
	mov	DWORD PTR -48[ebp], -1
	jmp	.L14
.L15:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, -111
	je	.L16
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, -97
	jne	.L17
.L16:
	mov	DWORD PTR -48[ebp], 0
	jmp	.L14
.L17:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 31
	jne	.L14
	mov	DWORD PTR -48[ebp], 1
.L14:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -12[ebp], eax
	je	.L18
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 91
	jne	.L19
	cmp	DWORD PTR -44[ebp], 20
	jle	.L18
	sar	DWORD PTR -44[ebp]
	jmp	.L18
.L19:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 93
	jne	.L20
	mov	eax, DWORD PTR -44[ebp]
	add	eax, eax
	or	eax, 1
	mov	DWORD PTR -44[ebp], eax
	jmp	.L18
.L20:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 47
	je	.L26
.L18:
	mov	edx, DWORD PTR -28[ebp]
	mov	eax, DWORD PTR -24[ebp]
	push	16777215
	push	10
	push	edx
	push	eax
	call	pixelScaled@PLT
	add	esp, 16
	mov	DWORD PTR -56[ebp], 0
	jmp	.L22
.L23:
	mov	eax, DWORD PTR -20[ebp]
	lea	edx, -50[eax]
	mov	eax, DWORD PTR -56[ebp]
	add	eax, edx
	sub	esp, 4
	push	16777215
	push	eax
	push	50
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -56[ebp], 1
.L22:
	cmp	DWORD PTR -56[ebp], 99
	jle	.L23
	jmp	.L24
.L26:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	pong, .-pong
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB1:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE1:
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
