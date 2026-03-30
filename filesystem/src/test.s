global _start


_start:
    mov eax, 0x1F
    int 0x80
    ret 2
