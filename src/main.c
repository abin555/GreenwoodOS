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


void idle_task(){
    char c = '@';
    while(1){
        if(c == '@') c = 'X';
        else c = '@';
        fb_putChar(fb_width-CHAR_W,fb_height-CHAR_H, c, 0xFFFFFFFF, 0);
        for(int i = 0; i < 100000; i++){
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
    }
}

void kernal_task(int argc, char **argv){
    print_serial("Kernel Continuing Boot ARGC %x ARGV %x\n", argc, argv);
    //fb_print(0, 8, "Kernal Tasking!");

    program_init();
    init_syscalls();
    window_init();
    console_init();
    init_drive_system();

    //printk("[Driver] Initializing Drivers\n");
    for(int i = 0; i < PCI_numDrivers; i++){
        if(PCI_drivers[i]->init_driver != NULL){
            //printk("[Driver] Init #%x\n", i);
            PCI_drivers[i]->init_driver(PCI_drivers[i]);
        }
    }
    MEM_printRegions();

    drive_enumerate();
    
    //AHCI_read((volatile HBA_PORT *)(drives[0]->driver.ahci), 0, 0, 5, (uint16_t *) fb_frontbuffer);
    
    ISO9660_printTree(drive_get('A')->format_info.ISO);

    start_task(idle_task, -1, 0, NULL, "IDLE");
    struct FILE* file = fopen("A/CHILD/TEST/TEST.TXT");
    //print_serial("First Char: %c\n", fgetc(file));
    char *buf = malloc(fsize(file));
    fcopy(file, buf, fsize(file));
    print_serial("%s\n", buf);
    fclose(file);

    struct WINDOW *kernal_win = window_open("KERNEL", true);
    kernal_console = console_open(kernal_win);
    struct task_state *kernal_task = &tasks[task_running_idx];
    kernal_task->console = kernal_console;
    char kernal_path[] = "A/";
    memset(kernal_task->currentDirectory.path, 0, sizeof(kernal_task->currentDirectory.path));
    memcpy(kernal_task->currentDirectory.path, kernal_path, sizeof(kernal_path));
    print_console(kernal_console, "TEST!\nTEST2\n");

    exec("OS/TERM/TERM.EXE", 0, NULL);

    //set_schedule(ONFOCUS);
    char c = '#';
    while(1){
        if(c == '#') c = '*';
        else c = '#';
        fb_putChar(fb_width-2*CHAR_W,fb_height-CHAR_H, c, 0xFFFFFFFF, 0);
        for(int i = 0; i < 100000; i++){
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
    }
    window_close(kernal_win);
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
    // size = *(unsigned *) magic_addr;
	load_gdt();
    interrupts_install_idt();
    getCPUVendorString();
    enableSSE();
    //asm("int 0x80");
	page_init();


    set_PAT();

    MEM_populateRegions();
    

	fb_init(GRUB_tagfb);

    for(int i = 0; i < 8; i++){
        fb_putChar(i*8, 4*8, '1'+i, 0xFFFFFFFF, 0);
    }
    //printk("Bootup Start\n");
    MEM_printRegions();
    alloc_init();
    //printk("Alloc Success\n");

    MEM_printRegions();

    //printk("Init PCI\n");
    PCI_init();
    //printk("Init Keyboard\n");
    kbd_init(0xFF);
    ps2_init();

    timer_init(1000);
    //printk("Starting Kernal Task");
    start_task(kernal_task, -1, 0xDEADBEEF, NULL, "Kernel");
    multitask_init();

    while(1){

    }

	return 0;
}
