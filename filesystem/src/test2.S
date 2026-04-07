global _start

_start:
    mov eax, 0x38
    mov ebx, 1
    mov ecx, str
    mov edx, 14
    int 0x80
    mov eax, 0x1f
    int 0x80
    ret 2

str:
    dd "Hello World!",10,0