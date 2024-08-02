#ifndef MEMORY_H
#define MEMORY_H

#include "stdint.h"
#include "framebuffer.h"
#include "paging.h"
#include "serial.h"
#include "grub.h"

#define MAX_MEMORY_SIZE 0x100000000 //4GB
#define MEMORY_NUM_REGIONS MAX_MEMORY_SIZE / PAGE_SIZE
#define WINDOW_BACKBUFFER_VIRT_BASE 0xB0000000

typedef enum {
	KERNEL = 1,
	DRIVER = 2,
	ALLOC = 3,
	SYSTEM = 4,
	OTHER = 5,
	PROGRAM = 6,
	UNDEFINED = 7,
	FRAMEBUFFER = 8,
	AVAILABLE = 9
} MEMORY_REGION_TYPE;

struct memory_region{
	uint8_t exists;
	uint8_t available;
	MEMORY_REGION_TYPE type;
	uint32_t physical_addr;
	uint32_t virtual_addr;
};

void memset(void *address, uint8_t value, uint32_t size);
void memcpy(void *dest, void* src, int size);
void memfcpy(void* target, void* source, uint32_t size);

int MEM_reserveRegion(uint32_t physical, uint32_t virtual, MEMORY_REGION_TYPE type);
void MEM_populateRegions();

int MEM_findRegionIdx(uint32_t size);
uint32_t MEM_reserveRegionBlock(int idx, uint32_t size, uint32_t virtual_base, MEMORY_REGION_TYPE type);

void MEM_printRegions();

#endif