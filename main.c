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
#include "usb.h"
#include "drivers.h"
#include "console.h"
//#include "IDE.h"

extern void load_gdt();

void kmain_loop(){
  while(1){
    terminal_handler();
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

  mem_init(0x10000000);

  initializeConsole();
  printk("PCI Listing:\n\0");
  pci_init();
  /*
  for(unsigned int drive = 0; drive < drivs; drive++){
    decodeHex(STR_edit, pci_drivers[drive]->init_one->Class, 32, 0);
    fb_write_xy(STR_edit, 32/4, 1, drive*9, 30);
    for(int bar = 0; bar < 6; bar++){
      uint32_t data = pci_drivers[drive]->BAR[bar];
      
      decodeHex(STR_edit, data, 32, 0);
      fb_write_xy(STR_edit, 32/4, 1, drive*9, bar+31);
    }
  }

  while(0){
    strcpy("  HELLO \0", STR_edit+1, sizeof("   HELLO \0"));
    STR_edit[0]++;
    printk(STR_edit);

  }
  */
  printk("\nDriver Init:\n\0");
  activate_Drivers();
  
  #define Size 2*512
  #define Width 16
  uint8_t *FileBuffer = malloc(Size);
  bool working = true;
  int sector = 0;
  while(working){
    working=AHCI_read(
      Drive_PORTS[0],
      sector,
      0,
      2,
      (uint16_t *)FileBuffer
    );
    //printk(FileBuffer);
    for(int y = 0; y < Size/8; y++){
      for(int x = 0; x < 16; x++){
        printChar(50+x, y, FileBuffer[x + y*16]);
      }
    }
    if(FileBuffer[1] == 'E' && FileBuffer[2] == 'L' && FileBuffer[3] == 'F'){
      working = false;
      printk("%8h\n", sector);
    }
    sector++;
    //for(unsigned int count =0; count < 0x111FFFF; count++){}
  }
  
  terminal_init();

  kmain_loop();
  return 0;
}