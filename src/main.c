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

uint8_t KERNEL_HEAP[0xFFFFF];

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
    print_serial("Serial Online\0");
    interrupts_install_idt();
    init_paging();
    print_serial("Interrupts Online\0");
    fb_setChar(1,1, 'R', 0x00FFFF, 0);
    print_serial("Heap 0x%x\n", KERNEL_HEAP);
    initialize_heap(0x04000000, 0x400000);
    init_backbuffer();
    initialize_console(fb_terminal_w,fb_terminal_h);
    initialize_ps2_keyboard(0);
    init_filesystem();

    getCPUVendorString();
    if(getCPUFeatures(CPUID_FEAT_EDX_PAT)){
        printk("CPU supports PAT\n");
    }
    if(getCPUFeatures(CPUID_FEAT_EDX_MTRR)){
        printk("CPU supports MTRR\n");
    }
    set_PAT();

    init_pci();

    drivers_init_pci();
    
    init_syscalls();

    init_timer(1000);
    
    init_terminal();

    if(ISO_checkFS(0)){
        ISO_read_volume_descriptor(0);
    }

    printk("Program Mem Located at 0x%x\n", &program);
    printk("Physical Addr 0x%x\n", get_physical(0));

    timer_attach(10, keyboard_dbg);

    printk("Address: %x\n", (uint32_t) filesystem_default_read_buffer);
    printk("Backbuffer Addr: %x\n", (uint32_t) back_buffer);

    process_scheduler();

    asm("hlt");
    return 0;
}
