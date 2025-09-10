#ifndef APIC_H
#define APIC_H

#include "stdint.h"

struct APIC_CoreInfo {
    int numcores;
    uint32_t ids[16];
    uint32_t stack_regions[16];
};

extern struct APIC_CoreInfo apic_coreInfo;

void enable_apic();
void apic_startCores();

#endif