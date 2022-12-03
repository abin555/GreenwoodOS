section program
global program
global program_end
extern exit_user_program
%define base 0
%define lbl_(x) (base+(x-program))
program:
    mov eax, 1
    mov ebx, lbl_(open_string)
    int 0x80

    mov ecx, 10
    jmp top
top:
    dec ecx
    cmp ecx, 0
    je clean

    mov eax, [lbl_(var_1)]
    mov edx, [lbl_(var_2)]
    add edx,eax
    mov [lbl_(var_1)], edx
    mov [lbl_(var_2)], edx
    mov eax, 3
    mov ebx, lbl_(string)
    int 0x80
    jmp top

clean:
    mov [lbl_(var_1)], DWORD 1
    mov [lbl_(var_2)], DWORD 1
    mov eax, 1
    mov ebx, lbl_(close_string)
    int 0x80
    ret

var_1:
    dw 1
    dw 0
var_2:
    dw 1
    dw 0
string:
    db "Value: %x",10,0
open_string:
    db "User Executed Program",10,0
close_string:
    db "Done Executing Program",10,0

times 0x2000-($-program) db 0
program_end:
    
