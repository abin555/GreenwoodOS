	.file	"terminal.c"
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
	.comm	settings_data,36,32
	.comm	grapher_interface,8,4
	.comm	formulas,352,32
	.comm	previousAscii_Pointer,4,4
	.comm	previousKey_Pointer,4,4
	.comm	axis_center_x,4,4
	.comm	axis_center_y,4,4
	.comm	Terminal_Buffer,75,32
	.comm	Terminal_OUT_Buffer,3000,32
	.comm	Terminal_Arguments,75,32
	.globl	Terminal_OUT_pointer
	.bss
	.align 4
	.type	Terminal_OUT_pointer, @object
	.size	Terminal_OUT_pointer, 4
Terminal_OUT_pointer:
	.zero	4
	.globl	Terminal_Buffer_Pointer
	.align 2
	.type	Terminal_Buffer_Pointer, @object
	.size	Terminal_Buffer_Pointer, 2
Terminal_Buffer_Pointer:
	.zero	2
	.globl	previousASCII_pointer
	.align 4
	.type	previousASCII_pointer, @object
	.size	previousASCII_pointer, 4
previousASCII_pointer:
	.zero	4
	.globl	previousKEY_pointer
	.align 4
	.type	previousKEY_pointer, @object
	.size	previousKEY_pointer, 4
previousKEY_pointer:
	.zero	4
	.globl	Terminal_Y
	.data
	.align 4
	.type	Terminal_Y, @object
	.size	Terminal_Y, 4
Terminal_Y:
	.long	92
	.text
	.globl	terminal_memory_view
	.type	terminal_memory_view, @function
terminal_memory_view:
.LFB2:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	terminal_memory_view, .-terminal_memory_view
	.globl	terminal_renderer
	.type	terminal_renderer, @function
terminal_renderer:
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
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	sub	esp, 8
	push	eax
	push	1
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	sub	esp, 4
	push	62
	push	eax
	push	0
	call	printChar@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	sub	esp, 12
	push	edx
	push	1
	push	eax
	push	75
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	terminal_renderer, .-terminal_renderer
	.globl	terminal_console
	.type	terminal_console, @function
terminal_console:
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
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	je	.L4
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	ecx, -1[eax]
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	mov	edx, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	ecx, BYTE PTR [edx+ecx]
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	BYTE PTR [edx+eax], cl
	jmp	.L5
.L4:
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	mov	edx, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	movzx	ecx, BYTE PTR 254[edx]
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	BYTE PTR [edx+eax], cl
.L5:
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	movzx	eax, BYTE PTR [edx+eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	ecx, edx
	movzx	edx, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	edx, dx
	add	edx, 1
	sub	esp, 4
	push	eax
	push	ecx
	push	edx
	call	printChar@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	add	eax, 2
	sub	esp, 8
	push	edx
	push	eax
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	add	eax, 1
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], ax
	sub	esp, 8
	push	0
	push	65280
	call	fb_set_color@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	sub	esp, 4
	push	62
	push	eax
	push	0
	call	printChar@PLT
	add	esp, 16
	sub	esp, 8
	push	0
	push	16777215
	call	fb_set_color@PLT
	add	esp, 16
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	terminal_console, .-terminal_console
	.globl	terminal_output
	.type	terminal_output, @function
terminal_output:
.LFB5:
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
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	edx, DWORD PTR 16[ebp]
	mov	eax, DWORD PTR 16[ebp]
	sub	eax, DWORD PTR 12[ebp]
	sub	esp, 12
	push	0
	push	edx
	push	DWORD PTR 12[ebp]
	push	eax
	push	DWORD PTR 8[ebp]
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	terminal_output, .-terminal_output
	.globl	terminal_compare
	.type	terminal_compare, @function
terminal_compare:
.LFB6:
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
	push	DWORD PTR 16[ebp]
	push	DWORD PTR 12[ebp]
	push	DWORD PTR 8[ebp]
	mov	edx, DWORD PTR Terminal_Buffer@GOT[eax]
	push	edx
	mov	ebx, eax
	call	STR_Compare@PLT
	add	esp, 16
	cmp	DWORD PTR 20[ebp], eax
	jne	.L8
	mov	eax, 1
	jmp	.L9
