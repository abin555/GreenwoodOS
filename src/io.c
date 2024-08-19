#include "io.h"

void outportl(uint16_t portid, uint32_t value)
{
	asm volatile("out dx, eax": :"d" (portid), "a" (value));
}
uint32_t inportl(uint16_t portid)
{
	uint32_t ret;
	asm volatile("in eax, dx":"=a"(ret):"d"(portid));
	return ret;
}

void mmio_outb(uint32_t base, uint8_t value) {
 uint8_t *mem = (uint8_t *) (base);
 *mem = value;
}

void mmio_outw(uint32_t base, uint16_t value) {
 uint16_t *mem = (uint16_t *) (base);
 *mem = value;
}

void mmio_outd(uint32_t base, uint32_t value) {
 uint32_t *mem = (uint32_t *) (base);
 *mem = value;
}

uint8_t mmio_inb(uint32_t base) {
 uint8_t *mem = (uint8_t *) (base);
 return *mem;
}

uint16_t mmio_inw(uint32_t base) {
 uint16_t *mem = (uint16_t *) (base);
 return *mem;
}

uint32_t mmio_ind(uint32_t base) {
 uint32_t *mem = (uint32_t *) (base);
 return *mem;
}