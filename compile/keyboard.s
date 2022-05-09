	.file	"keyboard.c"
	.intel_syntax noprefix
	.text
	.comm	INT_Software_Value,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.globl	KYBRD_SHIFT
	.bss
	.type	KYBRD_SHIFT, @object
	.size	KYBRD_SHIFT, 1
KYBRD_SHIFT:
	.zero	1
	.comm	KYBRD_CTRL,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.globl	keyboard_KEYBUFFER_POINTER
	.align 4
	.type	keyboard_KEYBUFFER_POINTER, @object
	.size	keyboard_KEYBUFFER_POINTER, 4
keyboard_KEYBUFFER_POINTER:
	.zero	4
	.globl	keyboard_ascii_pointer
	.align 4
	.type	keyboard_ascii_pointer, @object
	.size	keyboard_ascii_pointer, 4
keyboard_ascii_pointer:
	.zero	4
	.globl	prev_Scancode
	.type	prev_Scancode, @object
	.size	prev_Scancode, 1
prev_Scancode:
	.zero	1
	.comm	char_scancode,1,1
	.comm	memory_used,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
	.comm	kbd_US,256,32
	.comm	kbd_US_shift,256,32
	.comm	STR_edit,128,32
	.globl	_keyboard_disable
	.data
	.type	_keyboard_disable, @object
	.size	_keyboard_disable, 1
_keyboard_disable:
	.byte	1
	.text
	.globl	keyboard_ctrl_read_status
	.type	keyboard_ctrl_read_status, @function
keyboard_ctrl_read_status:
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
	sub	esp, 12
	push	100
	mov	ebx, eax
	call	inb@PLT
	add	esp, 16
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	keyboard_ctrl_read_status, .-keyboard_ctrl_read_status
	.globl	keyboard_ctrl_send_cmd
	.type	keyboard_ctrl_send_cmd, @function
keyboard_ctrl_send_cmd:
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
	mov	BYTE PTR -12[ebp], al
.L6:
	call	keyboard_ctrl_read_status
	movsx	eax, al
	and	eax, 2
	test	eax, eax
	je	.L8
	jmp	.L6
.L8:
	nop
	movzx	eax, BYTE PTR -12[ebp]
	movzx	eax, al
	sub	esp, 8
	push	eax
	push	100
	call	outb@PLT
	add	esp, 16
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	keyboard_ctrl_send_cmd, .-keyboard_ctrl_send_cmd
	.globl	keyboard_enc_read_buf
	.type	keyboard_enc_read_buf, @function
keyboard_enc_read_buf:
.LFB2:
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
	sub	esp, 12
	push	96
	mov	ebx, eax
	call	inb@PLT
	add	esp, 16
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	keyboard_enc_read_buf, .-keyboard_enc_read_buf
	.globl	keyboard_enc_send_cmd
	.type	keyboard_enc_send_cmd, @function
keyboard_enc_send_cmd:
.LFB3:
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
	mov	BYTE PTR -12[ebp], al
.L14:
	call	keyboard_ctrl_read_status
	movsx	eax, al
	and	eax, 2
	test	eax, eax
	je	.L16
	jmp	.L14
.L16:
	nop
	movzx	eax, BYTE PTR -12[ebp]
	movzx	eax, al
	sub	esp, 8
	push	eax
	push	96
	call	outb@PLT
	add	esp, 16
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	keyboard_enc_send_cmd, .-keyboard_enc_send_cmd
	.globl	keyboard_set_leds
	.type	keyboard_set_leds, @function
keyboard_set_leds:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 40
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	ecx, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 12[ebp]
	mov	eax, DWORD PTR 16[ebp]
	mov	BYTE PTR -28[ebp], cl
	mov	BYTE PTR -32[ebp], dl
	mov	BYTE PTR -36[ebp], al
	mov	BYTE PTR -9[ebp], 0
	cmp	BYTE PTR -36[ebp], 0
	je	.L18
	movzx	eax, BYTE PTR -9[ebp]
	or	eax, 1
	jmp	.L19
.L18:
	movzx	eax, BYTE PTR -9[ebp]
	and	eax, 1
.L19:
	mov	BYTE PTR -9[ebp], al
	cmp	BYTE PTR -28[ebp], 0
	je	.L20
	movzx	eax, BYTE PTR -28[ebp]
	or	eax, 2
	jmp	.L21
.L20:
	movzx	eax, BYTE PTR -28[ebp]
	and	eax, 2
