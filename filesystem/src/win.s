_start:
    mov eax, 0x01
    mov ebx, title
    mov ecx, 0
    int 0x80

    mov eax, 0x07
    mov ebx, 4
    int 0x80

    mov eax, 0x34
    int 0x80
    ret

title:
    db "Test Window", 0
