#include "io.h"
#include "keyboard.h"
#include "interrupts.h"
#include "frame_buffer.h"
#include "serial.h"
#include "string.h"
#include "terminal.h"
#include "keyboard_test.h"

extern void load_gdt();

int kmain(){
  load_gdt();
  interrupt_install_idt();
  unsigned long* location = (unsigned long*)0xA0000;
  unsigned long count = 0;
  while(count < 0xFFFFFFFF){
  *location = 0xFFFF;
  location += 16;
  count += 16;
  }
  return 0;
}