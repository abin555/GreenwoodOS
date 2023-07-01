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
#include "multitasking.h"
#include "window.h"

void test_window(){
    char window_name[16] = "Test Window\0";
    struct window *win = create_window(window_name);
    memset(win->buffer, 0xAAFFAA, WINDOW_ALLOC_WIDTH*WINDOW_ALLOC_HEIGHT);
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
            print_serial("Multiboot Address: 0x%x \nFramebuffer Address: 0x%x\n", (unsigned int) magic_addr, tagfb->common.framebuffer_addr);
            init_framebuffer(tagfb);
            break;
        }
        }
    }

    load_gdt();
    interrupts_install_idt();
    init_paging();
    fb_setChar(1,1, 'R', 0x00FFFF, 0);
    fb_setChar(1,2, '?', 0, 0xFFFFFF);
    initialize_heap(0x06000000, 0xC00000);
    
    initialize_console(fb_terminal_w,fb_terminal_h);

    //mem_dump();
    init_backbuffer();
    dump_page_map();

    //init_ps2();
    init_timer(1000);
    //return 0;
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
    
    //audio_init();
    init_mouse();

    init_program_memory();

    init_terminal();
    start_window_mgr();
    start_task(test_window, -1, 0, "TestWin");
    multitask_init();  

    while(1){}

    asm("hlt");
    return 0;
}
