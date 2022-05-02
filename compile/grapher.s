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
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.comm	settings_data,24,4
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
	sub	esp, 12
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
	.globl	draw_graph
	.type	draw_graph, @function
draw_graph:
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
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L4
.L5:
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 20[eax]
	mov	edx, DWORD PTR -12[ebp]
	add	edx, 10
	imul	edx, DWORD PTR -12[ebp]
	sub	edx, 2
	imul	edx, eax
	mov	eax, DWORD PTR axis_center_y@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	ecx, eax
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 16[eax]
	imul	eax, DWORD PTR -12[ebp]
	mov	edx, eax
	mov	eax, DWORD PTR axis_center_x@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	sub	esp, 4
	push	16711935
	push	ecx
	push	eax
	call	fb_setPixel@PLT
	add	esp, 16
	add	DWORD PTR -12[ebp], 1
.L4:
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	eax, DWORD PTR 4[eax]
	cmp	DWORD PTR -12[ebp], eax
	jle	.L5
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
	.size	draw_graph, .-draw_graph
	.globl	grapher_entry
	.type	grapher_entry, @function
grapher_entry:
.LFB3:
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
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	mov	eax, edx
	shr	eax
	neg	eax
	mov	edx, eax
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, -1431655765
	mul	edx
	mov	eax, edx
	shr	eax
	mov	edx, eax
	mov	eax, DWORD PTR settings_data@GOT[ebx]
	mov	DWORD PTR 4[eax], edx
	call	draw_regions
	call	draw_axis
	call	draw_graph
.L7:
	jmp	.L7
	.cfi_endproc
.LFE3:
	.size	grapher_entry, .-grapher_entry
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB4:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE4:
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
