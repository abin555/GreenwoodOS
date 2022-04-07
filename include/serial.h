#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H
#include "io.h"

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write(unsigned int com, char b);
int init_serial();

#endif