.L8:
	mov	eax, 0
.L9:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	terminal_compare, .-terminal_compare
	.section	.rodata
.LC0:
	.string	"print"
.LC1:
	.string	"peek"
.LC2:
	.string	"poke"
.LC3:
	.string	"load"
.LC4:
	.string	"exec"
.LC5:
	.string	"wipe"
.LC6:
	.string	"check"
.LC7:
	.string	"swap_PROG"
.LC8:
	.string	"set_PROG"
.LC9:
	.string	"clear"
.LC10:
	.string	"pong"
.LC11:
	.string	"grapher"
.LC12:
	.string	"reboot"
.LC13:
	.string	"malloc"
.LC14:
	.string	"msize"
.LC15:
	.string	"free"
	.text
	.globl	terminal_interpret
	.type	terminal_interpret, @function
terminal_interpret:
.LFB7:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	sub	esp, 48
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -12[ebp], 0
	mov	DWORD PTR -16[ebp], 0
	jmp	.L11
.L14:
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	eax, DWORD PTR -16[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	cmp	al, 32
	je	.L12
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	eax, DWORD PTR -16[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	test	al, al
	jne	.L34
.L12:
	mov	eax, DWORD PTR -16[ebp]
	mov	ecx, eax
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	mov	BYTE PTR [eax], cl
	add	DWORD PTR -12[ebp], 1
.L34:
	add	DWORD PTR -16[ebp], 1
.L11:
	cmp	DWORD PTR -16[ebp], 74
	jle	.L14
	mov	eax, DWORD PTR -12[ebp]
	lea	edx, 1[eax]
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	mov	BYTE PTR [eax+edx], -1
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	5
	push	eax
	push	0
	lea	eax, .LC0@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L15
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	mov	ecx, eax
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	lea	edx, 1[eax]
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	mov	esi, 75
	sub	esi, eax
	mov	eax, esi
	sub	esp, 12
	push	0
	push	ecx
	push	edx
	push	eax
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L15:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC1@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L16
	sub	esp, 4
	push	77
	push	0
	push	79
	call	printChar@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -24[ebp], eax
	sub	esp, 12
	push	DWORD PTR -24[ebp]
	call	ReadMem@PLT
	add	esp, 16
	mov	DWORD PTR -28[ebp], eax
	push	0
	push	32
	push	DWORD PTR -28[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	0
	push	9
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L16:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC2@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L17
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -32[ebp], eax
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 2[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR 1[edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -36[ebp], eax
	sub	esp, 8
	push	DWORD PTR -36[ebp]
	push	DWORD PTR -32[ebp]
	call	WriteMem@PLT
	add	esp, 16
	push	0
	push	32
	push	DWORD PTR -32[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	push	9
	push	32
	push	DWORD PTR -36[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	mov	BYTE PTR 9[eax], 32
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	0
	push	18
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L17:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC3@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L18
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	esp, 8
	push	440
	push	eax
	call	WriteMem@PLT
	add	esp, 16
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 5
	sub	esp, 8
	push	61371
	push	eax
	call	WriteMem@PLT
	add	esp, 16
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 10
	sub	esp, 8
	push	21689
	push	eax
	call	WriteMem@PLT
	add	esp, 16
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 15
	sub	esp, 8
	push	32973
	push	eax
	call	WriteMem@PLT
	add	esp, 16
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, 17
	sub	esp, 8
	push	195
	push	eax
	call	WriteMem@PLT
	add	esp, 16
.L18:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC4@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L19
	call	PROGA@PLT
.L19:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC5@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L20
	mov	DWORD PTR -20[ebp], 0
	jmp	.L21
.L22:
	mov	eax, DWORD PTR -20[ebp]
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	sub	esp, 8
	push	0
	push	eax
	call	WriteMem@PLT
	add	esp, 16
	add	DWORD PTR -20[ebp], 1
.L21:
	cmp	DWORD PTR -20[ebp], 999
	jle	.L22
.L20:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	5
	push	eax
	push	0
	lea	eax, .LC6@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L23
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	0
	push	9
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L23:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	9
	push	eax
	push	0
	lea	eax, .LC7@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L24
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	eax, 16777216
	jne	.L25
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	DWORD PTR [eax], 33554432
	jmp	.L24
.L25:
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	eax, 33554432
	jne	.L24
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	DWORD PTR [eax], 16777216
.L24:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	8
	push	eax
	push	0
	lea	eax, .LC8@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L26
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -40[ebp], eax
	mov	edx, DWORD PTR -40[ebp]
	mov	eax, DWORD PTR externalProgram@GOT[ebx]
	mov	DWORD PTR [eax], edx
.L26:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	5
	push	eax
	push	0
	lea	eax, .LC9@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L27
	sub	esp, 12
	push	0
	call	fb_clear@PLT
	add	esp, 16
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], 0
.L27:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC10@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L28
	call	pong@PLT
.L28:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	7
	push	eax
	push	0
	lea	eax, .LC11@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L29
	call	grapher_entry@PLT
.L29:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	6
	push	eax
	push	0
	lea	eax, .LC12@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L30
	call	kreboot@PLT
.L30:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	6
	push	eax
	push	0
	lea	eax, .LC13@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L31
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -44[ebp], eax
	sub	esp, 12
	push	DWORD PTR -44[ebp]
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR -48[ebp], eax
	mov	eax, DWORD PTR -48[ebp]
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, DWORD PTR memory_used@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 10
	sub	esp, 12
	push	0
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L31:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	5
	push	eax
	push	0
	lea	eax, .LC14@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L32
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -52[ebp], eax
	mov	eax, DWORD PTR -52[ebp]
	sub	esp, 12
	push	eax
	call	mgetSize@PLT
	add	esp, 16
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L32:
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	push	4
	push	eax
	push	0
	lea	eax, .LC15@GOTOFF[ebx]
	push	eax
	call	terminal_compare
	add	esp, 16
	test	eax, eax
	je	.L35
	mov	eax, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	eax, BYTE PTR 1[eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Arguments@GOT[ebx]
	movzx	edx, BYTE PTR [edx]
	movsx	edx, dl
	add	edx, 1
	sub	esp, 4
	push	eax
	push	edx
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	encodeHex@PLT
	add	esp, 16
	mov	DWORD PTR -56[ebp], eax
	mov	eax, DWORD PTR -56[ebp]
	sub	esp, 12
	push	eax
	call	free@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	0
	push	16711680
	push	45
	push	eax
	call	fb_write_cell@PLT
	add	esp, 16
	mov	eax, DWORD PTR memory_used@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	add	eax, 1
	sub	esp, 12
	push	0
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
.L35:
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
.LFE7:
	.size	terminal_interpret, .-terminal_interpret
	.globl	terminal_enter
	.type	terminal_enter, @function
terminal_enter:
.LFB8:
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
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	push	eax
	push	75
	mov	eax, DWORD PTR Terminal_OUT_Buffer@GOT[ebx]
	push	eax
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	push	eax
	call	STR_INSERT@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	edx, eax
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	sub	esp, 12
	push	0
	push	edx
	push	eax
	push	75
	mov	eax, DWORD PTR Terminal_OUT_Buffer@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], eax
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], 0
	call	terminal_interpret
	mov	DWORD PTR -12[ebp], 0
	jmp	.L37
.L38:
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	mov	BYTE PTR [eax], 0
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	eax, 1
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	printChar@PLT
	add	esp, 16
	add	DWORD PTR -12[ebp], 1
.L37:
	cmp	DWORD PTR -12[ebp], 74
	jle	.L38
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	sub	esp, 8
	push	eax
	push	1
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	sub	esp, 8
	push	0
	push	65280
	call	fb_set_color@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	sub	esp, 4
	push	62
	push	eax
	push	0
	call	printChar@PLT
	add	esp, 16
	sub	esp, 8
	push	0
	push	16777215
	call	fb_set_color@PLT
	add	esp, 16
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	imul	edx, eax
	mov	eax, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	cmp	edx, eax
	jge	.L40
	mov	DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx], 0
	sub	esp, 12
	push	0
	call	fb_clear@PLT
	add	esp, 16
.L40:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	terminal_enter, .-terminal_enter
	.globl	terminal_handler
	.type	terminal_handler, @function
terminal_handler:
.LFB9:
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
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR previousASCII_pointer@GOTOFF[ebx]
	cmp	edx, eax
	je	.L42
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	cmp	ax, 74
	ja	.L42
	call	terminal_console
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR previousASCII_pointer@GOTOFF[ebx], eax
	jmp	.L57
.L42:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR previousKEY_pointer@GOTOFF[ebx]
	cmp	edx, eax
	je	.L57
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	lea	edx, -1[eax]
	mov	eax, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	movzx	eax, al
	cmp	eax, 205
	je	.L44
	cmp	eax, 205
	jg	.L45
	cmp	eax, 203
	je	.L46
	cmp	eax, 203
	jg	.L45
	cmp	eax, 200
	je	.L47
	cmp	eax, 200
	jg	.L45
	cmp	eax, 14
	je	.L48
	cmp	eax, 156
	je	.L49
	jmp	.L45
.L48:
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	test	ax, ax
	je	.L58
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	sub	esp, 8
	push	edx
	push	eax
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	lea	edx, -1[eax]
	mov	eax, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	BYTE PTR [eax+edx], 0
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	sub	esp, 4
	push	32
	push	edx
	push	eax
	call	printChar@PLT
	add	esp, 16
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	test	ax, ax
	je	.L58
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	sub	eax, 1
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], ax
	jmp	.L58
.L49:
	call	terminal_enter
	jmp	.L45
.L46:
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	test	ax, ax
	je	.L59
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	sub	esp, 8
	push	edx
	push	eax
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	sub	eax, 1
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], ax
	jmp	.L59
.L44:
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	cmp	ax, 74
	ja	.L60
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	add	eax, 1
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], ax
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	movzx	eax, WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx]
	movzx	eax, ax
	sub	esp, 8
	push	edx
	push	eax
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	jmp	.L60
.L47:
	mov	DWORD PTR -12[ebp], 0
	jmp	.L53
