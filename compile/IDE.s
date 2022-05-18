	.file	"IDE.c"
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
	.comm	usb_driverName,27,4
	.comm	pci_devices,4,4
	.comm	pci_drivers,4,4
	.comm	devs,4,4
	.comm	drivs,4,4
	.globl	ide_driverName
	.data
	.align 4
	.type	ide_driverName, @object
	.size	ide_driverName, 22
ide_driverName:
	.string	"IDE CONTROLLER DRIVER"
	.text
	.globl	ide_driver_install
	.type	ide_driver_install, @function
ide_driver_install:
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
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 1
	sub	esp, 12
	push	eax
	push	50
	push	0
	push	22
	lea	eax, ide_driverName@GOTOFF[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, DWORD PTR pci_drivers@GOT[ebx]
	mov	eax, DWORD PTR [eax]
	mov	edx, DWORD PTR 12[ebp]
	sal	edx, 2
	add	eax, edx
	mov	eax, DWORD PTR [eax]
	mov	eax, DWORD PTR 12[eax]
	mov	DWORD PTR -12[ebp], eax
	push	0
	push	32
	push	DWORD PTR -12[ebp]
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 1
	sub	esp, 12
	push	eax
	push	86
	push	1
	push	8
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, DWORD PTR 8[ebp]
	push	0
	push	8
	push	eax
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
	push	eax
	call	decodeHex@PLT
	add	esp, 16
	mov	eax, DWORD PTR 8[ebp]
	add	eax, 1
	sub	esp, 12
	push	eax
	push	73
	push	1
	push	2
	mov	eax, DWORD PTR STR_edit@GOT[ebx]
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
.LFE2:
	.size	ide_driver_install, .-ide_driver_install
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
