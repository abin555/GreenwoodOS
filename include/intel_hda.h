#ifndef INTEL_HDA_H
#define INTEL_HDA_H

#include "console.h"
#include "pci.h"
#include "paging.h"
#include "memory.h"
#include "timer.h"


void initialize_INTEL_HDA(int driverID);

uint32_t get_bitfield(uint32_t field, uint32_t bit);

uint8_t get_8_offset(uint32_t BAR, uint32_t offset);
uint16_t get_16_offset(uint32_t BAR, uint32_t offset);
uint32_t get_32_offset(uint32_t BAR, uint32_t offset);
uint32_t get_24_offset(uint32_t BAR, uint32_t offset);

void set_8_offset(uint32_t BAR, uint32_t offset, uint8_t val);
void set_16_offset(uint32_t BAR, uint32_t offset, uint16_t val);
void set_32_offset(uint32_t BAR, uint32_t offset, uint32_t val);
#endif