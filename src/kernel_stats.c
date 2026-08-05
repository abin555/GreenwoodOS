#include "window.h"
#include "interrupts.h"
#include "system_calls.h"
#include "multitasking.h"
#include "framebuffer.h"

int kernel_stats(int argc, char **argv){
    (void) argc;
    (void) argv;
    
    struct WINDOW *win = window_open("Kernel Stats", 1);

    while(1){
        buf_print(win->backbuffer, 0, 0, "KERNEL STATISTICS", 0xFFFFFF, 0x0);
        window_copy_buffer(win);
        task_yield();
    }

    window_close(win);
}