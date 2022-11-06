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

void test_timer(){
    printk("Callback\n");
}
void test_timer2(){
    printk("BackCall\n");
}

void keyboard_dbg(){
    fb_setChar(fb_terminal_w-1, fb_terminal_h-2, keyboard_ASCIIBuffer[keyboard_ascii_index-1], 0xFFFFFF, 0);
    write_serial(keyboard_ASCIIBuffer[keyboard_ascii_index]);
}

uint8_t KERNEL_HEAP[0xFFFFF];

int kmain(unsigned long magic, unsigned long magic_addr){
    //
    
    init_serial();
    struct multiboot_tag *tag;
    struct multiboot_tag_mmap* memory_map;
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
        case MULTIBOOT_TAG_TYPE_MMAP:
            memory_map = (struct multiboot_tag_mmap *) tag;
            break;
        }
    }

    load_gdt();
    print_serial("Serial Online\0");
    interrupts_install_idt();
    print_serial("Interrupts Online\0");
    fb_setChar(1,1, 'R', 0xF0FFFF, 0);
    print_serial("Heap 0x%x\n", KERNEL_HEAP);
    initialize_heap((uint32_t) KERNEL_HEAP, sizeof(KERNEL_HEAP));
    initialize_console(fb_terminal_w,fb_terminal_h);
    printk("[GRUB MULTIBOOT] Address 0x%x\n", magic_addr);
    for(int i = 0; i < 10; i++){
        //struct multiboot_mmap_entry mmap_entry = memory_map->entries[i];
        printk("[Memory Map] Address: 0x%x Length: 0x%x Type: %x \n",
            memory_map->entries[i].upper_addr,
            memory_map->entries[i].upper_len,
            memory_map->entries[i].type
        );
    }
    init_paging();

    initialize_ps2_keyboard(0);
    init_filesystem();
    
    init_pci();
    
    //drivers_init_pci();
    
    init_syscalls();


    init_timer(1);
    
    init_terminal();
    printk("Program Mem Located at 0x%x\n", &program);

    uint8_t *mem = 0;
    for(int y = 0; y < 10; y++){
        for(int x = 0; x < 10; x++){
            printk("%2x ", mem[y*10+x]);
        }
        printk("\n");
    }

    timer_attach(10, keyboard_dbg);
    //add_process(exec_user_program);
    process_scheduler();
    asm("hlt");
    //timer_attach(10, test_timer);
    //timer_attach(20, test_timer2);
    return 0;
}
