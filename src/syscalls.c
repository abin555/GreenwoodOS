#include "syscalls.h"

void init_syscalls(){
    interrupt_add_handle(0x80, &syscalls_callback);
    printk("[SYS] System Calls Initialized\n");
}
uint32_t syscall_fb_color = 0;
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
            fb_drawChar(cpu.ebx, cpu.ecx, cpu.edx, syscall_fb_color, ~syscall_fb_color);
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
            cpu_state.ebx = fb_width;
            cpu_state.ecx = fb_height;
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
        case 0x0E:
            for(uint32_t scan_y = 0; scan_y < image_buffer_def.height; scan_y++){
                for(uint32_t scan_x = 0; scan_x < image_buffer_def.width; scan_x++){
                    fb_setPixel(cpu.ebx + scan_x, cpu.ecx + scan_y, cpu.edx);
                }
            }
            break;
        case 0x0F:
            console_initialized = !console_initialized;
            cpu_state.eax = console_initialized;
            break;
        case 0x10:
            fb_clear(cpu.ebx);
            break;
        case 0x11:
            cpu_state.eax = (uint32_t) fopen(cpu.ebx, (char*) cpu.ecx);
            break;
        case 0x12:
            cpu_state.eax = fclose((FILE*) cpu.ebx);
            break;
        case 0x13:
            cpu_state.eax = (uint32_t) fread(cpu.ebx, cpu.ecx);
            break;
        case 0x14:
            cpu_state.eax = timer_ticks;
            break;
        case 0x15:
            set_backbuffer((uint8_t) cpu.ebx);
            break;
        case 0x16:
            swap_buffers();
            break;
        case 0x17:
            cpu_state.eax = (uint32_t) framebuffer;
            break;
        case 0x18:
            cpu_state.ebx = keyboard_KEYBUFFER[keyboard_KEYBUFFER_index-1];
            break;
        case 0x19:
            cpu_state.ebx = keyboard_KEYBUFFER_index;
            break;
        case 0x1A:
            syscall_fb_color = cpu.ebx;
            break;
    }
    return cpu_state;
}