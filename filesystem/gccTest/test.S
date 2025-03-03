	.file	"main.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/aidanb/Greenwood-OS/filesystem/gccTest" "main.c"
	.globl	dprint
	.type	dprint, @function
dprint:
.LFB0:
	.file 1 "main.c"
	.loc 1 4 29
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	.loc 1 7 8
	movl	8(%ebp), %eax
	.loc 1 7 6
	movl	%eax, %ebx
	.loc 1 8 6
	movl	$37, %eax
	.loc 1 9 2
/APP
/  9 "main.c" 1
	int $0x80
/  0 "" 2
	.loc 1 10 1
/NO_APP
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	dprint, .-dprint
	.globl	msg
	.section	.rodata
.LC0:
	.string	"Test Program\n"
	.section	.text,"aw"
	.align 4
	.type	msg, @object
	.size	msg, 4
msg:
	.long	.LC0
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.loc 1 15 32
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 16 5
	movl	msg, %eax
	pushl	%eax
	call	dprint
	addl	$4, %esp
	.loc 1 17 12
	movl	$1, %eax
	.loc 1 18 1
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xfe
	.value	0x5
	.byte	0x1
	.byte	0x4
	.long	.Ldebug_abbrev0
	.uleb128 0x5
	.long	.LASF13
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0x6
	.long	0x2d
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x7
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.long	.LASF7
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.long	.LASF8
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.long	.LASF9
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x8
	.string	"msg"
	.byte	0x1
	.byte	0xd
	.byte	0xd
	.long	0x83
	.uleb128 0x5
	.byte	0x3
	.long	msg
	.uleb128 0x2
	.long	0x34
	.uleb128 0x9
	.long	.LASF14
	.byte	0x1
	.byte	0xf
	.byte	0x5
	.long	0x47
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0xbd
	.uleb128 0x3
	.long	.LASF11
	.byte	0xe
	.long	0x47
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x3
	.long	.LASF12
	.byte	0x1b
	.long	0xbd
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.byte	0
	.uleb128 0x2
	.long	0xc2
	.uleb128 0x2
	.long	0x2d
	.uleb128 0xa
	.long	.LASF15
	.byte	0x1
	.byte	0x4
	.byte	0x6
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0xb
	.string	"msg"
	.byte	0x1
	.byte	0x4
	.byte	0x19
	.long	0x83
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x4
	.string	"eax"
	.byte	0x5
	.long	0x63
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x4
	.string	"ebx"
	.byte	0x6
	.long	0x63
	.uleb128 0x1
	.byte	0x53
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
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
	.uleb128 0x2
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x21
	.sleb128 15
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x21
	.sleb128 24
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x6
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
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
	.uleb128 0x8
	.uleb128 0x34
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
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
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
	.uleb128 0xa
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
	.byte	0
	.byte	0
	.uleb128 0xb
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
.LASF10:
	.string	"long long unsigned int"
.LASF7:
	.string	"unsigned char"
.LASF2:
	.string	"long unsigned int"
.LASF8:
	.string	"short unsigned int"
.LASF13:
	.string	"GNU C17 14.2.0 -mtune=i386 -march=i386 -g"
.LASF14:
	.string	"main"
.LASF9:
	.string	"unsigned int"
.LASF3:
	.string	"char"
.LASF15:
	.string	"dprint"
.LASF11:
	.string	"argc"
.LASF6:
	.string	"long long int"
.LASF5:
	.string	"short int"
.LASF12:
	.string	"argv"
.LASF4:
	.string	"signed char"
	.section	.debug_line_str,"MS",@progbits,1
.LASF1:
	.string	"/home/aidanb/Greenwood-OS/filesystem/gccTest"
.LASF0:
	.string	"main.c"
	.ident	"GCC: (GNU) 14.2.0"
