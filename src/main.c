#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"
#include "grub.h"
#include "memory.h"
#include "allocator.h"
#include "pci.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "multitasking.h"
#include "ahci.h"
#include "drive.h"
#include "program.h"
#include "window.h"
#include "console.h"
#include "system_calls.h"
#include "random.h"
#include "exceptions.h"
#include "mouse.h"
#include "pcspeaker.h"
#include "desktop.h"
#include "ext2.h"
#include "rtc.h"

void kernel_task(int argc, char **argv){
    print_serial("Kernel Continuing Boot ARGC %x ARGV %x\n", argc, argv);
    task_lock = 1;
    print_serial("Program Init\n");
    program_init();
    init_syscalls();
    init_drive_system();
    for(int i = 0; i < PCI_numDrivers; i++){
        if(PCI_drivers[i]->init_driver != NULL){
            PCI_drivers[i]->init_driver(PCI_drivers[i]);
        }
    }

    drive_enumerate();

    window_init();
    console_init();

    rtc_init();

    serial_debug_mode = 0;

    struct WINDOW *kernel_win = window_open("KERNEL", false);
    kernel_console = console_open(kernel_win);
    struct task_state *kernel_task = &tasks[task_running_idx];
    kernel_task->console = kernel_console;
    kernel_task->window = kernel_win;
    set_schedule(ONFOCUS);

    char kernel_path[] = "A/";

    memset(kernel_task->currentDirectory.path, 0, sizeof(kernel_task->currentDirectory.path));
    memcpy(kernel_task->currentDirectory.path, kernel_path, sizeof(kernel_path));

    print_console(kernel_console, "kernel Window & Console Opened.\n");
    print_console(kernel_console, "Initial Directory: %s\n", kernel_path);
    
    print_console(kernel_console, "Loading Desktop Task...\n");
    start_task(desktop_viewer, -1, 0xDEADBEEF, NULL, "Desktop");  
    window_close(kernel_win);  

    task_lock = 0;
    return;
}

int kmain(unsigned int magic, unsigned long magic_addr){
    init_serial();
    print_serial("\n\nGreenwood OS Boot Start!\n");
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        print_serial("WTF, not multiboot?!?!\nHow did we get here...\n:(\n");
        return 0xFF;
    }
    parse_multiboot2(magic_addr);
    load_gdt();
    interrupts_install_idt();
    getCPUVendorString();
    enableSSE();
    page_init();
    exceptions_init();
    set_PAT();
    MEM_populateRegions();

	fb_init(GRUB_tagfb);

    alloc_init();

    PCI_init();
    kbd_init(0xFF);
    mouse_init();
    ps2_init();
    timer_init(1);

    multitask_init();

    start_task(kernel_task, -1, 0xDEADBEEF, NULL, "Kernel");
    multitask_start();

    while(1){}
    return 1;
}
