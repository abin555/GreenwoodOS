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
  pci_init();

  for(unsigned int dev = 0; dev < devs; dev++){
    for(int offset = 0; offset < 4*20; offset+=4){
      //uint32_t data = pci_drivers[dev]->header->BAR[offset];
      uint32_t data = pci_read_dword(
        pci_devices[dev]->device_id->bus,
        pci_devices[dev]->device_id->slot,
        pci_devices[dev]->device_id->func,
        offset
      );
      decodeHex(STR_edit, data, 32, 0);
      fb_write_xy(STR_edit, 32/4, 1, dev*9, (devs+1)+(offset/4));
    }
  }

  activate_Drivers();

  terminal_init();
  //fb_write_xy(msg, heap_end - heap_begin, 0, 0, 6);

  kmain_loop();
  return 0;
}