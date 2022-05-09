	.file	"memory.c"
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
	.globl	memory_used
	.bss
	.align 4
	.type	memory_used, @object
	.size	memory_used, 4
memory_used:
	.zero	4
	.globl	heap_begin
	.align 4
	.type	heap_begin, @object
	.size	heap_begin, 4
heap_begin:
	.zero	4
	.globl	heap_end
	.align 4
	.type	heap_end, @object
	.size	heap_end, 4
heap_end:
	.zero	4
	.globl	last_alloc
	.align 4
	.type	last_alloc, @object
	.size	last_alloc, 4
last_alloc:
	.zero	4
	.text
	.globl	memcpy
	.type	memcpy, @function
memcpy:
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
	jmp	.L2
.L3:
	mov	eax, DWORD PTR -4[ebp]
	lea	edx, 0[0+eax*4]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	mov	edx, DWORD PTR -4[ebp]
	lea	ecx, 0[0+edx*4]
	mov	edx, DWORD PTR 12[ebp]
	add	edx, ecx
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR [edx], eax
	add	DWORD PTR -4[ebp], 1
.L2:
	mov	eax, DWORD PTR -4[ebp]
	cmp	eax, DWORD PTR 16[ebp]
	jb	.L3
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	memcpy, .-memcpy
	.globl	memset
	.type	memset, @function
memset:
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
	mov	eax, DWORD PTR 8[ebp]
	mov	DWORD PTR -4[ebp], eax
	jmp	.L5
.L6:
	mov	eax, DWORD PTR -4[ebp]
	lea	edx, 1[eax]
	mov	DWORD PTR -4[ebp], edx
	mov	edx, DWORD PTR 12[ebp]
	mov	BYTE PTR [eax], dl
