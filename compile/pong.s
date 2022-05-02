	.file	"pong.c"
	.intel_syntax noprefix
	.text
	.comm	INT_Software_Value,4,4
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
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -16[ebp], eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -24[ebp], eax
	mov	DWORD PTR -28[ebp], -1
	mov	DWORD PTR -32[ebp], 1
	mov	BYTE PTR -33[ebp], 0
	mov	WORD PTR -36[ebp], 0
	mov	DWORD PTR -40[ebp], 4095
	sub	esp, 12
	push	0
	call	fb_clear@PLT
	add	esp, 16
.L20:
	movzx	eax, WORD PTR -36[ebp]
	add	eax, 1
	mov	WORD PTR -36[ebp], ax
	movzx	eax, WORD PTR -36[ebp]
	cdq
	idiv	DWORD PTR -40[ebp]
	mov	eax, edx
	test	eax, eax
	jne	.L2
	mov	edx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR -20[ebp]
	push	0
	push	10
	push	edx
	push	eax
	call	pixelScaled@PLT
	add	esp, 16
	mov	eax, DWORD PTR -28[ebp]
	add	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR -32[ebp]
	add	DWORD PTR -24[ebp], eax
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 49
	cmp	DWORD PTR -20[ebp], eax
	jge	.L3
	cmp	DWORD PTR -20[ebp], 49
	jg	.L4
.L3:
	cmp	DWORD PTR -20[ebp], 49
	jg	.L5
	mov	eax, DWORD PTR -16[ebp]
	sub	eax, 49
	cmp	DWORD PTR -24[ebp], eax
	jl	.L6
	mov	eax, DWORD PTR -16[ebp]
	add	eax, 49
	cmp	DWORD PTR -24[ebp], eax
	jg	.L6
	neg	DWORD PTR -28[ebp]
	movzx	eax, BYTE PTR -33[ebp]
	sub	eax, 1
	mov	BYTE PTR -33[ebp], al
	jmp	.L4
.L6:
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -24[ebp], eax
	neg	DWORD PTR -28[ebp]
	movzx	eax, BYTE PTR -33[ebp]
	add	eax, 1
	mov	BYTE PTR -33[ebp], al
	jmp	.L4
.L5:
	neg	DWORD PTR -28[ebp]
.L4:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 49
	cmp	DWORD PTR -24[ebp], eax
	jge	.L8
	cmp	DWORD PTR -24[ebp], 49
	jg	.L2
.L8:
	neg	DWORD PTR -32[ebp]
.L2:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -12[ebp], eax
	je	.L9
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 119
	jne	.L10
	cmp	DWORD PTR -16[ebp], 100
	jle	.L11
	sub	DWORD PTR -16[ebp], 25
	jmp	.L11
.L10:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 115
	jne	.L12
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 100
	cmp	DWORD PTR -16[ebp], eax
	jge	.L11
	add	DWORD PTR -16[ebp], 25
	jmp	.L11
.L12:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 91
	jne	.L13
	sar	DWORD PTR -40[ebp]
	jmp	.L11
.L13:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 93
	jne	.L14
	mov	eax, DWORD PTR -40[ebp]
	add	eax, eax
	or	eax, 1
	mov	DWORD PTR -40[ebp], eax
	jmp	.L11
.L14:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	cmp	al, 47
	je	.L22
.L11:
	mov	DWORD PTR -44[ebp], 0
	jmp	.L16
.L17:
	mov	eax, DWORD PTR -44[ebp]
	sub	esp, 4
	push	0
	push	eax
	push	50
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -44[ebp], 1
.L16:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -44[ebp], eax
	jl	.L17
.L9:
	mov	edx, DWORD PTR -24[ebp]
	mov	eax, DWORD PTR -20[ebp]
	push	16777215
	push	10
	push	edx
	push	eax
	call	pixelScaled@PLT
	add	esp, 16
	mov	DWORD PTR -48[ebp], 0
	jmp	.L18
.L19:
	mov	eax, DWORD PTR -16[ebp]
	lea	edx, -50[eax]
	mov	eax, DWORD PTR -48[ebp]
	add	eax, edx
	sub	esp, 4
	push	16777215
	push	eax
	push	50
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -48[ebp], 1
.L18:
	cmp	DWORD PTR -48[ebp], 99
	jle	.L19
	jmp	.L20
.L22:
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