.L56:
	mov	edx, DWORD PTR Terminal_OUT_pointer@GOTOFF[ebx]
	mov	eax, DWORD PTR fb_terminal_w@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	edx, eax
	mov	eax, DWORD PTR Terminal_OUT_Buffer@GOT[ebx]
	movzx	eax, BYTE PTR [eax+edx]
	mov	ecx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	edx, DWORD PTR -12[ebp]
	add	edx, ecx
	mov	BYTE PTR [edx], al
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	movsx	eax, al
	mov	edx, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	ecx, edx
	mov	edx, DWORD PTR -12[ebp]
	add	edx, 1
	sub	esp, 4
	push	eax
	push	ecx
	push	edx
	call	printChar@PLT
	add	esp, 16
	mov	edx, DWORD PTR Terminal_Buffer@GOT[ebx]
	mov	eax, DWORD PTR -12[ebp]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	test	al, al
	jne	.L54
	mov	eax, DWORD PTR Terminal_Y@GOTOFF[ebx]
	mov	edx, eax
	mov	eax, DWORD PTR -12[ebp]
	add	eax, 1
	sub	esp, 8
	push	edx
	push	eax
	call	fb_move_cursor_xy@PLT
	add	esp, 16
	mov	eax, DWORD PTR -12[ebp]
	mov	WORD PTR Terminal_Buffer_Pointer@GOTOFF[ebx], ax
	nop
	jmp	.L45
.L54:
	add	DWORD PTR -12[ebp], 1
.L53:
	cmp	DWORD PTR -12[ebp], 74
	jle	.L56
	jmp	.L45
.L58:
	nop
	jmp	.L45
.L59:
	nop
	jmp	.L45
.L60:
	nop
.L45:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR previousKEY_pointer@GOTOFF[ebx], eax
.L57:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	terminal_handler, .-terminal_handler
	.globl	terminal_init
	.type	terminal_init, @function
terminal_init:
.LFB10:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR fb_height@GOT[eax]
	mov	edx, DWORD PTR [edx]
	shr	edx, 3
	sub	edx, 1
	mov	DWORD PTR Terminal_Y@GOTOFF[eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	terminal_init, .-terminal_init
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB11:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE11:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB12:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE12:
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
