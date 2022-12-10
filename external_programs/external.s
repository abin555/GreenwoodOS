	.file	"external.c"
	.intel_syntax noprefix
	.text
	.globl	string
	.data
	.align 4
	.type	string, @object
	.size	string, 15
string:
	.string	"Test String!\n"
	.string	""
	.text
	.globl	main
	.type	main, @function
main:
	lea	ecx, 4[esp]
	and	esp, -16
	push	DWORD PTR -4[ecx]
	push	ebp
	mov	ebp, esp
	push	ebx
	push	ecx
	call	__x86.get_pc_thunk.bx
	add	ebx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	sub	esp, 12
	push	5
	call	test
	add	esp, 16
	sub	esp, 12
	lea	eax, string@GOTOFF[ebx]
	push	eax
	call	print
	add	esp, 16
	mov	eax, 2
	lea	esp, -8[ebp]
	pop	ecx
	pop	ebx
	pop	ebp
	lea	esp, -4[ecx]
	ret
	.size	main, .-main
	.globl	test
	.type	test, @function
test:
	push	ebp
	mov	ebp, esp
	sub	esp, 16
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	DWORD PTR -4[ebp], 2
	mov	eax, DWORD PTR 8[ebp]
	add	DWORD PTR -4[ebp], eax
	mov	eax, DWORD PTR -4[ebp]
	leave
	ret
	.size	test, .-test
	.globl	print
	.type	print, @function
print:
	push	ebp
	mov	ebp, esp
	call	__x86.get_pc_thunk.ax
	add	eax, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
#APP
# 18 "external.c" 1
	mov ebx, eax
# 0 "" 2
#NO_APP
	mov	DWORD PTR 8[ebp], eax
#APP
# 19 "external.c" 1
	mov eax, 3
# 0 "" 2
# 20 "external.c" 1
	int 0x80
# 0 "" 2
#NO_APP
	nop
	pop	ebp
	ret
	.size	print, .-print
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
	mov	eax, DWORD PTR [esp]
	ret
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
	mov	ebx, DWORD PTR [esp]
	ret
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
