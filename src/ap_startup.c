#include "ap_startup.h"
#include "interrupts.h"
#include "serial.h"
#include "descriptor_table.h"

#include "window.h"
#include "framebuffer.h"

volatile int ap_hold = 0;

int dummy(int v){
    debug_byte(v);
    return v+1;
}

static volatile int entry_count = 0;

void ap_startup(int apicid){
    register uint32_t ebx asm("ebx");
    debug_byte(0xAA);
    debug_byte('Z');
    debug_byte('0'+apicid);
    debug_byte('0'+ebx);
    uint8_t ap_id = ebx;

    volatile uint32_t *probe = (uint32_t*)0x1807cf0; // adjust to match this run's actual slot if it shifts
    for (int i = 0; i < 10; i++) {
        debug_byte((uint8_t)(*probe & 0xFF));
    }

    dummy(56);

    for (int i = 0; i < 10; i++) {
        debug_byte((uint8_t)(*probe & 0xFF));
    }

    print_serial("[CPU] Core #%d online!\n", apicid);
    dummy(68);
    interrupts_install_idt();
    print_serial("[CPU] Core #%d installed IDT\n", apicid);

    while(!ap_hold){

    }

    uint32_t x = 50 + 16*ap_id;
    uint32_t y = 50 + 8*ap_id;
    int vx = 1;
    int vy = 1;

    //struct WINDOW *ap_win = window_open("AP CORE", false);
    fb_putChar(50, 50, 'A', 0x00FF00, 0x000000);
    //window_copy_buffer(ap_win);
    while(1){ 
        asm("hlt");
        //fb_putChar(x, y, 'A', 0x00FF00, 0x000000);
        if(x > 1000 || x <= 0) vx *= -1;
        if(y > 500 || y <= 0) vy *= -1;
        x += vx;
        y += vy;
        for(uint32_t i = 0x0; i < 0xFFF; i++){
            asm("pause");
            fb_putChar(x, y, 'A', 0x00FF00, 0x000000);
            fb_putChar(x+8, y, '0'+ap_id, 0x00FF00, 0x000000);
        }
    }
}