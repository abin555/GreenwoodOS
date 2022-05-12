	.file	"string.c"
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
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.globl	STR_Compare
	.type	STR_Compare, @function
STR_Compare:
.LFB0:
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
	mov	eax, DWORD PTR 16[ebp]
	mov	DWORD PTR -8[ebp], eax
	jmp	.L2
.L4:
	mov	edx, DWORD PTR -8[ebp]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	movzx	edx, BYTE PTR [eax]
	mov	ecx, DWORD PTR -8[ebp]
	mov	eax, DWORD PTR 12[ebp]
	add	eax, ecx
	movzx	eax, BYTE PTR [eax]
	cmp	dl, al
	jne	.L3
	add	DWORD PTR -4[ebp], 1
.L3:
	add	DWORD PTR -8[ebp], 1
.L2:
	mov	eax, DWORD PTR -8[ebp]
	cmp	eax, DWORD PTR 20[ebp]
	jl	.L4
	mov	eax, DWORD PTR -4[ebp]
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
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -4[ebp], 0
	jmp	.L7
.L8:
	mov	edx, DWORD PTR -4[ebp]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	mov	ecx, DWORD PTR -4[ebp]
	mov	edx, DWORD PTR 20[ebp]
	add	edx, ecx
	mov	ecx, edx
	mov	edx, DWORD PTR 12[ebp]
	add	edx, ecx
	movzx	eax, BYTE PTR [eax]
	mov	BYTE PTR [edx], al
	add	DWORD PTR -4[ebp], 1
.L7:
	mov	eax, DWORD PTR -4[ebp]
	cmp	eax, DWORD PTR 16[ebp]
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
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -4[ebp], 0
	jmp	.L10
.L13:
	mov	eax, DWORD PTR -4[ebp]
	mov	edx, DWORD PTR 12[ebp]
	mov	ecx, eax
	sar	edx, cl
	mov	eax, edx
	and	eax, 1
	test	eax, eax
	je	.L11
	mov	eax, DWORD PTR 16[ebp]
	sub	eax, DWORD PTR -4[ebp]
	mov	edx, eax
	mov	eax, DWORD PTR 20[ebp]
	add	eax, edx
	mov	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	mov	BYTE PTR [eax], 49
	jmp	.L12
.L11:
	mov	eax, DWORD PTR 16[ebp]
	sub	eax, DWORD PTR -4[ebp]
	mov	edx, eax
	mov	eax, DWORD PTR 20[ebp]
	add	eax, edx
	mov	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	mov	BYTE PTR [eax], 48
.L12:
	add	DWORD PTR -4[ebp], 1
.L10:
	mov	eax, DWORD PTR -4[ebp]
	cmp	eax, DWORD PTR 16[ebp]
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
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 4
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	mov	BYTE PTR -4[ebp], dl
	movsx	edx, BYTE PTR -4[ebp]
	cmp	edx, 15
	ja	.L15
	sal	edx, 2
	mov	edx, DWORD PTR .L17@GOTOFF[edx+eax]
	add	eax, edx
	notrack jmp	eax
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
	mov	eax, 48
	jmp	.L33
.L31:
	mov	eax, 49
	jmp	.L33
.L30:
	mov	eax, 50
	jmp	.L33
.L29:
	mov	eax, 51
	jmp	.L33
.L28:
	mov	eax, 52
	jmp	.L33
.L27:
	mov	eax, 53
	jmp	.L33
.L26:
	mov	eax, 54
	jmp	.L33
.L25:
	mov	eax, 55
	jmp	.L33
.L24:
	mov	eax, 56
	jmp	.L33
.L23:
	mov	eax, 57
	jmp	.L33
.L22:
	mov	eax, 65
	jmp	.L33
.L21:
	mov	eax, 66
	jmp	.L33
.L20:
	mov	eax, 67
	jmp	.L33
.L19:
	mov	eax, 68
	jmp	.L33
.L18:
	mov	eax, 69
	jmp	.L33
.L16:
	mov	eax, 70
	jmp	.L33
.L15:
	mov	eax, 120
.L33:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	quadtoHex, .-quadtoHex
	.globl	hexToQuad
	.type	hexToQuad, @function
hexToQuad:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 4
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	mov	BYTE PTR -4[ebp], dl
	movsx	edx, BYTE PTR -4[ebp]
	sub	edx, 48
	cmp	edx, 54
	ja	.L35
	sal	edx, 2
	mov	edx, DWORD PTR .L37@GOTOFF[edx+eax]
	add	eax, edx
	notrack jmp	eax
	.section	.rodata
	.align 4
	.align 4
