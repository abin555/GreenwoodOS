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

void kernel_task(int argc, char **argv){
    print_serial("Kernel Continuing Boot ARGC %x ARGV %x\n", argc, argv);
    task_lock = 1;
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
    MEM_printRegions();

    struct WINDOW *kernel_win = window_open("KERNEL", true);
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
    char boot_program_path[] = "/A/OS/term/term.exe";


    print_console(kernel_console, "Starting INIT Program: %s\n", boot_program_path);
    //exec("/A/tune/tune.exe", 0, NULL);

    
    start_task(desktop_viewer, -1, 0xDEADBEEF, NULL, "Desktop");    

    //exec(boot_program_path, 0, NULL);



    task_lock = 0;

    char OpenedConsole = 0;
    while(1){
        if(&windows[window_selected] == kernel_win){
            if(kbd_getChar() == 10 && !OpenedConsole){
                OpenedConsole = 1;
                exec(boot_program_path, 0, NULL);
            }
            if(!(KBD_flags.key == 10)){
                OpenedConsole = 0;
            }
        }
    }
    window_close(kernel_win);
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
    MEM_printRegions();

    alloc_init();
    MEM_printRegions();

    PCI_init();
    kbd_init(0xFF);
    mouse_init();
    ps2_init();
    timer_init(1);

    //PCSpeaker_Handle.beep();

    start_task(kernel_task, -1, 0xDEADBEEF, NULL, "Kernel");
    multitask_init();

    while(1){}
    return 1;
}
