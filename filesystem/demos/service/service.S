	.file	"service.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/aidanb/Greenwood-OS/filesystem/demos/service" "service.c"
	.globl	serial
	.section	.bss
	.align 4
	.type	serial, @object
	.size	serial, 4
serial:
	.zero	4
	.section	.rodata
.LC0:
	.string	"Fibonacci Series: %d, %d, "
.LC1:
	.string	"%d, "
	.text
	.globl	fib
	.type	fib, @function
fib:
.LFB0:
	.file 1 "service.c"
	.loc 1 11 16
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	.loc 1 15 9
	movl	$0, -24(%ebp)
	.loc 1 15 17
	movl	$1, -16(%ebp)
	.loc 1 18 9
	movl	-24(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -20(%ebp)
	.loc 1 21 5
	movl	serial@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	-16(%ebp)
	pushl	-24(%ebp)
	leal	.LC0@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	.loc 1 24 12
	movl	$3, -12(%ebp)
	.loc 1 24 5
	jmp	.L2
.L3:
	.loc 1 25 9
	movl	serial@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	-20(%ebp)
	leal	.LC1@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	%eax
	call	fprintf@PLT
	addl	$16, %esp
	.loc 1 26 12
	movl	-16(%ebp), %eax
	movl	%eax, -24(%ebp)
	.loc 1 27 12
	movl	-20(%ebp), %eax
	movl	%eax, -16(%ebp)
	.loc 1 28 18
	movl	-24(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -20(%ebp)
	.loc 1 24 25 discriminator 3
	incl	-12(%ebp)
.L2:
	.loc 1 24 19 discriminator 1
	movl	-12(%ebp), %eax
	cmpl	8(%ebp), %eax
	jle	.L3
	.loc 1 30 5
	movl	serial@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$10
	call	fputc@PLT
	addl	$16, %esp
	.loc 1 31 1
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	fib, .-fib
	.globl	service_file_buf
	.section	.bss
	.align 4
	.type	service_file_buf, @object
	.size	service_file_buf, 4
service_file_buf:
	.zero	4
	.globl	num_reads
	.align 4
	.type	num_reads, @object
	.size	num_reads, 4
num_reads:
	.zero	4
	.globl	was_read
	.align 4
	.type	was_read, @object
	.size	was_read, 4
was_read:
	.zero	4
	.text
	.globl	read_notification
	.type	read_notification, @function
read_notification:
.LFB1:
	.loc 1 40 70
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	.loc 1 41 14
	movl	was_read@GOT(%eax), %eax
	movl	$1, (%eax)
	.loc 1 42 1
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	read_notification, .-read_notification
	.section	.rodata
	.align 4
.LC2:
	.string	"Hello from Service! Fib is at 0x%x\n"
.LC3:
	.string	"w"
.LC4:
	.string	"/-/dev/serial"
.LC5:
	.string	"r"
.LC6:
	.string	"/-/fsCTRL"
.LC7:
	.string	"Cannot open file!"
.LC8:
	.string	"Read %d times!"
.LC9:
	.string	"file"
.LC10:
	.string	"Serv"
.LC11:
	.string	"was_read @ 0x%x\n"
.LC12:
	.string	"[SERVICE] READ!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.loc 1 44 11
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	subl	$48, %esp
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	.loc 1 45 5
	subl	$8, %esp
	movl	fib@GOT(%ebx), %eax
	pushl	%eax
	leal	.LC2@GOTOFF(%ebx), %eax
	pushl	%eax
	call	printf@PLT
	addl	$16, %esp
	.loc 1 46 14
	subl	$8, %esp
	leal	.LC3@GOTOFF(%ebx), %eax
	pushl	%eax
	leal	.LC4@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fopen@PLT
	addl	$16, %esp
	.loc 1 46 12 discriminator 1
	movl	serial@GOT(%ebx), %edx
	movl	%eax, (%edx)
	.loc 1 47 5
	subl	$12, %esp
	pushl	$10
	call	fib@PLT
	addl	$16, %esp
	.loc 1 49 22
	subl	$8, %esp
	leal	.LC5@GOTOFF(%ebx), %eax
	pushl	%eax
	leal	.LC6@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fopen@PLT
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	.loc 1 50 7
	cmpl	$0, -12(%ebp)
	jne	.L6
	.loc 1 51 9
	subl	$12, %esp
	leal	.LC7@GOTOFF(%ebx), %eax
	pushl	%eax
	call	puts@PLT
	addl	$16, %esp
	.loc 1 52 16
	movl	$1, %eax
	jmp	.L10
.L6:
	.loc 1 55 5
	pushl	-12(%ebp)
	pushl	$1
	pushl	$24
	leal	-48(%ebp), %eax
	pushl	%eax
	call	fread@PLT
	addl	$16, %esp
	.loc 1 56 5
	subl	$12, %esp
	pushl	-12(%ebp)
	call	fclose@PLT
	addl	$16, %esp
	.loc 1 58 24
	subl	$12, %esp
	pushl	$1000
	call	malloc@PLT
	addl	$16, %esp
	movl	%eax, %edx
	.loc 1 58 22 discriminator 1
	movl	service_file_buf@GOT(%ebx), %eax
	movl	%edx, (%eax)
	.loc 1 59 15
	movl	num_reads@GOT(%ebx), %eax
	movl	$0, (%eax)
	.loc 1 60 14
	movl	was_read@GOT(%ebx), %eax
	movl	$0, (%eax)
	.loc 1 62 5
	movl	service_file_buf@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	$1000
	pushl	$0
	pushl	%eax
	call	memset@PLT
	addl	$16, %esp
	.loc 1 63 5
	movl	num_reads@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	service_file_buf@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%edx
	leal	.LC8@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	$1000
	pushl	%eax
	call	snprintf@PLT
	addl	$16, %esp
	.loc 1 65 22
	movl	-40(%ebp), %eax
	.loc 1 65 18
	movl	service_file_buf@GOT(%ebx), %edx
	movl	(%edx), %edx
	subl	$4, %esp
	pushl	$2
	pushl	$1000
	pushl	%edx
	call	*%eax
.LVL0:
	addl	$16, %esp
	movl	%eax, -16(%ebp)
	.loc 1 71 9
	movl	-32(%ebp), %eax
	.loc 1 71 5
	subl	$12, %esp
	pushl	$0
	pushl	$0
	movl	read_notification@GOT(%ebx), %edx
	pushl	%edx
	pushl	$0
	pushl	-16(%ebp)
	call	*%eax
.LVL1:
	addl	$32, %esp
	.loc 1 77 22
	movl	-44(%ebp), %eax
	.loc 1 77 18
	subl	$8, %esp
	pushl	-16(%ebp)
	leal	.LC9@GOTOFF(%ebx), %edx
	pushl	%edx
	call	*%eax
.LVL2:
	addl	$16, %esp
	movl	%eax, -20(%ebp)
	.loc 1 80 21
	movl	-48(%ebp), %eax
	.loc 1 80 17
	subl	$12, %esp
	leal	.LC10@GOTOFF(%ebx), %edx
	pushl	%edx
	call	*%eax
.LVL3:
	addl	$16, %esp
	movl	%eax, -24(%ebp)
	.loc 1 81 9
	movl	-36(%ebp), %eax
	.loc 1 81 5
	movl	-28(%ebp), %edx
	subl	$8, %esp
	pushl	-24(%ebp)
	pushl	%edx
	call	*%eax
.LVL4:
	addl	$16, %esp
	.loc 1 83 9
	movl	-36(%ebp), %eax
	.loc 1 83 5
	subl	$8, %esp
	pushl	-20(%ebp)
	pushl	-24(%ebp)
	call	*%eax
.LVL5:
	addl	$16, %esp
	.loc 1 85 5
	subl	$8, %esp
	movl	was_read@GOT(%ebx), %eax
	pushl	%eax
	leal	.LC11@GOTOFF(%ebx), %eax
	pushl	%eax
	call	printf@PLT
	addl	$16, %esp
.L9:
	.loc 1 87 12
	movl	was_read@GOT(%ebx), %eax
	movl	(%eax), %eax
	.loc 1 87 11
	testl	%eax, %eax
	je	.L8
	.loc 1 88 22
	movl	was_read@GOT(%ebx), %eax
	movl	$0, (%eax)
	.loc 1 89 13
	movl	serial@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%eax
	pushl	$16
	pushl	$1
	leal	.LC12@GOTOFF(%ebx), %eax
	pushl	%eax
	call	fwrite@PLT
	addl	$16, %esp
	.loc 1 90 13
	movl	service_file_buf@GOT(%ebx), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	$1000
	pushl	$0
	pushl	%eax
	call	memset@PLT
	addl	$16, %esp
	.loc 1 91 13
	movl	num_reads@GOT(%ebx), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	movl	num_reads@GOT(%ebx), %eax
	movl	%edx, (%eax)
	movl	num_reads@GOT(%ebx), %eax
	movl	(%eax), %edx
	movl	service_file_buf@GOT(%ebx), %eax
	movl	(%eax), %eax
	pushl	%edx
	leal	.LC8@GOTOFF(%ebx), %edx
	pushl	%edx
	pushl	$1000
	pushl	%eax
	call	snprintf@PLT
	addl	$16, %esp
.L8:
	.loc 1 93 9
	call	yield@PLT
	.loc 1 87 11
	jmp	.L9
.L10:
	.loc 1 95 1
	leal	-8(%ebp), %esp
	popl	%ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB3:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE3:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB4:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE4:
	.text
.Letext0:
	.file 2 "/home/aidanb/Greenwood-OS/filesystem/usr/include/stdio.h"
	.file 3 "/home/aidanb/Greenwood-OS/filesystem/usr/include/sys/sysfs.h"
	.file 4 "/home/aidanb/Greenwood-OS/filesystem/usr/include/string.h"
	.file 5 "/home/aidanb/Greenwood-OS/filesystem/usr/include/stdlib.h"
	.file 6 "/home/aidanb/Greenwood-OS/filesystem/usr/include/sys/task.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x54c
	.value	0x5
	.byte	0x1
	.byte	0x4
	.long	.Ldebug_abbrev0
	.uleb128 0x13
	.long	.LASF63
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x14
	.long	.LASF2
	.uleb128 0xd
	.long	.LASF2
	.byte	0x2
	.byte	0x12
	.byte	0x15
	.long	0x26
	.uleb128 0xd
	.long	.LASF3
	.byte	0x2
	.byte	0x16
	.byte	0x1b
	.long	0x43
	.uleb128 0x4
	.byte	0x4
	.byte	0x7
	.long	.LASF4
	.uleb128 0x4
	.byte	0x1
	.byte	0x6
	.long	.LASF5
	.uleb128 0x15
	.long	0x4a
	.uleb128 0xf
	.long	0x6e
	.byte	0x4
	.byte	0xe
	.long	0x6e
	.uleb128 0x8
	.long	.LASF6
	.byte	0x1
	.uleb128 0x8
	.long	.LASF7
	.byte	0x2
	.byte	0
	.uleb128 0x4
	.byte	0x4
	.byte	0x7
	.long	.LASF8
	.uleb128 0xd
	.long	.LASF9
	.byte	0x3
	.byte	0x7
	.byte	0x3
	.long	0x56
	.uleb128 0xe
	.long	.LASF18
	.byte	0x20
	.byte	0x9
	.long	0xee
	.uleb128 0x2
	.long	.LASF10
	.byte	0xa
	.byte	0x9
	.long	0xee
	.byte	0
	.uleb128 0x16
	.string	"buf"
	.byte	0x3
	.byte	0xb
	.byte	0xb
	.long	0xf5
	.byte	0x4
	.uleb128 0x2
	.long	.LASF11
	.byte	0xc
	.byte	0x9
	.long	0xee
	.byte	0x8
	.uleb128 0x2
	.long	.LASF12
	.byte	0xd
	.byte	0xc
	.long	0x11b
	.byte	0xc
	.uleb128 0x2
	.long	.LASF13
	.byte	0xe
	.byte	0xc
	.long	0x11b
	.byte	0x10
	.uleb128 0x2
	.long	.LASF14
	.byte	0xf
	.byte	0xb
	.long	0x143
	.byte	0x14
	.uleb128 0x2
	.long	.LASF15
	.byte	0x10
	.byte	0xb
	.long	0x143
	.byte	0x18
	.uleb128 0x2
	.long	.LASF16
	.byte	0x11
	.byte	0x10
	.long	0x75
	.byte	0x1c
	.byte	0
	.uleb128 0x17
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.long	0x4a
	.uleb128 0x10
	.long	0x114
	.uleb128 0x1
	.long	0x114
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0x116
	.byte	0
	.uleb128 0x18
	.byte	0x4
	.uleb128 0x3
	.long	0xee
	.uleb128 0x3
	.long	0xfa
	.uleb128 0x6
	.long	0xee
	.long	0x143
	.uleb128 0x1
	.long	0x114
	.uleb128 0x1
	.long	0x114
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0x116
	.byte	0
	.uleb128 0x3
	.long	0x120
	.uleb128 0xf
	.long	0x6e
	.byte	0x16
	.byte	0xa
	.long	0x160
	.uleb128 0x8
	.long	.LASF17
	.byte	0
	.uleb128 0x8
	.long	.LASF18
	.byte	0x1
	.byte	0
	.uleb128 0x19
	.byte	0x54
	.byte	0x3
	.byte	0x1d
	.byte	0x9
	.long	0x182
	.uleb128 0x2
	.long	.LASF19
	.byte	0x1e
	.byte	0x21
	.long	0x182
	.byte	0
	.uleb128 0x2
	.long	.LASF20
	.byte	0x1f
	.byte	0x11
	.long	0xee
	.byte	0x50
	.byte	0
	.uleb128 0x11
	.long	0x191
	.long	0x191
	.uleb128 0x12
	.long	0x43
	.byte	0
	.uleb128 0x3
	.long	0x196
	.uleb128 0xe
	.long	.LASF21
	.byte	0x74
	.byte	0x15
	.long	0x1de
	.uleb128 0x2
	.long	.LASF22
	.byte	0x19
	.byte	0x7
	.long	0x148
	.byte	0
	.uleb128 0x2
	.long	.LASF23
	.byte	0x1a
	.byte	0xa
	.long	0x205
	.byte	0x4
	.uleb128 0x2
	.long	.LASF24
	.byte	0x1b
	.byte	0x9
	.long	0xee
	.byte	0x18
	.uleb128 0x2
	.long	.LASF25
	.byte	0x22
	.byte	0x7
	.long	0x1de
	.byte	0x1c
	.uleb128 0x2
	.long	.LASF26
	.byte	0x23
	.byte	0x19
	.long	0x191
	.byte	0x70
	.byte	0
	.uleb128 0x1a
	.byte	0x54
	.byte	0x3
	.byte	0x1c
	.byte	0x5
	.long	0x200
	.uleb128 0x1b
	.string	"dir"
	.byte	0x3
	.byte	0x20
	.byte	0xb
	.long	0x160
	.uleb128 0x1c
	.long	.LASF27
	.byte	0x3
	.byte	0x21
	.byte	0x1f
	.long	0x200
	.byte	0
	.uleb128 0x3
	.long	0x81
	.uleb128 0x11
	.long	0x4a
	.long	0x214
	.uleb128 0x12
	.long	0x43
	.byte	0
	.uleb128 0xe
	.long	.LASF28
	.byte	0x18
	.byte	0x26
	.long	0x268
	.uleb128 0x2
	.long	.LASF29
	.byte	0x27
	.byte	0x1b
	.long	0x277
	.byte	0
	.uleb128 0x2
	.long	.LASF30
	.byte	0x28
	.byte	0x1b
	.long	0x290
	.byte	0x4
	.uleb128 0x2
	.long	.LASF31
	.byte	0x29
	.byte	0x1d
	.long	0x2ae
	.byte	0x8
	.uleb128 0x2
	.long	.LASF32
	.byte	0x2a
	.byte	0xb
	.long	0x2c7
	.byte	0xc
	.uleb128 0x2
	.long	.LASF33
	.byte	0x2b
	.byte	0xc
	.long	0x2eb
	.byte	0x10
	.uleb128 0x2
	.long	.LASF34
	.byte	0x32
	.byte	0x19
	.long	0x191
	.byte	0x14
	.byte	0
	.uleb128 0x6
	.long	0x191
	.long	0x277
	.uleb128 0x1
	.long	0xf5
	.byte	0
	.uleb128 0x3
	.long	0x268
	.uleb128 0x6
	.long	0x191
	.long	0x290
	.uleb128 0x1
	.long	0xf5
	.uleb128 0x1
	.long	0x200
	.byte	0
	.uleb128 0x3
	.long	0x27c
	.uleb128 0x6
	.long	0x200
	.long	0x2ae
	.uleb128 0x1
	.long	0xf5
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0x75
	.byte	0
	.uleb128 0x3
	.long	0x295
	.uleb128 0x6
	.long	0xee
	.long	0x2c7
	.uleb128 0x1
	.long	0x191
	.uleb128 0x1
	.long	0x191
	.byte	0
	.uleb128 0x3
	.long	0x2b3
	.uleb128 0x10
	.long	0x2eb
	.uleb128 0x1
	.long	0x200
	.uleb128 0x1
	.long	0x11b
	.uleb128 0x1
	.long	0x11b
	.uleb128 0x1
	.long	0x143
	.uleb128 0x1
	.long	0x143
	.byte	0
	.uleb128 0x3
	.long	0x2cc
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.long	.LASF35
	.uleb128 0x4
	.byte	0x8
	.byte	0x5
	.long	.LASF36
	.uleb128 0x4
	.byte	0xc
	.byte	0x4
	.long	.LASF37
	.uleb128 0x4
	.byte	0x10
	.byte	0x4
	.long	.LASF38
	.uleb128 0x4
	.byte	0x2
	.byte	0x7
	.long	.LASF39
	.uleb128 0x9
	.long	.LASF40
	.byte	0x9
	.byte	0x7
	.long	0x324
	.uleb128 0x5
	.byte	0x3
	.long	serial
	.uleb128 0x3
	.long	0x2b
	.uleb128 0x9
	.long	.LASF41
	.byte	0x23
	.byte	0x7
	.long	0xf5
	.uleb128 0x5
	.byte	0x3
	.long	service_file_buf
	.uleb128 0x9
	.long	.LASF42
	.byte	0x24
	.byte	0x5
	.long	0xee
	.uleb128 0x5
	.byte	0x3
	.long	num_reads
	.uleb128 0x9
	.long	.LASF43
	.byte	0x26
	.byte	0x5
	.long	0xee
	.uleb128 0x5
	.byte	0x3
	.long	was_read
	.uleb128 0x1d
	.long	.LASF64
	.byte	0x6
	.byte	0xd
	.byte	0x6
	.long	0x36a
	.uleb128 0xa
	.byte	0
	.uleb128 0x5
	.long	.LASF44
	.byte	0x2
	.byte	0x25
	.byte	0x5
	.long	0xee
	.long	0x38b
	.uleb128 0x1
	.long	0xf5
	.uleb128 0x1
	.long	0x43
	.uleb128 0x1
	.long	0x38b
	.uleb128 0xa
	.byte	0
	.uleb128 0x3
	.long	0x51
	.uleb128 0x5
	.long	.LASF45
	.byte	0x4
	.byte	0x15
	.byte	0x7
	.long	0x114
	.long	0x3b0
	.uleb128 0x1
	.long	0x114
	.uleb128 0x1
	.long	0xee
	.uleb128 0x1
	.long	0x43
	.byte	0
	.uleb128 0x5
	.long	.LASF46
	.byte	0x5
	.byte	0xe
	.byte	0x7
	.long	0x114
	.long	0x3c6
	.uleb128 0x1
	.long	0x43
	.byte	0
	.uleb128 0x5
	.long	.LASF47
	.byte	0x2
	.byte	0x1e
	.byte	0x5
	.long	0xee
	.long	0x3dc
	.uleb128 0x1
	.long	0x324
	.byte	0
	.uleb128 0x5
	.long	.LASF48
	.byte	0x2
	.byte	0x1f
	.byte	0x8
	.long	0x37
	.long	0x401
	.uleb128 0x1
	.long	0x114
	.uleb128 0x1
	.long	0x37
	.uleb128 0x1
	.long	0x37
	.uleb128 0x1
	.long	0x324
	.byte	0
	.uleb128 0x5
	.long	.LASF49
	.byte	0x2
	.byte	0x19
	.byte	0x7
	.long	0x324
	.long	0x41c
	.uleb128 0x1
	.long	0x38b
	.uleb128 0x1
	.long	0x38b
	.byte	0
	.uleb128 0x5
	.long	.LASF50
	.byte	0x2
	.byte	0x23
	.byte	0x5
	.long	0xee
	.long	0x433
	.uleb128 0x1
	.long	0x38b
	.uleb128 0xa
	.byte	0
	.uleb128 0x5
	.long	.LASF51
	.byte	0x2
	.byte	0x1a
	.byte	0x5
	.long	0xee
	.long	0x44f
	.uleb128 0x1
	.long	0x324
	.uleb128 0x1
	.long	0x38b
	.uleb128 0xa
	.byte	0
	.uleb128 0x1e
	.long	.LASF56
	.byte	0x1
	.byte	0x2c
	.byte	0x5
	.long	0xee
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x4b0
	.uleb128 0x7
	.long	.LASF52
	.byte	0x31
	.byte	0xb
	.long	0x324
	.uleb128 0x2
	.byte	0x75
	.sleb128 -12
	.uleb128 0x7
	.long	.LASF53
	.byte	0x36
	.byte	0x17
	.long	0x214
	.uleb128 0x2
	.byte	0x75
	.sleb128 -48
	.uleb128 0x7
	.long	.LASF54
	.byte	0x41
	.byte	0xb
	.long	0x114
	.uleb128 0x2
	.byte	0x75
	.sleb128 -16
	.uleb128 0x7
	.long	.LASF55
	.byte	0x4d
	.byte	0xb
	.long	0x114
	.uleb128 0x2
	.byte	0x75
	.sleb128 -20
	.uleb128 0xb
	.string	"dir"
	.byte	0x50
	.byte	0xb
	.long	0x114
	.uleb128 0x2
	.byte	0x75
	.sleb128 -24
	.byte	0
	.uleb128 0x1f
	.long	.LASF57
	.byte	0x1
	.byte	0x28
	.byte	0x6
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x4fb
	.uleb128 0xc
	.long	.LASF58
	.byte	0x1e
	.long	0x114
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xc
	.long	.LASF59
	.byte	0x28
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0xc
	.long	.LASF60
	.byte	0x34
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0xc
	.long	.LASF61
	.byte	0x41
	.long	0x116
	.uleb128 0x2
	.byte	0x91
	.sleb128 12
	.byte	0
	.uleb128 0x20
	.string	"fib"
	.byte	0x1
	.byte	0xb
	.byte	0x6
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x21
	.string	"n"
	.byte	0x1
	.byte	0xb
	.byte	0xe
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xb
	.string	"i"
	.byte	0xc
	.byte	0x9
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0xb
	.string	"t1"
	.byte	0xf
	.byte	0x9
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.string	"t2"
	.byte	0xf
	.byte	0x11
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x7
	.long	.LASF62
	.byte	0x12
	.byte	0x9
	.long	0xee
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x21
	.sleb128 40
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3e
	.uleb128 0x21
	.sleb128 7
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x21
	.sleb128 19
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF54:
	.string	"cdev"
.LASF55:
	.string	"test"
.LASF40:
	.string	"serial"
.LASF3:
	.string	"size_t"
.LASF28:
	.string	"SysFS_Meta"
.LASF59:
	.string	"offset"
.LASF43:
	.string	"was_read"
.LASF38:
	.string	"_Float128"
.LASF63:
	.string	"GNU C17 14.2.0 -mtune=i386 -march=i386 -g -fPIC"
.LASF61:
	.string	"head"
.LASF44:
	.string	"snprintf"
.LASF29:
	.string	"mkdir"
.LASF24:
	.string	"namelen"
.LASF52:
	.string	"metaFile"
.LASF50:
	.string	"printf"
.LASF14:
	.string	"write_specialized_callback"
.LASF2:
	.string	"FILE"
.LASF25:
	.string	"data"
.LASF58:
	.string	"file"
.LASF32:
	.string	"addChild"
.LASF51:
	.string	"fprintf"
.LASF20:
	.string	"numChildren"
.LASF60:
	.string	"nbytes"
.LASF4:
	.string	"long unsigned int"
.LASF26:
	.string	"parent"
.LASF39:
	.string	"short unsigned int"
.LASF19:
	.string	"children"
.LASF30:
	.string	"mkcdev"
.LASF45:
	.string	"memset"
.LASF33:
	.string	"setCallbacks"
.LASF42:
	.string	"num_reads"
.LASF49:
	.string	"fopen"
.LASF34:
	.string	"root"
.LASF27:
	.string	"chardev"
.LASF12:
	.string	"write_callback"
.LASF46:
	.string	"malloc"
.LASF47:
	.string	"fclose"
.LASF64:
	.string	"yield"
.LASF7:
	.string	"CDEV_READ"
.LASF8:
	.string	"unsigned int"
.LASF53:
	.string	"meta"
.LASF16:
	.string	"perms"
.LASF5:
	.string	"char"
.LASF13:
	.string	"read_callback"
.LASF18:
	.string	"SysFS_Chardev"
.LASF62:
	.string	"nextTerm"
.LASF56:
	.string	"main"
.LASF21:
	.string	"SysFS_Inode"
.LASF36:
	.string	"long long int"
.LASF31:
	.string	"createCdev"
.LASF23:
	.string	"name"
.LASF11:
	.string	"buf_size"
.LASF41:
	.string	"service_file_buf"
.LASF15:
	.string	"read_specialized_callback"
.LASF22:
	.string	"type"
.LASF17:
	.string	"SysFS_Directory"
.LASF35:
	.string	"long int"
.LASF37:
	.string	"long double"
.LASF6:
	.string	"CDEV_WRITE"
.LASF10:
	.string	"owner_pid"
.LASF57:
	.string	"read_notification"
.LASF9:
	.string	"CDEV_PERMS"
.LASF48:
	.string	"fread"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"service.c"
.LASF1:
	.string	"/home/aidanb/Greenwood-OS/filesystem/demos/service"
	.ident	"GCC: (GNU) 14.2.0"
