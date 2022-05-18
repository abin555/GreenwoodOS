	.file	"main.c"
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
	.comm	SYS_MODE,1,1
	.comm	decode,500,32
	.comm	usb_driverName,27,4
	.comm	ide_driverName,22,4
	.comm	pci_devices,4,4
	.comm	pci_drivers,4,4
	.comm	devs,4,4
	.comm	drivs,4,4
	.globl	kmain_loop
	.type	kmain_loop, @function
kmain_loop:
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
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
.L2:
	call	terminal_handler@PLT
	jmp	.L2
	.cfi_endproc
.LFE2:
	.size	kmain_loop, .-kmain_loop
	.globl	kmain
	.type	kmain, @function
kmain:
.LFB3:
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
	sub	esp, 44
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 920085129
	je	.L4
	mov	eax, 255
	jmp	.L5
.L4:
	mov	eax, DWORD PTR 12[ebp]
	add	eax, 8
	mov	DWORD PTR -28[ebp], eax
	jmp	.L6
.L8:
	mov	eax, DWORD PTR -28[ebp]
	mov	eax, DWORD PTR [eax]
	cmp	eax, 8
	jne	.L7
	mov	eax, DWORD PTR -28[ebp]
	mov	DWORD PTR -56[ebp], eax
	sub	esp, 12
	push	DWORD PTR -56[ebp]
	call	init_fb@PLT
	add	esp, 16
.L7:
	mov	eax, DWORD PTR -28[ebp]
	mov	eax, DWORD PTR 4[eax]
	add	eax, 7
	and	eax, -8
	add	DWORD PTR -28[ebp], eax
.L6:
	mov	eax, DWORD PTR -28[ebp]
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	jne	.L8
	call	load_gdt@PLT
	call	interrupt_install_idt@PLT
	sub	esp, 8
	push	0
	push	16777215
	call	fb_set_color@PLT
	add	esp, 16
	mov	eax, DWORD PTR restore_kernel_addr@GOT[ebx]
	lea	edx, kmain_loop@GOTOFF[ebx]
	mov	DWORD PTR [eax], edx
	sub	esp, 12
	push	268435456
	call	mem_init@PLT
	add	esp, 16
	call	pci_init@PLT
	mov	DWORD PTR -32[ebp], 0
	jmp	.L9
.L12:
	mov	DWORD PTR -36[ebp], 0
	jmp	.L10
.L11:
	mov	eax, DWORD PTR -36[ebp]
	movzx	esi, ax
	mov	eax, DWORD PTR pci_devices@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR -32[ebp]
	sal	edx, 2
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	movzx	eax, WORD PTR 4[eax]
	movzx	ecx, ax
	mov	eax, DWORD PTR pci_devices@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR -32[ebp]
	sal	edx, 2
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	mov	eax, DWORD PTR 16[eax]
	movzx	edx, ax
	mov	eax, DWORD PTR pci_devices@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edi, DWORD PTR -32[ebp]
	sal	edi, 2
	add	eax, edi
	mov	eax, DWORD PTR [eax]
	mov	eax, DWORD PTR 12[eax]
	movzx	eax, ax
	push	esi
	push	ecx
	push	edx
	push	eax
	call	pci_read_dword@PLT
	add	esp, 16
	mov	DWORD PTR -52[ebp], eax
	push	0
	push	32
	push	DWORD PTR -52[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR -36[ebp]
	lea	edx, 3[eax]
	test	eax, eax
	cmovs	eax, edx
	sar	eax, 2
	mov	edx, eax
	mov	eax, DWORD PTR devs@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	add	eax, edx
	lea	ecx, 1[eax]
	mov	edx, DWORD PTR -32[ebp]
	mov	eax, edx
	sal	eax, 3
	add	eax, edx
	sub	esp, 12
	push	ecx
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	add	DWORD PTR -36[ebp], 4
.L10:
	cmp	DWORD PTR -36[ebp], 79
	jle	.L11
	add	DWORD PTR -32[ebp], 1
.L9:
	mov	eax, DWORD PTR devs@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -32[ebp], eax
	jb	.L12
	mov	DWORD PTR -40[ebp], 0
	jmp	.L13
.L16:
	mov	eax, DWORD PTR pci_drivers@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR -40[ebp]
	sal	edx, 2
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	mov	eax, DWORD PTR 8[eax]
	movzx	eax, WORD PTR 6[eax]
	movzx	eax, ax
	push	0
	push	32
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	edx, DWORD PTR -40[ebp]
	mov	eax, edx
	sal	eax, 3
	add	eax, edx
	sub	esp, 12
	push	30
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	DWORD PTR -44[ebp], 0
	jmp	.L14
.L15:
	mov	eax, DWORD PTR pci_drivers@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR -40[ebp]
	sal	edx, 2
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR 12[eax+edx*4]
	mov	DWORD PTR -48[ebp], eax
	push	0
	push	32
	push	DWORD PTR -48[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR -44[ebp]
	add	eax, 31
	mov	ecx, eax
	mov	edx, DWORD PTR -40[ebp]
	mov	eax, edx
	sal	eax, 3
	add	eax, edx
	sub	esp, 12
	push	ecx
	push	eax
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	add	DWORD PTR -44[ebp], 1
.L14:
	cmp	DWORD PTR -44[ebp], 4
	jle	.L15
	add	DWORD PTR -40[ebp], 1
.L13:
	mov	eax, DWORD PTR drivs@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -40[ebp], eax
	jb	.L16
	call	activate_Drivers@PLT
	call	terminal_init@PLT
	call	kmain_loop
	mov	eax, 0
.L5:
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
.LFE3:
	.size	kmain, .-kmain
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
