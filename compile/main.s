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
	.comm	pci_devices,4,4
	.comm	pci_drivers,4,4
	.comm	devs,4,4
	.comm	drivs,4,4
	.comm	vga_width,4,4
	.comm	vga_height,4,4
	.comm	screen,4,4
	.globl	flyingDot
	.type	flyingDot, @function
flyingDot:
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
	mov	DWORD PTR -12[ebp], 50
	mov	DWORD PTR -16[ebp], 50
	mov	DWORD PTR -20[ebp], 3
	mov	DWORD PTR -24[ebp], 5
	mov	DWORD PTR -28[ebp], 255
.L25:
	mov	DWORD PTR -32[ebp], 0
	jmp	.L2
.L19:
	mov	eax, DWORD PTR -20[ebp]
	add	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR -24[ebp]
	add	DWORD PTR -16[ebp], eax
	mov	edx, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR -12[ebp]
	sub	esp, 4
	push	16777215
	push	edx
	push	eax
	call	fb_setPixel@PLT
	add	esp, 16
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 10
	cmp	DWORD PTR -12[ebp], eax
	jge	.L3
	cmp	DWORD PTR -12[ebp], 10
	jg	.L4
.L3:
	neg	DWORD PTR -20[ebp]
	cmp	DWORD PTR -20[ebp], 0
	jns	.L5
	sub	DWORD PTR -20[ebp], 1
.L5:
	cmp	DWORD PTR -20[ebp], 0
	jle	.L4
	add	DWORD PTR -20[ebp], 1
.L4:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	sub	eax, 10
	cmp	DWORD PTR -16[ebp], eax
	jge	.L6
	cmp	DWORD PTR -16[ebp], 10
	jg	.L7
.L6:
	neg	DWORD PTR -24[ebp]
	cmp	DWORD PTR -24[ebp], 0
	jns	.L8
	sub	DWORD PTR -24[ebp], 1
.L8:
	cmp	DWORD PTR -24[ebp], 0
	jle	.L7
	add	DWORD PTR -24[ebp], 1
.L7:
	cmp	DWORD PTR -20[ebp], 50
	jg	.L9
	cmp	DWORD PTR -20[ebp], -50
	jge	.L10
.L9:
	mov	DWORD PTR -20[ebp], 1
.L10:
	cmp	DWORD PTR -24[ebp], 50
	jg	.L11
	cmp	DWORD PTR -24[ebp], -50
	jge	.L12
.L11:
	mov	DWORD PTR -24[ebp], 1
.L12:
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -12[ebp], eax
	jg	.L13
	cmp	DWORD PTR -12[ebp], 0
	jns	.L14
.L13:
	mov	eax, DWORD PTR fb_width@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -12[ebp], eax
.L14:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR -16[ebp], eax
	jg	.L15
	cmp	DWORD PTR -16[ebp], 0
	jns	.L16
.L15:
	mov	eax, DWORD PTR fb_height@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	shr	eax
	mov	DWORD PTR -16[ebp], eax
.L16:
	mov	DWORD PTR -36[ebp], 0
	jmp	.L17
.L18:
	add	DWORD PTR -36[ebp], 1
.L17:
	cmp	DWORD PTR -36[ebp], 65534
	jbe	.L18
	add	DWORD PTR -32[ebp], 1
.L2:
	cmp	DWORD PTR -32[ebp], 254
	jle	.L19
	mov	DWORD PTR -40[ebp], 0
	jmp	.L20
.L24:
	mov	DWORD PTR -44[ebp], 0
	jmp	.L21
.L23:
	mov	eax, DWORD PTR -28[ebp]
	sal	eax, 16
	and	eax, 16711680
	mov	edx, eax
	mov	eax, DWORD PTR -28[ebp]
	sal	eax, 8
	movzx	eax, ax
	or	eax, edx
	or	eax, DWORD PTR -28[ebp]
	mov	ecx, eax
	mov	edx, DWORD PTR -44[ebp]
	mov	eax, DWORD PTR -40[ebp]
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	fb_setPixel@PLT
	add	esp, 16
	sub	DWORD PTR -28[ebp], 1
	cmp	DWORD PTR -28[ebp], 0
	jne	.L22
	mov	DWORD PTR -28[ebp], 255
.L22:
	add	DWORD PTR -44[ebp], 1
.L21:
	cmp	DWORD PTR -44[ebp], 499
	jle	.L23
	add	DWORD PTR -40[ebp], 1
.L20:
	cmp	DWORD PTR -40[ebp], 499
	jle	.L24
	jmp	.L25
	.cfi_endproc
.LFE0:
	.size	flyingDot, .-flyingDot
	.globl	kmain_loop
	.type	kmain_loop, @function
kmain_loop:
.LFB1:
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
.L27:
	call	terminal_handler@PLT
	jmp	.L27
	.cfi_endproc
.LFE1:
	.size	kmain_loop, .-kmain_loop
	.globl	kmain
	.type	kmain, @function
kmain:
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
	cmp	DWORD PTR 8[ebp], 920085129
	je	.L29
	mov	eax, 255
	jmp	.L30
.L29:
	mov	eax, DWORD PTR 12[ebp]
	add	eax, 8
	mov	DWORD PTR -12[ebp], eax
	jmp	.L31
.L33:
	mov	eax, DWORD PTR -12[ebp]
	mov	eax, DWORD PTR [eax]
	cmp	eax, 8
	jne	.L32
	mov	eax, DWORD PTR -12[ebp]
	mov	DWORD PTR -16[ebp], eax
	sub	esp, 12
	push	DWORD PTR -16[ebp]
	call	init_fb@PLT
	add	esp, 16
.L32:
	mov	eax, DWORD PTR -12[ebp]
	mov	eax, DWORD PTR 4[eax]
	add	eax, 7
	and	eax, -8
	add	DWORD PTR -12[ebp], eax
.L31:
	mov	eax, DWORD PTR -12[ebp]
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	jne	.L33
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
	call	activate_Drivers@PLT
	call	terminal_init@PLT
	call	kmain_loop
	mov	eax, 0
.L30:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	kmain, .-kmain
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB3:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE3:
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
