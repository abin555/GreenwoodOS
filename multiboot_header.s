section .multiboot_header
header_start:
    dd 0xe85250d6                ;Magic Number for Multiboot 2
    dd 0                         ;Architecture 0 (Protected x86)
    dd header_end - header_start ;header length
    ;Checksum
    dd 0x10000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ;Optional Tags

    ;Required End Tags
    dw 0    ;type
    dw 0    ;flags
    dd 8    ;size
header_end: