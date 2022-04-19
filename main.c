#include "io.h"
#include "keyboard.h"
#include "interrupts.h"
#include "frame_buffer.h"
#include "serial.h"
#include "string.h"
#include "terminal.h"
#include "keyboard_test.h"

extern void load_gdt();

struct gdt {
  unsigned int address;
  unsigned short size;
} __attribute__((packed));


int sum_of_three(int arg1, int arg2, int arg3){
	return arg1 + arg2 + arg3;
}

void PROGRAMA();

int main(){
  load_gdt();
  interrupt_install_idt();
  fb_clear(' ', FB_WHITE, FB_BLACK);
  
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