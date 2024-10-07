#include "serial.h"
#include <stdarg.h>
#define PORT 0x3f8
#include "framebuffer.h"

char quadToHex(uint8_t quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
    case 0x10:
      return '1';
      break;
    case 0x02:
    case 0x20:
      return '2';
      break;
    case 0x03:
    case 0x30:
      return '3';
      break;
    case 0x04:
    case 0x40:
      return '4';
      break;
    case 0x05:
    case 0x50:
      return '5';
      break;
    case 0x06:
    case 0x60:
      return '6';
      break;
    case 0x07:
    case 0x70:
      return '7';
      break;
    case 0x08:
    case 0x80:
      return '8';
      break;
    case 0x09:
    case 0x90:
      return '9';
      break;
    case 0x0a:
    case 0xa0:
      return 'A';
      break;
    case 0x0b:
    case 0xb0:
      return 'B';
      break;
    case 0x0c:
    case 0xc0:
      return 'C';
      break;
    case 0x0d:
    case 0xd0:
      return 'D';
      break;
    case 0x0e:
    case 0xe0:
      return 'E';
      break;
    case 0x0f:
    case 0xf0:
      return 'F';
      break;
  }
  return 'x';
}

int ser_printDecimal(int data){
    int idx = 0;
    int pow = 1;
    if(data < 0){
        write_serial('-');
        idx++;
        data *= -1;
    }
    while(pow * 10 <= data)
        pow *= 10;
    while(pow != 0){
        int d = data / pow;
        write_serial((char)((int)'0' + d));
        data = data - d * pow;
        pow /= 10;
        idx++;
    }
    return idx;
}

int ser_printHex(unsigned int data, int setlength){
    for(int i = (setlength ? setlength : 7); i >= 0; i--){
        write_serial(quadToHex((data >> 4*i) & 0xF));
    }
    return (setlength ? setlength : 8);
}

int ser_printBinary(unsigned int data, int setlength){
    for(int i = 0; i < (setlength ? setlength : 32); i++){
        if((data >> i) & 1){
            write_serial('1');
        }
        else{
            write_serial('0');
        }
    }
    return 32;
}

void ser_printStr(char *str){
  int i = 0;
  while(str[i] != '\0'){
    write_serial(str[i]);
    i++;
  }
}

int init_serial(){
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
    return 0;
}

int serial_received() {
   return inb(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return inb(PORT);
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
int fb_idx;
int fb_idxy = 0;
int serial_debug_mode = 0;
void write_serial(char a) {
  while (is_transmit_empty() == 0);
  #ifdef OS_DEBUG
  if(fb_frontbuffer != 0x0){
    fb_putChar(8*(fb_idx++), 8*fb_idxy, a, 0xFFFFFF, 0x0);
  }
  #endif
  outb(PORT,a);
}

void print_serial(char str[], ...){
    int i  = 0;
    fb_idx = 0;
    va_list listpd;
    va_start(listpd, str);
    while(1){
        if(str[i] == '%'){
            i++;
            switch(str[i]){
                case 'x':
                    ser_printHex(va_arg(listpd, unsigned int), 0);
                    break;
                case 's':
                    ser_printStr(va_arg(listpd, char*));
                    break;
                case 'c':
                    write_serial(va_arg(listpd, int));
                    break;
                case 'd':
                  ser_printDecimal(va_arg(listpd, int));
            }
        }
        else{
            if(str[i] == 0) break;
            write_serial(str[i]);
        }
        i++;
    }
    va_end(listpd);
    #ifdef OS_DEBUG
    if(fb_frontbuffer != 0x0){
      fb_idxy++;
      if(fb_idxy > (int) fb_height / 8) fb_idxy = 0;
      for(int j = 0; j < 0xFFFFF; j++){}
    }
    #endif
}