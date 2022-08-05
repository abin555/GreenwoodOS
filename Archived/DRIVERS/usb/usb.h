#ifndef USB_H
#define USB_H

#include "frame_buffer.h"
#include "PCI.h"
#include "hci.h"
#include "drivers.h"

char usb_driverName[31];

uint32_t **usb_host_device_list;
uint32_t usb_host_device_num;

void usb_init_driver(int driverID);
void usb_exit_driver();

#endif