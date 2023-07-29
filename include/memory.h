#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"
#include "framebuffer.h"
#include "paging.h"
#include "serial.h"
#include "grub.h"

#define MAX_MEMORY_SIZE 0x100000000 //4GB
#define MEMORY_NUM_REGIONS MAX_MEMORY_SIZE / PAGE_SIZE

typedef enum {
	KERNEL = 1,
	DRIVER = 2,
	ALLOC = 3,
	SYSTEM = 4,
	OTHER = 5,
	PROGRAM = 6,
	UNDEFINED = 7
} MEMORY_REGION_TYPE;

struct memory_region{
	uint8_t exists : 1;
	uint8_t available : 1;
	MEMORY_REGION_TYPE type;
	uint32_t physical_addr;
	uint32_t virtual_addr;
	int region_group_parent_idx;
	uint32_t region_group_size;
};

struct memory_region MEMORY_REGIONS[MEMORY_NUM_REGIONS];

void MEM_populateRegion();

#endif