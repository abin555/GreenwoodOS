#ifndef SERIAL_H
#define SERIAL_H
#include "io.h"

extern int serial_debug_mode;

int init_serial();

int serial_recieved();
char read_serial();

int is_transmit_empty();
void write_serial(char a);

void print_serial(char str[], ...);

#endif