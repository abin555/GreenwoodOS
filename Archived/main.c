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
#include "PCI.h"
#include "drivers.h"
#include "console.h"
#include "filesystem.h"
#include "mouse.h"
#include "ps2.h"
#include "pic.h"

extern void load_gdt();

void kmain_loop(){
  int state = 0;
  while(1){
    terminal_handler();
    if(state){
      printChar(25, 25, 'A');
    }
    else{
      printChar(25,25, 'Z');
    }
    state = !state;
    //printk("%2h\n", inb(PS2_DATA));
  }
}

extern unsigned int kernel_end;

int kmain(unsigned long magic, unsigned long magic_addr){
  struct multiboot_tag *tag;
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
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            struct multiboot_tag_framebuffer *tagfb
              = (struct multiboot_tag_framebuffer *) tag;
            init_fb(tagfb);
        }
    }
  }


  load_gdt();
  interrupt_install_idt();

  fb_set_color(0xFFFFFF,0);
  restore_kernel_addr = (u32 *) &kmain_loop;

  mem_init(0x01000000);

  initializeConsole();
  //mouse_init_sanik();
  
  //init_ps2();
  keyboard_enable();
  //mouse_init();
  //keyboard_enable();
  
  init_filesystem();
  printk("PCI Listing:\n\0");
  pci_init();

  printk("\nDriver Init:\n\0");
  activate_Drivers();


  terminal_init();

  kmain_loop();
  return 0;
}