#include "io.h"
#include "multiboot.h"
#include "types.h"
#include "framebuffer.h"

extern void load_gdt();

u32 vga_width;
u32 vga_height;
u64* screen;

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
  fb_setPixel(1,1,0xFFFFFF);
  fb_write_cell(1,1,'A',0xFFFFFF,0x000000);
  return 0;
  int color = 0;
  while(1){
    for(int y = 0; y < 768; y++){
      for(int x = 0; x < 1024; x++){
        fb[fb_width*y+x] = x + y + color;
      }
    }
    color++;
    if(color > 0xFFFFFF){
      color=0;
    }
  }


  return 0;
}