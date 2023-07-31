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

void kbd_test(){
    fb_putChar(8*CHAR_W, 8*CHAR_H, kbd_getChar(), 0xFFFFFF, 0);
}

int task_test(){
    register int reg_argc asm("eax");
    register char** reg_argv asm("ebx");
    int argc = reg_argc;
    char **argv = reg_argv;
    //int argc;
    //char **argv;
    //asm("movl %%eax, %0;" : "=r" (argc) : );
    //asm("movl %%ebx, %0;" : "=r" (argv) : );
    print_serial("\n\nINSIDE TEST TASK PROGRAM!\n");
    for(int i = 0; i < 10; i++){
        print_serial("Test 1: %x\n", i);
    }
    print_serial("ARGC: %x ARGV[0]: %s\n", argc, argv[0]);
    return 0;
}

int task_test2(){
    register int reg_argc asm("eax");
    register char** reg_argv asm("ebx");
    int argc = reg_argc;
    char **argv = reg_argv;
    print_serial("\n\nTest Task 2 %x %s\n", argc, argv[0]);
    for(int i = 0; i < 10; i++){
        print_serial("Test 2: %x\n", i);
    }
    print_serial("Exit Test Task 2\n");
    return 0;
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

    fb_print(0, 0, "Bootup Start");

    alloc_init();

    MEM_printRegions();

    PCI_init();

    kbd_init(0xFF);
    ps2_init();


    timer_init(1000);
    timer_attach(10, kbd_test);

    char *task_args[1] = {"Test"};
    start_task(task_test, -1, 1, task_args, "Test");
    start_task(task_test2, -1, 1, task_args, "Test");
    multitask_init();

    while(1){

    }

	return 0;
}
