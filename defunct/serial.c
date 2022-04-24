#include "serial.h"

#define SERIAL_COM1_BASE 0x3F8

#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

#define SERIAL_LINE_ENABLE_DLAB 0x80;


/** serial_configure_baud_rate:
* Sets the speed of the data being sent. The default speed of a serial
* port is 115200 bits/s. The argument is a divisor of that number, hence
* the resulting speed becomes (115200 / divisor) bits/s.
**
@param com The COM port to configure
* @param divisor The divisor
*/
void serial_configure_baud_rate(unsigned short com, unsigned short divisor){
  outb(SERIAL_LINE_COMMAND_PORT(com),
       0x80);
  outb(SERIAL_DATA_PORT(com),
       (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com),
       divisor & 0x00FF);
}

/** serial_configure_line:
* Configures the line of the given serial port. The port is set to have a
* data length of 8 bits, no parity bits, one stop bit and break control
* disabled.
**
@param com The serial port to configure
*/
void serial_configure_line(unsigned short com){
  /* Bit: | 7 | 6 | 5 4 3 | 2 | 1 0 |
  * Content: | d | b | prty | s | dl |
  * Value: | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
  */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
* Checks whether the transmit FIFO queue is empty or not for the given COM
* port.
**
@param com The COM port
* @return 0 if the transmit FIFO queue is not empty
* 1 if the transmit FIFO queue is empty
*/
int serial_is_transmit_fifo_empty(unsigned int com){
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(unsigned int com, char b){
  while(serial_is_transmit_fifo_empty(com) == 0){}
  outb(com, b);
}

#define PORT 0x3F8

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
