#include "mouse.h"
uint8_t mouse_cycle = 0;
uint16_t mouse_byte[3];

uint16_t mouse_x = 0;
uint16_t mouse_y = 0;

void mouse_wait(uint8_t a_type) //unsigned char
{
  uint32_t _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(uint8_t a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

uint8_t mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

struct cpu_state mouse_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    switch(mouse_cycle){
        case 0:
            mouse_byte[0] = inb(0x60);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = inb(0x60);
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = inb(0x60);
            mouse_x += mouse_byte[1];
            mouse_y += mouse_byte[2];
            mouse_cycle = 0;
            break;
    }
    printk("[MOUSE] Interrupt\n");
    return cpu;
}

void initialize_ps2_mouse(){
    printk("[MOUSE] Initializing\n");
    IRQ_OFF;
    uint8_t status;
    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status=(inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();

    interrupt_add_handle(44, &mouse_handler);
    IRQ_clear_mask(12);

    IRQ_RES;
}