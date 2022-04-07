#include "io.h"
#include "keyboard.h"
#include "interrupts.h"
#include "frame_buffer.h"
#include "serial.h"

struct gdt {
  unsigned int address;
  unsigned short size;
} __attribute__((packed));


int sum_of_three(int arg1, int arg2, int arg3){
	return arg1 + arg2 + arg3;
}

void delay(int times){
  for(int x = 0; x < 10*times; x++){
    for(int y = 0; y < 10*times; y++){

    }
  }
}

char decode[100];

void decodeData(int in, int len, int start){
  for(int x = 1; x < len; x++){
    if((in >> x) & 1){
      decode[len-x+start] = '1';
    }
    else{
      decode[len-x+start] = '0';
    }
  }
}
char quadtoHex(char quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
      return '1';
      break;
    case 0x02:
      return '2';
      break;
    case 0x03:
      return '3';
      break;
    case 0x04:
      return '4';
      break;
    case 0x05:
      return '5';
      break;
    case 0x06:
      return '6';
      break;
    case 0x07:
      return '7';
      break;
    case 0x08:
      return '8';
      break;
    case 0x09:
      return '9';
      break;
    case 0x0a:
      return 'A';
      break;
    case 0x0b:
      return 'B';
      break;
    case 0x0c:
      return 'C';
      break;
    case 0x0d:
      return 'D';
      break;
    case 0x0e:
      return 'E';
      break;
    case 0x0f:
      return 'F';
      break;
  }
  return 'x';
}

void decodeHex(int in, int len, int start){
  for(int i = 0; i < len/4; i++){
    decode[start+(len/4 - i)] = quadtoHex((in >> 4*i) & 0x0F);
  }
}

int main(){
  interrupt_install_idt();

  fb_clear(' ', FB_WHITE, FB_BLACK);

  while(1){
    decodeData(prev_Scancode, 8, 0);
    decodeHex(prev_Scancode, 8, 8);
    decodeData(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 11);
    decodeHex(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1], 8, 19);
    decodeHex(char_scancode, 8, 27);

    fb_move_cursor(0);
    fb_write(keyboard_KEYBUFFER, 100);
    fb_move_cursor(160);
    fb_write(decode, 8);
    fb_move_cursor_xy(0,3);
    fb_write_start(decode, 40, 0);
  }
  return 0;
}



