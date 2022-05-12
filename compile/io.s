	.file	"io.c"
	.intel_syntax noprefix
	.text
	.globl	INT_Software_Value
	.data
	.align 4
	.type	INT_Software_Value, @object
	.size	INT_Software_Value, 4
INT_Software_Value:
	.long	16
	.text
	.globl	software_interrupt
	.type	software_interrupt, @function
software_interrupt:
.LFB0:
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
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	edx, DWORD PTR 8[ebp]
	mov	BYTE PTR -12[ebp], dl
	mov	edx, DWORD PTR INT_Software_Value@GOTOFF[eax]
	movzx	ecx, BYTE PTR -12[ebp]
	mov	BYTE PTR [edx], cl
	mov	ebx, eax
	call	software_int@PLT
	nop
	add	esp, 20
	pop	ebx
	.cfi_restore 3
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	software_interrupt, .-software_interrupt
	.globl	WriteMem
	.type	WriteMem, @function
WriteMem:
.LFB1:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 8[ebp]
	mov	edx, DWORD PTR 12[ebp]
	mov	DWORD PTR [eax], edx
	nop
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	WriteMem, .-WriteMem
	.globl	ReadMem
	.type	ReadMem, @function
ReadMem:
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
	mov	eax, DWORD PTR 8[ebp]
	mov	eax, DWORD PTR [eax]
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	ReadMem, .-ReadMem
	.globl	outportl
	.type	outportl, @function
outportl:
.LFB3:
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
	mov	eax, DWORD PTR 8[ebp]
	mov	WORD PTR -4[ebp], ax
	movzx	edx, WORD PTR -4[ebp]
	mov	eax, DWORD PTR 12[ebp]
#APP
# 19 "io.c" 1
	out dx, eax
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	outportl, .-outportl
	.globl	inportl
	.type	inportl, @function
inportl:
.LFB4:
	.cfi_startproc
	endbr32
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 20
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	eax, DWORD PTR 8[ebp]
	mov	WORD PTR -20[ebp], ax
	movzx	eax, WORD PTR -20[ebp]
	mov	edx, eax
#APP
# 24 "io.c" 1
	in eax, dx
# 0 "" 2
#NO_APP
	mov	DWORD PTR -4[ebp], eax
	mov	eax, DWORD PTR -4[ebp]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	inportl, .-inportl
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB5:
	.cfi_startproc
	mov	eax, DWORD PTR [esp]
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
