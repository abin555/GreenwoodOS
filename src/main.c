#include "multiboot.h"
#include "framebuffer.h"
#include "serial.h"
#include "paging.h"
#include "interrupts.h"
#include "descriptor_table.h"
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
#include "ethernet.h"
#include "acpi.h"
#include "apic.h"
#include "vfs.h"
#include "sysfs.h"
#include "audio.h"
#include "sysroot.h"
#include "audio_cdev.h"
#include "udp.h"
#include "dhcp.h"
#include "tcp.h"
#include "http.h"
#include "netfs.h"
#include "icmp.h"
#include "netproc.h"
#include "stddef.h"
#include "monitors.h"

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
    vfs_init();

    struct SysFS_Inode *sysroot = sysroot_init();
    sysfs_debugTree(sysroot, 0);

    vfs_addFS(
        sysfs_generateVFSRoot(
            sysroot,
            '-'
        )
    );


    window_init();
    console_init();

    rtc_init();

    serial_debug_mode = 0;

    kernel_console = NULL;
    struct task_state *kernel_task = &tasks[task_running_idx];
    udp_init();
    dhcp_init(ethernet_getDriver());
    tcp_init();
    //http_send_request(ethernet_getDriver(), (uint8_t [4]){10,0,1,2}, 80, "GET", "/", "localhost");

    netfs_init();    

    monitors_init();


    struct WINDOW *kernel_win = window_open("KERNEL", false);
    kernel_console = console_open(kernel_win);    
    kernel_task->console = kernel_console;
    kernel_task->window = kernel_win;
    set_schedule(ONFOCUS);
    

    char kernel_path[] = "A/";

    memset(kernel_task->currentDirectory.path, 0, sizeof(kernel_task->currentDirectory.path));
    memcpy(kernel_task->currentDirectory.path, kernel_path, sizeof(kernel_path));

    audio_init();    
    IDT_dump();
    netproc_init();
    start_task(desktop_viewer, -1, 0xDEADBEEF, NULL, "Desktop");
    //exec("/A/wolf/wolf.elf", 0, NULL);
    MEM_printRegions();

    task_lock = 0;
    set_schedule(NEVER);
    //task_yield();
    while(1){
        
    }
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
    print_serial("[GDT] Setup Start\n");
    gdt_install();
    interrupts_install_idt();
    getCPUVendorString();
    enableSSE();
    page_init();
    exceptions_init();
    set_PAT();
    MEM_populateRegions();
    enable_apic();

	fb_init(GRUB_tagfb);

    alloc_init();
    
    /*
    if(GRUB_ACPI_NEW){
        acpi_init(GRUB_ACPI_NEW->rsdp);
    }
    else{
        acpi_init(GRUB_ACPI_OLD->rsdp);
    }
    acpi_initFADT();
    acpi_parseMADT();
    */
    
    //apic_startCores();

    //tasking_setup_kernel_stack();

    PCI_init();
    IRQ_OFF;
    kbd_init(0xFF);
    mouse_init();
    ps2_init();
    timer_init(2);
    multitask_init();
    IRQ_RES;

    start_task(kernel_task, -1, 0xDEADBEEF, NULL, "Kernel");
    multitask_start();

    while(1){}
    return 1;
}
