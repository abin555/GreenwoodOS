#ifndef AUDIO_H
#define AUDIO_H

#include "pci.h"
#include "interrupts.h"
#include "serial.h"
#include "stdint.h"

struct audio_driver {
	struct PCI_driver *pci;
};

#endif