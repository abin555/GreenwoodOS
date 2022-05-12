	.file	"PCI.c"
	.intel_syntax noprefix
	.text
	.comm	INT_Software_Value,4,4
	.comm	fb_width,4,4
	.comm	fb_height,4,4
	.comm	fb,4,4
	.comm	fb_backBuffer,8294400,32
	.comm	fb_terminal_w,4,4
	.comm	fb_terminal_h,4,4
	.comm	FG,4,4
	.comm	BG,4,4
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
	.globl	pci_devices
	.bss
	.align 4
	.type	pci_devices, @object
	.size	pci_devices, 4
pci_devices:
	.zero	4
	.globl	devs
	.align 4
	.type	devs, @object
	.size	devs, 4
devs:
	.zero	4
	.globl	pci_drivers
	.align 4
	.type	pci_drivers, @object
	.size	pci_drivers, 4
pci_drivers:
	.zero	4
	.globl	drivs
	.align 4
	.type	drivs, @object
	.size	drivs, 4
drivs:
	.zero	4
	.text
	.globl	add_pci_device
	.type	add_pci_device, @function
add_pci_device:
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
	mov	edx, DWORD PTR pci_devices@GOTOFF[eax]
	mov	ecx, DWORD PTR devs@GOTOFF[eax]
	sal	ecx, 2
	add	ecx, edx
	mov	edx, DWORD PTR 8[ebp]
	mov	DWORD PTR [ecx], edx
	mov	edx, DWORD PTR devs@GOTOFF[eax]
	add	edx, 1
	mov	DWORD PTR devs@GOTOFF[eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	add_pci_device, .-add_pci_device
	.globl	pci_read_word
	.type	pci_read_word, @function
pci_read_word:
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
	sub	esp, 124
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -124[ebp], eax
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 12[ebp]
	mov	ecx, DWORD PTR 16[ebp]
	mov	ebx, DWORD PTR 20[ebp]
	mov	WORD PTR -76[ebp], ax
	mov	eax, edx
	mov	WORD PTR -80[ebp], ax
	mov	eax, ecx
	mov	WORD PTR -84[ebp], ax
	mov	eax, ebx
	mov	WORD PTR -88[ebp], ax
	movzx	eax, WORD PTR -76[ebp]
	mov	DWORD PTR -32[ebp], eax
	mov	DWORD PTR -28[ebp], 0
	movzx	eax, WORD PTR -80[ebp]
	mov	DWORD PTR -40[ebp], eax
	mov	DWORD PTR -36[ebp], 0
	movzx	eax, WORD PTR -84[ebp]
	mov	DWORD PTR -48[ebp], eax
	mov	DWORD PTR -44[ebp], 0
	mov	WORD PTR -50[ebp], 0
	mov	eax, DWORD PTR -32[ebp]
	mov	edx, DWORD PTR -28[ebp]
	shld	edx, eax, 16
	sal	eax, 16
	mov	ecx, eax
	mov	ebx, edx
	mov	eax, DWORD PTR -40[ebp]
	mov	edx, DWORD PTR -36[ebp]
	shld	edx, eax, 11
	sal	eax, 11
	mov	DWORD PTR -96[ebp], eax
	mov	DWORD PTR -92[ebp], edx
	mov	edx, ecx
	or	edx, DWORD PTR -96[ebp]
	mov	esi, edx
	mov	ecx, ebx
	or	ecx, DWORD PTR -92[ebp]
	mov	edi, ecx
	mov	eax, DWORD PTR -48[ebp]
	mov	edx, DWORD PTR -44[ebp]
	shld	edx, eax, 8
	sal	eax, 8
	mov	ecx, esi
	or	ecx, eax
	mov	DWORD PTR -104[ebp], ecx
	or	edi, edx
	mov	DWORD PTR -100[ebp], edi
	movzx	eax, WORD PTR -88[ebp]
	mov	edx, 0
	mov	edi, eax
	and	edi, 252
	mov	DWORD PTR -112[ebp], edi
	mov	edi, edx
	and	edi, 0
	mov	DWORD PTR -108[ebp], edi
	mov	ecx, DWORD PTR -104[ebp]
	mov	ebx, DWORD PTR -100[ebp]
	mov	esi, ecx
	mov	eax, DWORD PTR -112[ebp]
	mov	edx, DWORD PTR -108[ebp]
	or	esi, eax
	mov	DWORD PTR -120[ebp], esi
	mov	edi, ebx
	or	edi, edx
	mov	DWORD PTR -116[ebp], edi
	mov	ebx, DWORD PTR -120[ebp]
	mov	esi, DWORD PTR -116[ebp]
	mov	eax, ebx
	or	eax, -2147483648
	mov	DWORD PTR -64[ebp], eax
	mov	eax, esi
	or	ah, 0
	mov	DWORD PTR -60[ebp], eax
	mov	eax, DWORD PTR -64[ebp]
	sub	esp, 8
	push	eax
	push	3320
	mov	ebx, DWORD PTR -124[ebp]
	call	outportl@PLT
	add	esp, 16
	sub	esp, 12
	push	3324
	call	inportl@PLT
	add	esp, 16
	movzx	edx, WORD PTR -88[ebp]
	and	edx, 2
	sal	edx, 3
	mov	ecx, edx
	shr	eax, cl
	mov	WORD PTR -50[ebp], ax
	movzx	eax, WORD PTR -50[ebp]
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
	.size	pci_read_word, .-pci_read_word
	.globl	getVendorID
	.type	getVendorID, @function
getVendorID:
.LFB2:
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
	mov	WORD PTR -28[ebp], cx
	mov	WORD PTR -32[ebp], dx
	mov	WORD PTR -36[ebp], ax
	movzx	ecx, WORD PTR -36[ebp]
	movzx	edx, WORD PTR -32[ebp]
	movzx	eax, WORD PTR -28[ebp]
	push	0
	push	ecx
	push	edx
	push	eax
	call	pci_read_word
	add	esp, 16
	movzx	eax, ax
	mov	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR -12[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	getVendorID, .-getVendorID
	.globl	getDeviceID
	.type	getDeviceID, @function
getDeviceID:
.LFB3:
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
	mov	WORD PTR -28[ebp], cx
	mov	WORD PTR -32[ebp], dx
	mov	WORD PTR -36[ebp], ax
	movzx	ecx, WORD PTR -36[ebp]
	movzx	edx, WORD PTR -32[ebp]
	movzx	eax, WORD PTR -28[ebp]
	push	2
	push	ecx
	push	edx
	push	eax
	call	pci_read_word
	add	esp, 16
	movzx	eax, ax
	mov	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR -12[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	getDeviceID, .-getDeviceID
	.section	.rodata
.LC0:
	.string	"vendor:"
.LC1:
	.string	" device: "
	.text
	.globl	pci_probe
	.type	pci_probe, @function
pci_probe:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	ebx
	sub	esp, 36
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -12[ebp], 0
	mov	DWORD PTR -16[ebp], 0
	jmp	.L10
.L17:
	mov	DWORD PTR -20[ebp], 0
	jmp	.L11
.L16:
	mov	DWORD PTR -24[ebp], 0
	jmp	.L12
.L15:
	mov	eax, DWORD PTR -24[ebp]
	movzx	ecx, ax
	mov	eax, DWORD PTR -20[ebp]
	movzx	edx, ax
	mov	eax, DWORD PTR -16[ebp]
	movzx	eax, ax
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	getVendorID
	add	esp, 16
	mov	WORD PTR -26[ebp], ax
	cmp	WORD PTR -26[ebp], -1
	je	.L18
	mov	eax, DWORD PTR -24[ebp]
	movzx	ecx, ax
	mov	eax, DWORD PTR -20[ebp]
	movzx	edx, ax
	mov	eax, DWORD PTR -16[ebp]
	movzx	eax, ax
	sub	esp, 4
	push	ecx
	push	edx
	push	eax
	call	getDeviceID
	add	esp, 16
	mov	WORD PTR -28[ebp], ax
	mov	eax, DWORD PTR -12[ebp]
	sub	esp, 12
	push	eax
	push	0
	push	0
	push	7
	lea	eax, .LC0@GOTOFF[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	movzx	eax, WORD PTR -26[ebp]
	push	0
	push	16
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR -12[ebp]
	sub	esp, 12
	push	eax
	push	8
	push	-1
	push	4
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, DWORD PTR -12[ebp]
	sub	esp, 12
	push	eax
	push	12
	push	0
	push	10
	lea	eax, .LC1@GOTOFF[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	movzx	eax, WORD PTR -28[ebp]
	push	0
	push	16
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR -12[ebp]
	sub	esp, 12
	push	eax
	push	22
	push	-1
	push	4
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	add	DWORD PTR -12[ebp], 1
	sub	esp, 12
	push	16
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR -32[ebp], eax
	movzx	edx, WORD PTR -26[ebp]
	mov	eax, DWORD PTR -32[ebp]
	mov	DWORD PTR [eax], edx
	movzx	edx, WORD PTR -28[ebp]
	mov	eax, DWORD PTR -32[ebp]
	mov	DWORD PTR 4[eax], edx
	mov	eax, DWORD PTR -32[ebp]
	mov	edx, DWORD PTR -24[ebp]
	mov	DWORD PTR 8[eax], edx
	mov	eax, DWORD PTR -32[ebp]
	mov	DWORD PTR 12[eax], 0
	sub	esp, 12
	push	DWORD PTR -32[ebp]
	call	add_pci_device
	add	esp, 16
	jmp	.L14
.L18:
	nop
.L14:
	add	DWORD PTR -24[ebp], 1
.L12:
	cmp	DWORD PTR -24[ebp], 7
	jbe	.L15
	add	DWORD PTR -20[ebp], 1
.L11:
	cmp	DWORD PTR -20[ebp], 31
	jbe	.L16
	add	DWORD PTR -16[ebp], 1
.L10:
	cmp	DWORD PTR -16[ebp], 255
	jbe	.L17
	nop
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	pci_probe, .-pci_probe
	.globl	pci_init
	.type	pci_init, @function
pci_init:
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
	mov	DWORD PTR drivs@GOTOFF[ebx], 0
	mov	eax, DWORD PTR drivs@GOTOFF[ebx]
	mov	DWORD PTR devs@GOTOFF[ebx], eax
	sub	esp, 12
	push	512
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR pci_devices@GOTOFF[ebx], eax
	sub	esp, 12
	push	640
	call	malloc@PLT
	add	esp, 16
	mov	DWORD PTR pci_drivers@GOTOFF[ebx], eax
	call	pci_probe
	nop
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	pci_init, .-pci_init
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB6:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE6:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB7:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE7:
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
