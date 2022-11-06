section .text
global restore_process
restore_process_eax: dd 0
restore_process:
    pop eax
    mov [restore_process_eax], eax
    pop ebx
    pop ecx
    pop edx
    pop ebp
    pop esi
    pop edi
    pop esp

    pop eax
    jmp eax