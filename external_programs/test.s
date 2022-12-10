bits 32
main:
    call 0xc0105170
    ret
    mov ecx, 10
loop:
    mov eax, 3
    mov ebx, str
    mov edx, ecx
    int 0x80
    dec ecx
    cmp ecx, 0
    je end
    jmp loop
end:
    call get_char
    mov edx, eax
    mov eax, 3
    mov ebx, test_str
    int 0x80
    ret

get_char:
    mov eax, 0xa
    int 0x80
    mov ecx, ebx
get_char_loop:
    mov eax, 0xa
    int 0x80
    cmp ebx, ecx
    je get_char_loop
    mov eax, 0x9
    int 0x80
    mov eax, ebx
    ret

str:
    db 'Hello World %1x',10,0

test_str:
    db 'Pressed Key: %c',10,0