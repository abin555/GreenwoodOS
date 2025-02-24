#ifndef AC97_H
#define AC97_H

#include "pci.h"
#include "stdint.h"
#include "serial.h"
#include "interrupts.h"
#include "memory.h"
#include "paging.h"
#include "audio.h"


struct audio_driver *ac97_init(struct PCI_driver *driver);

#endif