#include "apic.h"
#include "cpu.h"
#include "cpuid.h"
#include "memory.h"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

/** returns a 'true' value if the CPU supports APIC
 *  and if the local APIC hasn't been disabled in MSRs
 *  note that this requires CPUID to be supported.
 */
bool check_apic() {
   uint32_t eax, unused, edx;
   __get_cpuid(1, &eax, &unused, &unused, &edx);
   return edx & CPUID_FEAT_EDX_APIC;
}

/* Set the physical address for local APIC registers */
void cpu_set_apic_base(void *apic) {
   uint32_t eax = (((uint32_t) apic) & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
   edx = (apic >> 32) & 0x0f;
#endif

   setMTRR_low(IA32_APIC_BASE_MSR, eax);
}

/**
 * Get the physical address of the APIC registers page
 * make sure you map it to virtual memory ;)
 */
void *cpu_get_apic_base() {
   uint32_t eax;
   eax = getMTRR_low(IA32_APIC_BASE_MSR);

#ifdef __PHYSICAL_MEMORY_EXTENSION__
   return (void *) (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
   return (void *) (eax & 0xfffff000);
#endif
}

static uint32_t readAPICRegister(uint32_t reg)
{
	return *((volatile uint32_t *)(cpu_get_apic_base() + reg));
}

static void writeAPICRegister(uint32_t reg, uint32_t value)
{
	*((volatile uint32_t *)(cpu_get_apic_base() + reg)) = value;
}

void enable_apic() {
    print_serial("[APIC] Enabling\n");
    /* Section 11.4.1 of 3rd volume of Intel SDM recommends mapping the base address page as strong uncacheable for correct APIC operation. */
    void *apic_base = cpu_get_apic_base();

    MEM_reserveRegion((uint32_t) apic_base, (uint32_t) apic_base, SYSTEM);

    /* Hardware enable the Local APIC if it wasn't enabled */
    cpu_set_apic_base(apic_base);

    /* Set the Spurious Interrupt Vector Register bit 8 to start receiving interrupts */
    writeAPICRegister(0xF0, readAPICRegister(0xF0) | 0x100);
}