#ifndef SYS_MEMORY_H
#define SYS_MEMORY_H

void *memory_requestRegion(unsigned int size);
void memory_returnRegion(void *region, unsigned int size);

typedef enum {
	FEAT_FONT = 0x01,
	FEAT_KEYBOARD = 0x02,
	FEAT_PAGETABLE = 0x03,
	FEAT_TASKTABLE = 0x04,
	FEAT_PROGRAMBASE = 0x05,
	FEAT_FRAMEBUFFER = 0x06,
	FEAT_TIMER = 0x07,
	FEAT_KEYPRESSMAP = 0x08,

} KERNEL_FEATURE;

struct FEATURE_INFO{
	void *addr;
	unsigned int size;
};

struct FEATURE_INFO getKernelFeature(KERNEL_FEATURE feature);

#endif