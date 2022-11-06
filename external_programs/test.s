section program
bits 32
org 0x00100000
main:
    mov ecx, 5
    jmp loop
loop:
    dec ecx
    mov eax, 0x01
    mov ebx, msg
    int 0x80
    cmp ecx, 0
    jne loop
    ret
msg:
    dd "User Program"
    dd 10
    dd 0