.L37:
	.long	.L52@GOTOFF
	.long	.L51@GOTOFF
	.long	.L50@GOTOFF
	.long	.L49@GOTOFF
	.long	.L48@GOTOFF
	.long	.L47@GOTOFF
	.long	.L46@GOTOFF
	.long	.L45@GOTOFF
	.long	.L44@GOTOFF
	.long	.L43@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L42@GOTOFF
	.long	.L41@GOTOFF
	.long	.L40@GOTOFF
	.long	.L39@GOTOFF
	.long	.L38@GOTOFF
	.long	.L36@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L35@GOTOFF
	.long	.L42@GOTOFF
	.long	.L41@GOTOFF
	.long	.L40@GOTOFF
	.long	.L39@GOTOFF
	.long	.L38@GOTOFF
	.long	.L36@GOTOFF
	.text
.L52:
	mov	eax, 0
	jmp	.L53
.L51:
	mov	eax, 1
	jmp	.L53
.L50:
	mov	eax, 2
	jmp	.L53
.L49:
	mov	eax, 3
	jmp	.L53
.L48:
	mov	eax, 4
	jmp	.L53
.L47:
	mov	eax, 5
	jmp	.L53
.L46:
	mov	eax, 6
	jmp	.L53
.L45:
	mov	eax, 7
	jmp	.L53
.L44:
	mov	eax, 8
	jmp	.L53
.L43:
	mov	eax, 9
	jmp	.L53
.L42:
	mov	eax, 10
	jmp	.L53
.L41:
	mov	eax, 11
	jmp	.L53
.L40:
	mov	eax, 12
	jmp	.L53
.L39:
	mov	eax, 13
	jmp	.L53
.L38:
	mov	eax, 14
	jmp	.L53
.L36:
	mov	eax, 15
	jmp	.L53
.L35:
	mov	eax, 0
.L53:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	hexToQuad, .-hexToQuad
	.globl	decodeHex
	.type	decodeHex, @function
decodeHex:
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
	jmp	.L55
.L56:
	mov	eax, DWORD PTR -8[ebp]
	sal	eax, 2
	mov	edx, DWORD PTR 12[ebp]
	mov	ecx, eax
	shr	edx, cl
	mov	eax, edx
	movsx	eax, al
	and	eax, 15
	mov	edx, eax
	mov	eax, DWORD PTR 16[ebp]
	lea	ecx, 3[eax]
	test	eax, eax
	cmovs	eax, ecx
	sar	eax, 2
	sub	eax, DWORD PTR -8[ebp]
	mov	ecx, eax
	mov	eax, DWORD PTR 20[ebp]
	add	eax, ecx
	mov	ecx, eax
	mov	eax, DWORD PTR 8[ebp]
	lea	ebx, [ecx+eax]
	push	edx
	call	quadtoHex
	add	esp, 4
	mov	BYTE PTR [ebx], al
	add	DWORD PTR -8[ebp], 1
.L55:
	mov	eax, DWORD PTR 16[ebp]
	lea	edx, 3[eax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 2
	cmp	DWORD PTR -8[ebp], eax
	jl	.L56
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	decodeHex, .-decodeHex
	.globl	encodeHex
	.type	encodeHex, @function
encodeHex:
.LFB6:
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
	mov	DWORD PTR -12[ebp], 0
	jmp	.L58
.L59:
	mov	eax, DWORD PTR -8[ebp]
	sal	eax, 4
	mov	ebx, eax
	mov	edx, DWORD PTR 12[ebp]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	mov	edx, eax
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	eax
	call	hexToQuad
	add	esp, 4
	movsx	eax, al
	or	eax, ebx
	mov	DWORD PTR -8[ebp], eax
	add	DWORD PTR -12[ebp], 1
.L58:
	mov	eax, DWORD PTR 16[ebp]
	sub	eax, DWORD PTR 12[ebp]
	cmp	DWORD PTR -12[ebp], eax
	jl	.L59
	mov	eax, DWORD PTR -8[ebp]
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	encodeHex, .-encodeHex
	.globl	strcpy
	.type	strcpy, @function
strcpy:
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
	jmp	.L62
.L63:
	mov	edx, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR -4[ebp]
	add	eax, edx
	mov	ecx, DWORD PTR 12[ebp]
	mov	edx, DWORD PTR -4[ebp]
	add	edx, ecx
	movzx	eax, BYTE PTR [eax]
	mov	BYTE PTR [edx], al
	add	DWORD PTR -4[ebp], 1
.L62:
	mov	eax, DWORD PTR -4[ebp]
	cmp	eax, DWORD PTR 16[ebp]
	jb	.L63
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	strcpy, .-strcpy
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB8:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
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