.L5:
	mov	eax, DWORD PTR 16[ebp]
	lea	edx, -1[eax]
	mov	DWORD PTR 16[ebp], edx
	test	eax, eax
	jne	.L6
	mov	eax, DWORD PTR 8[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	memset, .-memset
	.globl	mem_init
	.type	mem_init, @function
mem_init:
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
	mov	edx, DWORD PTR 8[ebp]
	add	edx, 4096
	mov	DWORD PTR last_alloc@GOTOFF[eax], edx
	mov	edx, DWORD PTR last_alloc@GOTOFF[eax]
	mov	DWORD PTR heap_begin@GOTOFF[eax], edx
	mov	edx, DWORD PTR heap_begin@GOTOFF[eax]
	add	edx, 50
	mov	DWORD PTR heap_end@GOTOFF[eax], edx
	mov	eax, DWORD PTR heap_begin@GOTOFF[eax]
	push	50
	push	0
	push	eax
	call	memset
	add	esp, 12
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	mem_init, .-mem_init
	.globl	malloc
	.type	malloc, @function
malloc:
.LFB3:
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
	cmp	DWORD PTR 8[ebp], 0
	jne	.L10
	mov	eax, 0
	jmp	.L11
.L10:
	mov	eax, DWORD PTR heap_begin@GOTOFF[ebx]
	mov	DWORD PTR -12[ebp], eax
	jmp	.L12
.L17:
	mov	eax, DWORD PTR -12[ebp]
	mov	DWORD PTR -16[ebp], eax
	mov	eax, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	je	.L19
	mov	eax, DWORD PTR -16[ebp]
	movzx	eax, BYTE PTR 4[eax]
	test	al, al
	je	.L15
	mov	eax, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR [eax]
	add	DWORD PTR -12[ebp], eax
	add	DWORD PTR -12[ebp], 8
	add	DWORD PTR -12[ebp], 4
	jmp	.L12
.L15:
	mov	eax, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR [eax]
	cmp	DWORD PTR 8[ebp], eax
	ja	.L16
	mov	eax, DWORD PTR -16[ebp]
	mov	BYTE PTR 4[eax], 1
	mov	eax, DWORD PTR -12[ebp]
	add	eax, 8
	push	DWORD PTR 8[ebp]
	push	0
	push	eax
	call	memset
	add	esp, 12
	mov	edx, DWORD PTR memory_used@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	add	eax, 8
	mov	DWORD PTR memory_used@GOTOFF[ebx], eax
	mov	eax, DWORD PTR -12[ebp]
	add	eax, 8
	jmp	.L11
.L16:
	mov	eax, DWORD PTR -16[ebp]
	mov	eax, DWORD PTR [eax]
	add	DWORD PTR -12[ebp], eax
	add	DWORD PTR -12[ebp], 8
	add	DWORD PTR -12[ebp], 4
.L12:
	mov	edx, DWORD PTR -12[ebp]
	mov	eax, DWORD PTR last_alloc@GOTOFF[ebx]
	cmp	edx, eax
	jb	.L17
	jmp	.L14
.L19:
	nop
.L14:
	mov	edx, DWORD PTR last_alloc@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	lea	edx, 8[eax]
	mov	eax, DWORD PTR heap_end@GOTOFF[ebx]
	cmp	edx, eax
	jb	.L18
	mov	DWORD PTR -39[ebp], 544503119
	mov	DWORD PTR -35[ebp], 1210082927
	mov	DWORD PTR -31[ebp], 542130501
	mov	DWORD PTR -27[ebp], 1869440365
	mov	WORD PTR -23[ebp], 31090
	mov	BYTE PTR -21[ebp], 0
	sub	esp, 12
	push	0
	push	0
	push	0
	push	19
	lea	eax, -39[ebp]
	push	eax
	call	fb_write_xy@PLT
	add	esp, 32
	mov	eax, 0
	jmp	.L11
.L18:
	mov	eax, DWORD PTR last_alloc@GOTOFF[ebx]
	mov	DWORD PTR -20[ebp], eax
	mov	eax, DWORD PTR -20[ebp]
	mov	BYTE PTR 4[eax], 1
	mov	eax, DWORD PTR -20[ebp]
	mov	edx, DWORD PTR 8[ebp]
	mov	DWORD PTR [eax], edx
	mov	edx, DWORD PTR last_alloc@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	mov	DWORD PTR last_alloc@GOTOFF[ebx], eax
	mov	eax, DWORD PTR last_alloc@GOTOFF[ebx]
	add	eax, 8
	mov	DWORD PTR last_alloc@GOTOFF[ebx], eax
	mov	eax, DWORD PTR last_alloc@GOTOFF[ebx]
	add	eax, 4
	mov	DWORD PTR last_alloc@GOTOFF[ebx], eax
	mov	edx, DWORD PTR memory_used@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	add	eax, 12
	mov	DWORD PTR memory_used@GOTOFF[ebx], eax
	mov	eax, DWORD PTR -20[ebp]
	add	eax, 8
	sub	esp, 4
	push	DWORD PTR 8[ebp]
	push	0
	push	eax
	call	memset
	add	esp, 16
	mov	eax, DWORD PTR -20[ebp]
	add	eax, 8
.L11:
	mov	ebx, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	malloc, .-malloc
	.globl	free
	.type	free, @function
free:
.LFB4:
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
	mov	edx, DWORD PTR 8[ebp]
	sub	edx, 8
	mov	DWORD PTR -4[ebp], edx
	mov	ecx, DWORD PTR memory_used@GOTOFF[eax]
	mov	edx, DWORD PTR -4[ebp]
	mov	edx, DWORD PTR [edx]
	sub	ecx, edx
	mov	edx, ecx
	sub	edx, 8
	mov	DWORD PTR memory_used@GOTOFF[eax], edx
	mov	eax, DWORD PTR -4[ebp]
	mov	BYTE PTR 4[eax], 0
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	free, .-free
	.globl	mgetSize
	.type	mgetSize, @function
mgetSize:
.LFB5:
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
	mov	eax, DWORD PTR 8[ebp]
	sub	eax, 8
	mov	DWORD PTR -4[ebp], eax
	mov	eax, DWORD PTR -4[ebp]
	mov	eax, DWORD PTR [eax]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	mgetSize, .-mgetSize
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
