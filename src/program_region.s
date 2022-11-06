section program
global program
global program_end
%define base 0
%define lbl_(x) (base+(x-program))
program:
    mov ecx, 5
    jmp label
label:
    dec ecx
	mov eax, 1
	mov ebx, lbl_(data_string)
	int 0x80    
    cmp ecx, 0
    jne label
    ret
data_string:
    db 'Program Running',10
    db 0
data_string_end:


times 0x2000-($-program) db 0
program_end:
    
