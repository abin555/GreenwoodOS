section program
global program
global program_end
extern exit_user_program

%define base 0
%define lbl_(x) (base+(x-program))
program:
    call check_key
    mov eax, 0x9
    int 0x80
    mov edx, ebx
    mov eax, 3
    mov ebx, lbl_(string)
    int 0x80

    cmp edx, 'Q'
    jne program
    mov eax, 1
    mov ebx, lbl_(quit_str)
    int 0x80
    ret

check_key:
    mov eax, 0x0a
    int 0x80
    mov ecx, ebx
check_key_loop
    mov eax, 0x0a
    int 0x80
    cmp ebx, ecx
    je check_key_loop
    ret
string:
    db '%c',0
quit_str:
    db 'QUIT USER PROGRAM',10,0

times 0x2000-($-program) db 0
program_end:
    
