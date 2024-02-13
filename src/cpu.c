#include "cpu.h"

char vendorIDString[13] = {0};

void getCPUVendorString(){
    register uint32_t cpuID_mode asm("eax")__attribute__((unused)) = 0;
    asm volatile ("cpuid");
    register uint32_t ebx asm("ebx");
    register uint32_t edx asm("edx");
    register uint32_t ecx asm("ecx");
    uint32_t *vendorString = (uint32_t*) vendorIDString;
    vendorString[0] = ebx;
    vendorString[1] = edx;
    vendorString[2] = ecx;
    ////printk("CPUID: %s\n", vendorIDString);
    print_serial("CPUID: %s\n", vendorIDString);
}

uint8_t getCPUFeatures(uint32_t feature){
    uint32_t eax, unused, edx = 0;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    ////printk("CPU Feature:\n%b\n%b\n%b\n", feature, edx, edx & feature);
    return (edx & feature) != 0;
}

uint32_t getMTRR_low(uint32_t register_address){
    register uint32_t MTRR asm("ecx")__attribute__((unused)) = register_address;
    asm volatile("RDMSR");
    register uint32_t eax asm("eax");
    return eax;
}
uint32_t getMTRR_high(uint32_t register_address){
    register uint32_t MTRR asm("ecx")__attribute__((unused)) = register_address;
    asm volatile("RDMSR");
    register uint32_t edx asm("edx");
    return edx;
}
void setMTRR_low(uint32_t register_address, uint32_t value){
    register uint32_t MTRR asm("ecx")__attribute__((unused)) = register_address;
    register uint32_t edx asm("edx")__attribute__((unused)) = getMTRR_high(register_address);
    register uint32_t eax asm("eax")__attribute__((unused)) = value;
    asm volatile("WRMSR");
}
void setMTRR_high(uint32_t register_address, uint32_t value){
    register uint32_t MTRR asm("ecx")__attribute__((unused)) = register_address;
    register uint32_t edx asm("edx")__attribute__((unused)) = value;
    register uint32_t eax asm("eax")__attribute__((unused)) = getMTRR_low(register_address);
    asm volatile("WRMSR");
}

void enableSSE(){
    if(getCPUFeatures(CPUID_FEAT_EDX_SSE)/*SSE Feature*/){
        print_serial("CPU Supports SSE!\n");
        enableSSE_ASM();
    }
    else{
        print_serial("CPU does not support SSE!\n");
        asm("hlt");
    }
}