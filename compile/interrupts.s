	.file	"interrupts.c"
	.intel_syntax noprefix
	.text
	.comm	INT_Software_Value,4,4
	.globl	SYS_MODE
	.data
	.type	SYS_MODE, @object
	.size	SYS_MODE, 1
SYS_MODE:
	.byte	1
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
	.comm	idt_descriptors,2048,32
	.comm	idt,6,4
	.comm	BUFFER_COUNT,4,4
	.text
	.globl	interrupts_init_descriptor
	.type	interrupts_init_descriptor, @function
interrupts_init_descriptor:
.LFB2:
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
	mov	edx, DWORD PTR 12[ebp]
	shr	edx, 16
	mov	ebx, edx
	mov	edx, DWORD PTR idt_descriptors@GOT[eax]
	mov	ecx, DWORD PTR 8[ebp]
	mov	WORD PTR 6[edx+ecx*8], bx
	mov	edx, DWORD PTR 12[ebp]
	mov	ebx, edx
	mov	edx, DWORD PTR idt_descriptors@GOT[eax]
	mov	ecx, DWORD PTR 8[ebp]
	mov	WORD PTR [edx+ecx*8], bx
	mov	edx, DWORD PTR idt_descriptors@GOT[eax]
	mov	ecx, DWORD PTR 8[ebp]
	mov	WORD PTR 2[edx+ecx*8], 8
	mov	edx, DWORD PTR idt_descriptors@GOT[eax]
	mov	ecx, DWORD PTR 8[ebp]
	mov	BYTE PTR 4[edx+ecx*8], 0
	mov	eax, DWORD PTR idt_descriptors@GOT[eax]
	mov	edx, DWORD PTR 8[ebp]
	mov	BYTE PTR 5[eax+edx*8], -114
	nop
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	interrupts_init_descriptor, .-interrupts_init_descriptor
	.globl	interrupt_install_idt
	.type	interrupt_install_idt, @function
interrupt_install_idt:
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
	mov	eax, DWORD PTR int_handler_33@GOT[ebx]
	push	eax
	push	33
	call	interrupts_init_descriptor
	add	esp, 8
	mov	eax, DWORD PTR int_handler_34@GOT[ebx]
	push	eax
	push	34
	call	interrupts_init_descriptor
	add	esp, 8
	mov	eax, DWORD PTR int_handler_128@GOT[ebx]
	push	eax
	push	128
	call	interrupts_init_descriptor
	add	esp, 8
	mov	eax, DWORD PTR idt_descriptors@GOT[ebx]
	mov	edx, eax
	mov	eax, DWORD PTR idt@GOT[ebx]
	mov	DWORD PTR 2[eax], edx
	mov	eax, DWORD PTR idt@GOT[ebx]
	mov	WORD PTR [eax], 2048
	mov	eax, DWORD PTR idt@GOT[ebx]
	sub	esp, 12
	push	eax
	call	load_idt@PLT
	add	esp, 16
	sub	esp, 8
	push	40
	push	32
	call	pic_remap@PLT
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
	.size	interrupt_install_idt, .-interrupt_install_idt
	.globl	KERNEL_INTERRUPT
	.type	KERNEL_INTERRUPT, @function
KERNEL_INTERRUPT:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR INT_Software_Value@GOT[eax]
	mov	edx, DWORD PTR [edx]
	movzx	edx, BYTE PTR [edx]
	movzx	edx, dl
	cmp	edx, 4
	je	.L4
	cmp	edx, 4
	jg	.L9
	cmp	edx, 3
	je	.L6
	cmp	edx, 3
	jg	.L9
	cmp	edx, 1
	je	.L7
	cmp	edx, 2
	je	.L8
	jmp	.L9
.L7:
	mov	BYTE PTR SYS_MODE@GOTOFF[eax], 1
	jmp	.L5
.L8:
	mov	BYTE PTR SYS_MODE@GOTOFF[eax], 2
	jmp	.L5
