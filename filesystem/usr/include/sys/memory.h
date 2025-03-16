#ifndef SYS_MEMORY_H
#define SYS_MEMORY_H

void *memory_requestRegion(unsigned int size);
void memory_returnRegion(void *region, unsigned int size);

#endif