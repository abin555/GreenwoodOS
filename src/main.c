#include "multiboot.h"
#include "framebuffer.h"
#include "io.h"
#include "interrupts.h"
#include "memory.h"
#include "console.h"
#include "ps2.h"
#include "keyboard.h"
#include "pci.h"
#include "timer.h"
#include "ahci.h"
#include "filesystem.h"
#include "drivers.h"
#include "syscalls.h"
#include "programs/terminal.h"
#include "processes.h"
#include "programs.h"
#include "paging.h"
#include "serial.h"
#include "programs/image.h"
#include "ISO9660.h"
#include "cpu.h"
#include "mouse.h"
#include "drivers/audio/audio.h"
#include "drivers/audio/pcspk.h"
#include "drivers/audio/sb16.h"
#include "window_manager.h"
#include "multitasking.h"

uint32_t last_keycode_index;
void sys_timer_callback(){
    if(keyboard_KEYBUFFER_index != last_keycode_index){
        last_keycode_index = keyboard_KEYBUFFER_index;
        switch(keyboard_KEYBUFFER[keyboard_KEYBUFFER_index-1]){
            case 0xC3:
                audio_setEnable(!audio_muted);
                break;
            case 0xD8:
                if(!audio_muted){
                    play_sound(1000);
                    for(int i = 0; i < 40000000; i++){}
                    mute();
                }
                break;
        }
    }
}

void subfunction(int task, int val){
    for(int i = 0; i < val; i++){
        delay(50);
        printk("Task %d Subfunction %d\n", task, i);
    }
}

void test_task1(){
    subfunction(1,5);
    for(int i = 0; i < 10; i++){
        delay(10);
        printk("Task 1 %d\n", i);
    }
    printk("Task 1: This should, God willing End\n");
}

void test_task2(){
    for(int i = 0; i < 5; i++){
        delay(10);
        printk("Task 2 %d\n", i);
    }
    subfunction(2, 5);
    printk("Task 2 End?\n");
}

int kmain(unsigned long magic, unsigned long magic_addr){    
    init_serial();
    struct multiboot_tag *tag;
    // unsigned size;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return 0xFF;
    }
    // size = *(unsigned *) magic_addr;
    for (
        tag = (struct multiboot_tag *)(magic_addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *)tag;
            print_serial("Multiboot Address: 0x%x \nFramebuffer Address: 0x%x\0", (unsigned int) magic_addr, tagfb->common.framebuffer_addr);
            init_framebuffer(tagfb);
            break;
        }
        }
    }

    load_gdt();
    interrupts_install_idt();
    init_paging();
    fb_setChar(1,1, 'R', 0x00FFFF, 0);
    initialize_heap(0x06000000, 0xC00000);
    //init_backbuffer();
    initialize_console(fb_terminal_w,fb_terminal_h);
    
    //mem_dump();
    //init_backbuffer();
    //mem_dump();
    //return 0;
    init_ps2();
    init_timer(1000);

    initialize_ps2_keyboard(0);
    initialize_ps2_mouse(0);
    
    
    
    init_filesystem();

    getCPUVendorString();
    set_PAT();

    init_pci();

    drivers_init_pci();
    //init_soundblaster();
    init_syscalls();
    
    ready_filesystem();
    
    audio_init();
    init_mouse();
    
    timer_attach(100, sys_timer_callback);
    init_program_memory();

    
    
      

    //delay(2000);
    use_window = true;
    if(use_window){
        init_window_manager(10);
        console_addWindow();
        printk("Window Manager Init!\n");
        //timer_attach(10, draw_screen);
    }

    start_task(test_task1, -1, 0, "TestTask1");
    start_task(test_task2, -1, 0, "TestTask2");
    init_terminal();
    multitask_init();  
    while(1){}

    asm("hlt");
    return 0;
}
