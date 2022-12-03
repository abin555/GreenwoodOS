bits 32
global update_page
update_page:
    invlpg [0]
    ret

extern page_directory
global load_page_directory
load_page_directory:
    mov eax, [page_directory]
    mov cr3, eax
    ret

global enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret