#include "io.h"
#include "multiboot.h"
#include "types.h"
#include "frame_buffer.h"
#include "keyboard.h"
#include "terminal.h"
#include "interrupts.h"
#include "keyboard_test.h"
#include "gfx.h"
#include "pong.h"
#include "memory.h"
#include "pci.h"
#include "drivers.h"
#include "console.h"
#include "filesystem.h"
#include "mouse.h"
#include "ps2.h"
#include "pic.h"
#include "timer.h"
#include "paging.h"

extern void load_gdt();

void kmain_loop(){
  while(1){
    terminal_handler();
    //console_moveline(-1);
    //printk("%2h\n", inb(PS2_DATA));
  }
}

extern unsigned int kernel_end;

int kmain(unsigned long magic, unsigned long magic_addr){
  struct multiboot_tag *tag;
  __attribute__((unused)) struct multiboot_tag_mmap* memory_map; 
  //unsigned size;
  if(magic != MULTIBOOT2_BOOTLOADER_MAGIC){
    return 0xFF;
  }
  //size = *(unsigned *) magic_addr;
  for(
    tag = (struct multiboot_tag *) (magic_addr + 8);
    tag->type != MULTIBOOT_TAG_TYPE_END;
    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
  ){
    switch(tag->type){
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:{
        struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *) tag;
        init_fb(tagfb);
        break;
      }
      case MULTIBOOT_TAG_TYPE_MMAP:
        //memory_map = (struct multiboot_tag_mmap *) tag;
        break;
    }
  }


  load_gdt();
  interrupt_install_idt();

  fb_set_color(0xFFFFFF,0);
  restore_kernel_addr = (u32 *) &kmain_loop;

  mem_init(0x01000000);
  
  initializeConsole();
  printk("[HEAP] Heap memory initialized at %8x\n[HEAP] Paging memory address initialized to %8x\n", heap_begin, placement_address);
  //init_paging();
  /*
  printk("Memory Map:\n");
  for(unsigned int i = 0; i < memory_map->entry_size; i++){
    printk("Addr: %8h%8h Sz: %8h%8h Type: %2h Zero: %8h\n", memory_map->entries[i].addr, memory_map->entries[i].len, memory_map->entries[i].type, memory_map->entries[i].zero);
  }
  */
  timer_init(1000);
  init_ps2();
  
  init_filesystem();
  printk("PCI Listing:\n\0");
  pci_init();

  printk("\nDriver Init:\n\0");
  activate_Drivers();



  terminal_init();

  kmain_loop();
  return 0;
}