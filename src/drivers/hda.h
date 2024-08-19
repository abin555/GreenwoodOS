#ifndef INTEL_HDA_H
#define INTEL_HDA_H

#include "io.h"
#include "pci.h"
#include "stdint.h"
#include "timer.h"

typedef enum {
	HDA_UNINITALIZED = 0,
	HDA_CORB_RIRB = 1,
	HDA_PIO = 2
} HDA_COMM_TYPE;

struct IntelHDA_Driver {
	uint32_t base;
	HDA_COMM_TYPE commType;
	bool initialized; 
	uint32_t input_stream_base;
	uint32_t output_stream_base;
	uint32_t *output_buffer_list;
	
	uint32_t *corb_mem;
	uint32_t corb_num_entries;
	bool corb_pointer;

	uint32_t *rirb_mem;
	uint32_t rirb_num_entries;
	bool rirb_pointer;
};


void hda_init(struct PCI_driver *driver);
int hda_send_verb(uint32_t codec, uint32_t node, uint32_t verb, uint32_t command);

#endif