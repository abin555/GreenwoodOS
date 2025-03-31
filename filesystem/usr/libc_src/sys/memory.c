#include <sys/memory.h>
#include <stdint.h>

void *memory_requestRegion(unsigned int size){
    register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = size;
	eax = 0x20;
	asm("int 0x80");
	return (void *) eax;
}
void memory_returnRegion(void *region, unsigned int size){
    register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	register unsigned int ecx asm("ecx");
	ecx = size;
	ebx = (unsigned int) region;
	eax = 0x2B;
	asm("int 0x80");
}

struct FEATURE_INFO getKernelFeature(KERNEL_FEATURE feature){
	void *addr;
	uint32_t size;
	struct FEATURE_INFO info;
	register uint32_t eax asm("eax");
	register uint32_t ebx asm("ebx");
	ebx = (uint32_t) feature;
	eax = 0x17;
	asm("int 0x80");
	addr = (void *) eax;
	size = ebx;
	info.addr = addr;
	info.size = size;
	return info;
}