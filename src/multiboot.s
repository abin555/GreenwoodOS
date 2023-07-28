;;Section to define GRUB system boot headers
section .multiboot
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0	                     ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; insert optional multiboot tags here
;%define FORMER_SYS
%ifndef FORMER_SYS
align 8
framebuffer_tag_start:  
	DW 5 ;MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	DW 1 ;MULTIBOOT_HEADER_TAG_OPTIONAL
	DD framebuffer_tag_end - framebuffer_tag_start
	;DD 1366
	;DD 768
	;DD 1920
	;DD 1080
	DD 640
	DD 480
	DD 32
framebuffer_tag_end:
align 8
%endif
; required end tag
	dw 0    ; type
	dw 0    ; flags
	dd 8    ; size
header_end: