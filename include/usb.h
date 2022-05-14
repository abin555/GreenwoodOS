#ifndef USB_H
#define USB_H

#include "frame_buffer.h"
#include "PCI.h"

char usb_driverName[27];


void usb_init_driver(int driverID);
void usb_exit_driver();

#endif