#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/task.h>
#include <sys/window.h>
#include <sys/sysfs.h>

void addKbdEventHandler(void (*handler)(unsigned char)){
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) handler;
	eax = 0x21;
	asm("int 0x80");
}

void desktop_kbd_event(char ascii);

int main(int argc, char **argv){
    struct WINDOW *window = window_open("DESKTOP", true);
    addKbdEventHandler(desktop_kbd_event);

    set_schedule(ALWAYS);
    printf("Starting Desktop Environment\n");
}

void __attribute__ ((optimize("-O3"))) desktop_kbd_event(char ascii){
    //print_serial("[DESKTOP] Kbd callback - %c\n", (char) ascii);
    if(KBD_flags.ctrl && ascii == 'T'){
        exec("/A/OS/termvp/term.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'E'){
        exec("/A/utils/explorer/explorer.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'Q'){
        exec("/A/utils/task/taskmgr.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'C'){
        exec("/A/utils/clock/clock.elf", 0, NULL);
    }
    else if(KBD_flags.ctrl && ascii == 'L'){
        exec("/A/lisp/lisp.elf", 0, NULL);
    }
    /*
    else if(KBD_flags.ctrl && ascii == 'M'){
        for(int i = 0; i < global_viewport_list->count; i++){
            if(global_viewport_list->elements[i].inUse){
                if(!global_viewport_list->elements[i].vp->minimized)
                    viewport_toggle_size(global_viewport_list->elements[i].vp);
            }
        }
    }
    else if(global_viewport_list->elements[0].inUse){
        if(global_viewport_list->elements[0].vp == NULL) return;
        if(!global_viewport_list->elements[0].vp->minimized){
            global_viewport_list->elements[0].vp->ascii = (char) ascii;
        }
    }
    */
}