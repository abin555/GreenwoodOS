
program:
    mov eax, 0x0F
    int 0x80; Disable Console
    mov eax, 0x10
    int 0x80; Clear FB

    call load_flappy_bird_img
    mov [flappy_bird_X], DWORD 50
    mov [flappy_bird_Y], DWORD 250
    ;call draw_flappy_bird
    call main_loop
    call clean_flappy_bird_img
    mov eax, 0x0F
    int 0x80; Enable Console
    ret

main_loop:
    call draw_flappy_bird
    call get_key
    cmp eax, 'w'
    je move_up
    cmp eax, 's'
    je move_down
    cmp eax, 'q'
    jne main_loop 
    ret

move_up:
    call clear_flappy_bird
    mov eax, [flappy_bird_Y]
    sub eax, 12
    mov [flappy_bird_Y], eax
    jmp main_loop
move_down:
    call clear_flappy_bird
    mov eax, [flappy_bird_Y]
    add eax, 12
    mov [flappy_bird_Y], eax
    jmp main_loop

;Returns ASCII keycode in ebx register
check_key:
    mov eax, 0x0a
    int 0x80
    mov ecx, ebx
check_key_loop:
    mov eax, 0x0a
    int 0x80
    cmp ebx, ecx
    je check_key_loop
    ret

get_key:
    call check_key
    mov eax, 0x09
    int 0x80
    mov eax, ebx
    ret

load_flappy_bird_img:
    mov eax, 0x6
    mov ebx, 1024
    int 0x80;Call MALLOC syscall
    mov [flappy_bird_buffer_addr], eax

    mov eax, 0x0D
    mov ebx, 0
    mov ecx, 0x1a*4
    mov edx, flappy_bird_buffer_addr
    int 0x80;Read FS
    mov eax, 0x0D
    mov ebx, 0
    mov ecx, 0x1a*4 + 1
    mov edx, flappy_bird_buffer_addr
    add edx, 512
    int 0x80;Read FS
    ret
clean_flappy_bird_img:
    mov eax, 0x7
    mov ebx, flappy_bird_buffer_addr
    int 0x80; Free Bird
    ret

draw_flappy_bird:
    mov eax, 0x0B
    mov ebx, 17
    mov ecx, 12
    mov edx, flappy_bird_buffer_addr
    int 0x80;Call Load Image Buffer Syscall

    mov eax, 0x0C
    mov ebx, [flappy_bird_X]
    mov ecx, [flappy_bird_Y]
    int 0x80; Call Draw Image Syscall
    ret
clear_flappy_bird:
    mov eax, 0x0E
    mov ebx, [flappy_bird_X]
    mov ecx, [flappy_bird_Y]
    int 0x80
    ret

flappy_bird_X:
    dd 0
flappy_bird_Y:
    dd 0

flappy_bird_buffer_addr:
    dd 0