.L21:
	mov	BYTE PTR -9[ebp], al
	cmp	BYTE PTR -32[ebp], 0
	je	.L22
	movzx	eax, BYTE PTR -28[ebp]
	or	eax, 4
	jmp	.L23
.L22:
	movzx	eax, BYTE PTR -28[ebp]
	and	eax, 4
.L23:
	mov	BYTE PTR -9[ebp], al
	sub	esp, 12
	push	-19
	call	keyboard_enc_send_cmd
	add	esp, 16
	movsx	eax, BYTE PTR -9[ebp]
	sub	esp, 12
	push	eax
	call	keyboard_enc_send_cmd
	add	esp, 16
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	keyboard_set_leds, .-keyboard_set_leds
	.globl	keyboard_self_test
	.type	keyboard_self_test, @function
keyboard_self_test:
.LFB5:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 8
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 12
	push	-86
	call	keyboard_ctrl_send_cmd
	add	esp, 16
.L27:
	call	keyboard_ctrl_read_status
	movsx	eax, al
	and	eax, 1
	test	eax, eax
	jne	.L30
	jmp	.L27
.L30:
	nop
	call	keyboard_enc_read_buf
	cmp	al, 85
	sete	al
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	keyboard_self_test, .-keyboard_self_test
	.globl	keyboard_disable
	.type	keyboard_disable, @function
keyboard_disable:
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
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 12
	push	-83
	call	keyboard_ctrl_send_cmd
	add	esp, 16
	mov	BYTE PTR _keyboard_disable@GOTOFF[ebx], 1
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	keyboard_disable, .-keyboard_disable
	.globl	keyboard_enable
	.type	keyboard_enable, @function
keyboard_enable:
.LFB7:
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
	push	-82
	call	keyboard_ctrl_send_cmd
	add	esp, 16
	mov	BYTE PTR _keyboard_disable@GOTOFF[ebx], 0
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	keyboard_enable, .-keyboard_enable
	.globl	keyboard_keyread
	.type	keyboard_keyread, @function
keyboard_keyread:
.LFB8:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 8
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 12
	push	-12
	call	keyboard_enc_send_cmd
	add	esp, 16
.L36:
	call	keyboard_ctrl_read_status
	movsx	eax, al
	and	eax, 1
	test	eax, eax
	jne	.L39
	jmp	.L36
.L39:
	nop
	call	keyboard_enc_read_buf
	movsx	eax, al
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	keyboard_keyread, .-keyboard_keyread
	.globl	keyboard_flag_handler
	.type	keyboard_flag_handler, @function
keyboard_flag_handler:
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
	call	__x86.get_pc_thunk.dx
	add	edx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 8[ebp]
	mov	BYTE PTR -12[ebp], al
	movzx	eax, BYTE PTR -12[ebp]
	cmp	eax, 211
	je	.L48
	cmp	eax, 211
	jg	.L49
	cmp	eax, 170
	je	.L43
	cmp	eax, 170
	jg	.L49
	cmp	eax, 157
	je	.L44
	cmp	eax, 157
	jg	.L49
	cmp	eax, 91
	je	.L50
	cmp	eax, 91
	jg	.L49
	cmp	eax, 46
	je	.L46
	cmp	eax, 46
	jg	.L49
	cmp	eax, 29
	je	.L47
	cmp	eax, 42
	jne	.L49
	mov	BYTE PTR KYBRD_SHIFT@GOTOFF[edx], 1
	sub	esp, 4
	push	0
	push	1
	push	0
	call	keyboard_set_leds
	add	esp, 16
	jmp	.L42
.L43:
	mov	BYTE PTR KYBRD_SHIFT@GOTOFF[edx], 0
	sub	esp, 4
	push	0
	push	0
	push	0
	call	keyboard_set_leds
	add	esp, 16
	jmp	.L42
.L47:
	mov	eax, DWORD PTR KYBRD_CTRL@GOT[edx]
	mov	BYTE PTR [eax], 1
	jmp	.L42
.L44:
	mov	eax, DWORD PTR KYBRD_CTRL@GOT[edx]
	mov	BYTE PTR [eax], 0
	jmp	.L42
.L46:
	mov	ebx, edx
	call	restore_kernel@PLT
	jmp	.L42
.L48:
	nop
	jmp	.L49
.L50:
	nop
.L42:
.L49:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	keyboard_flag_handler, .-keyboard_flag_handler
	.globl	convertascii
	.type	convertascii, @function
