#include "io.h"
#include "keyboard.h"
#include "interrupts.h"
#include "frame_buffer.h"
#include "serial.h"
#include "string.h"
#include "terminal.h"
#include "keyboard_test.h"
#include "multiboot.h"

extern void load_gdt();

void PROGRAMA();
__attribute__((unused))
int kmain(unsigned int magic, unsigned int magic_addr){
  load_gdt();
  interrupt_install_idt();
  fb_clear(' ', FB_WHITE, FB_BLACK);

  decodeHex(STR_edit, magic, 32, 0);
  fb_write_xy(STR_edit, 8, 1, 0, 2);
  decodeHex(STR_edit, magic_addr, 32, 0);
  fb_write_xy(STR_edit, 8, 1, 0, 3);
  while(1){
    switch(SYS_MODE){
      case 1:
        terminal_handler();
        break;
      case 4:
        KYBRD_DEBUG_DISPLAY();
        break;
      default:
        KYBRD_DEBUG_DISPLAY();
        
    }
  }
  return 0;
}