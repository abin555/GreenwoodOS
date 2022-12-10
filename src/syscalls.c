#include "syscalls.h"

void init_syscalls(){
    interrupt_add_handle(0x80, &syscalls_callback);
    printk("[SYS] System Calls Initialized\n");
}

struct cpu_state syscalls_callback(struct cpu_state cpu, struct stack_state stack __attribute__((unused))){
    struct cpu_state cpu_state = cpu;
    switch(cpu.eax){
        case 0x01:
            printk((char *) cpu.ebx);
            break;
        case 0x02:
            fb_setChar(fb_terminal_h-4,fb_terminal_w, cpu.ebx, 0x0000FF, 0);
            break;
        case 0x03:
            printk((char *) cpu.ebx, cpu.edx);
            break;
        case 0x04:
            fb_setChar(cpu.ebx, cpu.ecx, cpu.edx, 0x0000FF, 0);
            break;
        case 0x05:
            fb_setPixel(cpu.ebx, cpu.ecx, cpu.edx);
            break;
        case 0x06:
            cpu_state.eax = (unsigned int) malloc(cpu.ebx);
            break;
        case 0x07:
            free((void*) cpu.ebx);
            break;
        case 0x08:
            draw_image(cpu.ebx, cpu.ecx);
            break;
        case 0x09:
            cpu_state.ebx = keyboard_ASCIIBuffer[keyboard_ascii_index-1];
            break;
        case 0x0a:
            cpu_state.ebx = keyboard_ascii_index;
            break;
        case 0x0B:
            image_load_buffer(cpu.ebx, cpu.ecx, (uint8_t *) cpu.edx);
            break;
        case 0x0C:
            draw_buffer(cpu.ebx, cpu.ecx);
            break;
        case 0x0D:
            FS_read(cpu.ebx, cpu.ecx, 1, (uint32_t *) cpu.edx);
            break;
    }
    return cpu_state;
}