convertascii:
.LFB10:
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
	movzx	edx, BYTE PTR -4[ebp]
	mov	ecx, DWORD PTR kbd_US@GOT[eax]
	movzx	edx, BYTE PTR [ecx+edx]
	test	dl, dl
	je	.L52
	movzx	edx, BYTE PTR KYBRD_SHIFT@GOTOFF[eax]
	test	dl, dl
	je	.L53
	movzx	edx, BYTE PTR -4[ebp]
	mov	eax, DWORD PTR kbd_US_shift@GOT[eax]
	movzx	eax, BYTE PTR [eax+edx]
	jmp	.L54
.L53:
	movzx	edx, BYTE PTR -4[ebp]
	mov	eax, DWORD PTR kbd_US@GOT[eax]
	movzx	eax, BYTE PTR [eax+edx]
	jmp	.L54
.L52:
	mov	eax, 0
.L54:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	convertascii, .-convertascii
	.globl	keyboard_handle_interrupt
	.type	keyboard_handle_interrupt, @function
keyboard_handle_interrupt:
.LFB11:
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
	call	keyboard_enc_read_buf
	mov	BYTE PTR -9[ebp], al
	sub	esp, 12
	push	DWORD PTR 8[ebp]
	call	pic_acknowledge@PLT
	add	esp, 16
	cmp	BYTE PTR -9[ebp], 0
	je	.L56
	movzx	eax, BYTE PTR -9[ebp]
	push	0
	push	8
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	sub	esp, 12
	push	0
	push	0
	push	0
	push	3
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	movzx	eax, BYTE PTR -9[ebp]
	mov	edx, DWORD PTR kbd_US@GOT[ebx]
	movzx	eax, BYTE PTR [edx+eax]
	test	al, al
	je	.L57
	mov	eax, DWORD PTR KYBRD_CTRL@GOT[ebx]
	movzx	eax, BYTE PTR [eax]
	xor	eax, 1
	test	al, al
	je	.L57
	movzx	eax, BYTE PTR KYBRD_SHIFT@GOTOFF[ebx]
	test	al, al
	je	.L58
	movzx	edx, BYTE PTR -9[ebp]
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx]
	mov	ecx, DWORD PTR kbd_US_shift@GOT[ebx]
	movzx	ecx, BYTE PTR [ecx+edx]
	mov	edx, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	mov	BYTE PTR [edx+eax], cl
	jmp	.L59
.L58:
	movzx	edx, BYTE PTR -9[ebp]
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx]
	mov	ecx, DWORD PTR kbd_US@GOT[ebx]
	movzx	ecx, BYTE PTR [ecx+edx]
	mov	edx, DWORD PTR keyboard_ASCIIBuffer@GOT[ebx]
	mov	BYTE PTR [edx+eax], cl
.L59:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx]
	add	eax, 1
	mov	DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx], eax
	jmp	.L60
.L57:
	movzx	eax, BYTE PTR -9[ebp]
	sub	esp, 12
	push	eax
	call	keyboard_flag_handler
	add	esp, 16
.L60:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOTOFF[ebx]
	mov	edx, DWORD PTR keyboard_KEYBUFFER@GOT[ebx]
	movzx	ecx, BYTE PTR -9[ebp]
	mov	BYTE PTR [edx+eax], cl
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOTOFF[ebx]
	add	eax, 1
	mov	DWORD PTR keyboard_KEYBUFFER_POINTER@GOTOFF[ebx], eax
.L56:
	mov	eax, DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx]
	cmp	eax, 254
	jbe	.L61
	mov	DWORD PTR keyboard_ascii_pointer@GOTOFF[ebx], 0
.L61:
	mov	eax, DWORD PTR keyboard_KEYBUFFER_POINTER@GOTOFF[ebx]
	cmp	eax, 254
	jbe	.L63
	mov	DWORD PTR keyboard_KEYBUFFER_POINTER@GOTOFF[ebx], 0
.L63:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	keyboard_handle_interrupt, .-keyboard_handle_interrupt
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB12:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE12:
	.section	.text.__x86.get_pc_thunk.dx,"axG",@progbits,__x86.get_pc_thunk.dx,comdat
	.globl	__x86.get_pc_thunk.dx
	.hidden	__x86.get_pc_thunk.dx
	.type	__x86.get_pc_thunk.dx, @function
__x86.get_pc_thunk.dx:
.LFB13:
	.cfi_startproc
	mov	edx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE13:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB14:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE14:
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
