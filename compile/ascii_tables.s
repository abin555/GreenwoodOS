	.file	"ascii_tables.c"
	.text
	.comm	INT_Software_Value,4,4
	.comm	KYBRD_CAPS_LOCK,1,1
	.comm	KYBRD_SHIFT,1,1
	.comm	keyboard_KEYBUFFER,255,32
	.comm	keyboard_ASCIIBuffer,255,32
	.comm	keyboard_KEYBUFFER_POINTER,4,4
	.comm	keyboard_ascii_pointer,4,4
	.comm	prev_Scancode,1,1
	.comm	char_scancode,1,1
	.globl	kbd_US
	.data
	.align 32
	.type	kbd_US, @object
	.size	kbd_US, 256
kbd_US:
	.string	""
	.string	""
	.string	"1234567890-="
	.string	""
	.string	"qwertyuiop[]"
	.string	""
	.string	"asdfghjkl;'`"
	.string	"\\zxcvbnm,./"
	.string	"*"
	.string	" "
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"-"
	.string	""
	.string	""
	.string	"+"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.zero	165
	.globl	kbd_US_shift
	.align 32
	.type	kbd_US_shift, @object
	.size	kbd_US_shift, 256
kbd_US_shift:
	.string	""
	.string	""
	.string	"!@#$%^&*()_+"
	.string	""
	.string	"QWERTYUIOP{}"
	.string	""
	.string	"ASDFGHJKL:\"~"
	.string	"|ZXCVBNM<>?"
	.string	"*"
	.string	" "
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"-"
	.string	""
	.string	""
	.string	"+"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.zero	165
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
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
