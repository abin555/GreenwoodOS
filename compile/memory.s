	.file	"memory.c"
	.intel_syntax noprefix
	.text
	.globl	last_alloc
	.bss
	.align 4
	.type	last_alloc, @object
	.size	last_alloc, 4
last_alloc:
	.zero	4
	.globl	heap_end
	.align 4
	.type	heap_end, @object
	.size	heap_end, 4
heap_end:
	.zero	4
	.globl	heap_begin
	.align 4
	.type	heap_begin, @object
	.size	heap_begin, 4
heap_begin:
	.zero	4
	.globl	memory_used
	.align 4
	.type	memory_used, @object
	.size	memory_used, 4
memory_used:
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
	add	edx, 4194304
	mov	DWORD PTR heap_end@GOTOFF[eax], edx
	mov	ecx, DWORD PTR heap_end@GOTOFF[eax]
	mov	edx, DWORD PTR heap_begin@GOTOFF[eax]
	sub	ecx, edx
	mov	edx, ecx
	mov	eax, DWORD PTR heap_begin@GOTOFF[eax]
	push	edx
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
	sub	esp, 16
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	cmp	DWORD PTR 8[ebp], 0
	jne	.L10
	mov	eax, 0
	jmp	.L11
.L10:
	mov	eax, DWORD PTR heap_begin@GOTOFF[ebx]
	mov	DWORD PTR -8[ebp], eax
	jmp	.L12
.L17:
	mov	eax, DWORD PTR -8[ebp]
	mov	DWORD PTR -12[ebp], eax
	mov	eax, DWORD PTR -12[ebp]
	movzx	eax, BYTE PTR 1[eax]
	test	al, al
	je	.L18
	mov	eax, DWORD PTR -12[ebp]
	movzx	eax, BYTE PTR [eax]
	test	al, al
	je	.L15
	mov	eax, DWORD PTR -12[ebp]
	movzx	eax, BYTE PTR 1[eax]
	movzx	eax, al
	add	DWORD PTR -8[ebp], eax
	add	DWORD PTR -8[ebp], 2
	add	DWORD PTR -8[ebp], 4
	jmp	.L12
.L15:
	mov	eax, DWORD PTR -12[ebp]
	movzx	eax, BYTE PTR 1[eax]
	movzx	eax, al
	cmp	DWORD PTR 8[ebp], eax
	ja	.L16
	mov	eax, DWORD PTR -12[ebp]
	mov	BYTE PTR [eax], 1
	mov	eax, DWORD PTR -8[ebp]
	add	eax, 2
	push	DWORD PTR 8[ebp]
	push	0
	push	eax
	call	memset
	add	esp, 12
	mov	edx, DWORD PTR memory_used@GOTOFF[ebx]
	mov	eax, DWORD PTR 8[ebp]
	add	eax, edx
	add	eax, 2
	mov	DWORD PTR memory_used@GOTOFF[ebx], eax
	mov	eax, DWORD PTR -8[ebp]
	add	eax, 2
	jmp	.L11
.L16:
	mov	eax, DWORD PTR -12[ebp]
	movzx	eax, BYTE PTR 1[eax]
	movzx	eax, al
	add	DWORD PTR -8[ebp], eax
	add	DWORD PTR -8[ebp], 2
	add	DWORD PTR -8[ebp], 4
.L12:
	mov	edx, DWORD PTR -8[ebp]
	mov	eax, DWORD PTR last_alloc@GOTOFF[ebx]
	cmp	edx, eax
	jb	.L17
	jmp	.L14
.L18:
	nop
.L14:
	mov	eax, 0
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
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB4:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE4:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB5:
	.cfi_startproc
	mov	ebx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE5:
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
