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
#include "ethernet.h"
#include "acpi.h"
#include "apic.h"
#include "vfs.h"
#include "sysfs.h"
#include "audio.h"
#include "wav.h"
#include "sysroot.h"
#include "audio_cdev.h"

extern void zig_test();

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

    vfs_addSysRoot(
        sysroot,
        '-'
    );

    window_init();
    console_init();

    rtc_init();

    serial_debug_mode = 0;

    kernel_console = NULL;
    struct task_state *kernel_task = &tasks[task_running_idx];

    ethernet_demo();
    audio_init();


    struct WINDOW *kernel_win = window_open("KERNEL", false);
    kernel_console = console_open(kernel_win);    
    kernel_task->console = kernel_console;
    kernel_task->window = kernel_win;
    set_schedule(ONFOCUS);
    

    char kernel_path[] = "A/";

    memset(kernel_task->currentDirectory.path, 0, sizeof(kernel_task->currentDirectory.path));
    memcpy(kernel_task->currentDirectory.path, kernel_path, sizeof(kernel_path));
    
    
    print_console(kernel_console, "kernel Window & Console Opened.\n");
    print_console(kernel_console, "Initial Directory: %s\n", kernel_path);
    
    print_console(kernel_console, "Loading Desktop Task...\n");
    print_console(kernel_console, "There are %d drives\n", drive_count);
    for(int i = 0; i < drive_count; i++){
        print_console(kernel_console, "Drive %d is type %d and format %d\n", i, drives[i]->type, drives[i]->format);
    }

    zig_test();

    audio_init();

    
    int fd_test = vfs_open("A/test.txt\0", VFS_FLAG_READ | VFS_FLAG_WRITE);
    print_serial("[VFS] opened fd %d\n", fd_test);
    if(fd_test != -1){
        char buf[12];
        memset(buf, 0, 12);
        int numb = vfs_read(fd_test, buf, 10);
        print_serial("Read: %s (%d bytes)\n", buf, numb);
        vfs_read(fd_test, buf, 1);
        numb = vfs_read(fd_test, buf, 10);
        print_serial("Read: %s (%d bytes)\n", buf, numb);

        print_serial("Seeked to %d\n", vfs_seek(fd_test, 0, 0));
        vfs_write(fd_test, "GOOF", 4);
        print_serial("Now at %d\n", vfs_seek(fd_test, 0, 1));
        print_serial("Seeked to %d\n", vfs_seek(fd_test, 0, 0));
        memset(buf, 0, 12);
        numb = vfs_read(fd_test, buf, 6);
        print_serial("Read: %s (%d bytes)\n", buf, numb);

        vfs_close(fd_test);
    }


    print_serial("VFS Pipe test\n");
    int pipe_read, pipe_write;
    vfs_mkpipe(&pipe_write, &pipe_read);
    char buf1[10];
    char buf2[10];
    memset(buf1, 0, 10);
    memset(buf2, 0, 10);
    int n;
    n = vfs_write(pipe_write, "Hello There", 11);
    print_serial("Wrote %d bytes to pipe\n", n);
    n = vfs_read(pipe_read, buf1, 5);
    print_serial("Read %d bytes from pipe \"%s\"\n", n, buf1);
    vfs_read(pipe_read, NULL, 1);
    memset(buf1, 0, sizeof(buf1));
    n = vfs_read(pipe_read, buf1, 6);
    print_serial("Read %d bytes from pipe \"%s\"\n", n, buf1);

    vfs_close(pipe_read);
    vfs_close(pipe_write);

    int sysfs_test = vfs_open("-/sys/audio", VFS_FLAG_READ | VFS_FLAG_WRITE);
    if(sysfs_test != -1){
        print_serial("Found sysfile!\n");
        struct AudioInterface audio;
        n = vfs_read(sysfs_test, &audio, sizeof(audio));
        print_serial("Read %d bytes from sysfile\n", n);
        audio.set_volume(50);
        /*
        int wav_fd = vfs_open("A/Audio/Macintosh.wav\0", VFS_FLAG_READ);
        int wav_size = vfs_seek(wav_fd, 0, 2);
        wav_size = 44100*8;
        vfs_seek(wav_fd, 0, 0);
        void *file_buf = (void*) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(wav_size), wav_size, 0, OTHER));
        vfs_read(wav_fd, file_buf, wav_size);
        audio.wav_play(audio.wav_read(file_buf, wav_size), 0);
        vfs_close(wav_fd);
        */
        //wait(500);
        //audio.stop();
    }
    vfs_close(sysfs_test);
    
    int sysserial = vfs_open("-/dev/serial", VFS_FLAG_WRITE);
    if(sysserial != -1){
        char Testbuf[] = "\n\nSERIAL BUFFER TEST\n\n";
        vfs_write(sysserial, Testbuf, sizeof(Testbuf));
        vfs_write(sysserial, Testbuf, sizeof(Testbuf));
        vfs_close(sysserial);
    }

    /*
    int wav_fd = vfs_open("A/Audio/Macintosh.wav\0", VFS_FLAG_READ);
    if(wav_fd != -1){
        int wav_size = vfs_seek(wav_fd, 0, 2);
        vfs_seek(wav_fd, 0, 0);
        void *file_buf = (void*) get_virtual(MEM_reserveRegionBlock(MEM_findRegionIdx(wav_size), wav_size, 0, OTHER));
        vfs_read(wav_fd, file_buf, wav_size);
        audio_set_volume(100);
        wav_play(wav_read_info(file_buf, wav_size), 0);
    }
    */
    
    
    start_task(desktop_viewer, -1, 0xDEADBEEF, NULL, "Desktop");  
    //window_close(kernel_win);  

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
    //enable_apic();

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
    

    //tasking_setup_kernel_stack();

    PCI_init();
    
    kbd_init(0xFF);
    mouse_init();
    ps2_init();
    timer_init(2);

    multitask_init();

    start_task(kernel_task, -1, 0xDEADBEEF, NULL, "Kernel");
    multitask_start();

    while(1){}
    return 1;
}
