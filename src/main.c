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
#include "audio.h"

void test_timer(){
    printk("Callback\n");
}
void test_timer2(){
    printk("BackCall\n");
}

void keyboard_dbg(){
    fb_setChar(fb_terminal_w-1, fb_terminal_h-2, keyboard_ASCIIBuffer[keyboard_ascii_index-1], 0xFFFFFF, 0);
    write_serial(keyboard_ASCIIBuffer[keyboard_ascii_index-1]);
}
void mouse_dbg(){
    fb_setPixel(mouse_x, mouse_y, 0xFF0000);
}

int kmain(unsigned long magic, unsigned long magic_addr){    
    //init_serial();
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
            //print_serial("Multiboot Address: 0x%x \nFramebuffer Address: 0x%x\0", (unsigned int) magic_addr, tagfb->common.framebuffer_addr);
            init_framebuffer(tagfb);
            break;
        }
        }
    }

    load_gdt();
    interrupts_install_idt();
    init_paging();
    fb_setChar(1,1, 'R', 0x00FFFF, 0);
    initialize_heap(0x04000000, 0x800000);
    init_backbuffer();
    initialize_console(fb_terminal_w,fb_terminal_h);
    initialize_ps2_keyboard(0);
    init_filesystem();

    getCPUVendorString();
    if(getCPUFeatures(CPUID_FEAT_EDX_PAT)){
        printk("CPU supports PAT\n");
        set_PAT();
    }
    if(getCPUFeatures(CPUID_FEAT_EDX_MTRR)){
        printk("CPU supports MTRR\n");
    }

    init_pci();

    init_timer(2000);

    drivers_init_pci();
    
    init_syscalls();

    
    init_terminal();
    ready_filesystem();

    if(audio_device_present){
        audio_devices[0]->set_volume(audio_devices[0]->hardware->stream, 255);
        audio_play(framebuffer, fb_width*fb_height);
    }

    //timer_attach(10, keyboard_dbg);

    process_scheduler();

    asm("hlt");
    return 0;
}