.L6:
	mov	BYTE PTR SYS_MODE@GOTOFF[eax], 3
	jmp	.L5
.L4:
	mov	BYTE PTR SYS_MODE@GOTOFF[eax], 4
	nop
.L5:
.L9:
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	KERNEL_INTERRUPT, .-KERNEL_INTERRUPT
	.globl	SYS_CALL
	.type	SYS_CALL, @function
SYS_CALL:
.LFB5:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	cmp	edx, 6
	ja	.L19
	sal	edx, 2
	mov	edx, DWORD PTR .L13@GOTOFF[edx+eax]
	add	edx, eax
	notrack jmp	edx
	.section	.rodata
	.align 4
	.align 4
.L13:
	.long	.L19@GOTOFF
	.long	.L18@GOTOFF
	.long	.L17@GOTOFF
	.long	.L20@GOTOFF
	.long	.L15@GOTOFF
	.long	.L14@GOTOFF
	.long	.L12@GOTOFF
	.text
.L18:
	mov	edx, DWORD PTR BG@GOT[eax]
	mov	esi, DWORD PTR [edx]
	mov	edx, DWORD PTR FG@GOT[eax]
	mov	ebx, DWORD PTR [edx]
	mov	edx, DWORD PTR 16[ebp]
	movsx	ecx, dl
	mov	edx, DWORD PTR 12[ebp]
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ebx, eax
	call	fb_write_cell@PLT
	add	esp, 16
	jmp	.L11
.L17:
	mov	ecx, DWORD PTR 20[ebp]
	mov	edx, DWORD PTR 16[ebp]
	mov	ebx, DWORD PTR 12[ebp]
	sub	esp, 4
	push	ecx
	push	edx
	push	ebx
	mov	ebx, eax
	call	fb_write_start@PLT
	add	esp, 16
	jmp	.L11
.L15:
	mov	edx, DWORD PTR 12[ebp]
	sub	esp, 12
	push	edx
	mov	ebx, eax
	call	fb_move_cursor@PLT
	add	esp, 16
	jmp	.L11
.L14:
	mov	ebx, DWORD PTR 20[ebp]
	mov	ecx, DWORD PTR 16[ebp]
	mov	edx, DWORD PTR 12[ebp]
	sub	esp, 4
	push	ebx
	push	ecx
	push	edx
	mov	ebx, eax
	call	fb_setPixel@PLT
	add	esp, 16
	jmp	.L11
.L12:
	mov	ebx, eax
	call	restore_kernel@PLT
	jmp	.L11
.L20:
	nop
.L11:
.L19:
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
.LFE5:
	.size	SYS_CALL, .-SYS_CALL
	.globl	interrupt_handler
	.type	interrupt_handler, @function
interrupt_handler:
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
	cmp	DWORD PTR 40[ebp], 128
	je	.L22
	cmp	DWORD PTR 40[ebp], 128
	ja	.L27
	cmp	DWORD PTR 40[ebp], 33
	je	.L24
	cmp	DWORD PTR 40[ebp], 34
	je	.L25
	jmp	.L27
.L24:
	sub	esp, 12
	push	DWORD PTR 40[ebp]
	mov	ebx, eax
	call	keyboard_handle_interrupt@PLT
	add	esp, 16
	jmp	.L26
.L25:
	call	KERNEL_INTERRUPT
	jmp	.L26
.L22:
	push	DWORD PTR 36[ebp]
	push	DWORD PTR 32[ebp]
	push	DWORD PTR 28[ebp]
	push	DWORD PTR 24[ebp]
	push	DWORD PTR 20[ebp]
	push	DWORD PTR 16[ebp]
	push	DWORD PTR 12[ebp]
	push	DWORD PTR 8[ebp]
	call	SYS_CALL
	add	esp, 32
	jmp	.L26
.L27:
	nop
.L26:
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	interrupt_handler, .-interrupt_handler
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB7:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
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
