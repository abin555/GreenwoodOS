#include "apic.h"
#include "cpu.h"
#include "cpuid.h"
#include "memory.h"
#include "timer.h"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

/** returns a 'true' value if the CPU supports APIC
 *  and if the local APIC hasn't been disabled in MSRs
 *  note that this requires CPUID to be supported.
 */

struct APIC_CoreInfo apic_coreInfo;

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

    apic_coreInfo.numcores = 0;
}

extern int ap_trampoline;
uint32_t bspid, apic_bspdone;
uint8_t aprunning;
uint32_t apic_stack_top;

void apic_startCores(){
   print_serial("[APIC] starting cores!\n");
   aprunning = 0;  // count how many APs have started
   apic_bspdone = 0;
   // get the BSP's Local APIC ID
   __asm__ __volatile__ (
      "mov eax, 1\n\t"
      "cpuid\n\t"
      "shr ebx, 24\n\t"
      "mov %0, ebx\n\t"
      : "=r"(bspid)
      :
      : "eax", "ebx", "ecx", "edx"
   );

   print_serial("Reserving Stacks!\n");
   for(int i = 0; i < apic_coreInfo.numcores; i++){
      uint32_t coreid = apic_coreInfo.ids[i];
      if(coreid == bspid){
         print_serial("Is boot core, skipping\n");
         continue;
      }
      int block = MEM_findRegionIdx(0x7D00);
      apic_coreInfo.stack_regions[i] = MEM_reserveRegionBlock(block, 0x7D00, 0, STACK);
   }

   print_serial("BSP ID: %d, aprunning: %d\n", bspid, aprunning);

   create_page_entry(0x8000, 0x8000, 0x83);
   memcpy((void*)0x8000, &ap_trampoline, 4096);

   for(int i = 0; i < apic_coreInfo.numcores; i++){
      uint32_t coreid = apic_coreInfo.ids[i];
      apic_stack_top = apic_coreInfo.stack_regions[i] + 0x7D00;
      print_serial("Starting Core: %d (#%d)\n", coreid, i);
      if(coreid == bspid){
         print_serial("Is boot core, skipping\n");
         continue;
      }
      writeAPICRegister(0x280, 0);
      writeAPICRegister(0x310, readAPICRegister(0x310) | (coreid << 24));
      writeAPICRegister(0x300, (readAPICRegister(0x300) & 0xFFF00000) | 0x00C500);

      do {
         __asm__ __volatile__ ("pause" : : : "memory");
      } while(
         readAPICRegister(0x300) & (coreid << 12)
      );

      writeAPICRegister(0x310, readAPICRegister(0x310) | (coreid << 24));
      writeAPICRegister(0x300, (readAPICRegister(0x300) & 0xFFF00000) | 0x008500);

      do {
         __asm__ __volatile__ ("pause" : : : "memory");
      } while(
         readAPICRegister(0x300) & (coreid << 12)
      );
      print_serial("Wait 1\n");
      for(uint32_t wait = 0; wait < 0xFFFFF; wait++){
         for(uint32_t wait2 = 0; wait2 < 0xFF; wait2++){
         
         }
      }
      print_serial("Wait Done\n");

      for(int j = 0; j < 2; j++){
         writeAPICRegister(0x280, 0);
         writeAPICRegister(0x310, readAPICRegister(0x310) | (coreid << 24));
         writeAPICRegister(0x300, (readAPICRegister(0x300) & 0xff0f800) | 0x000608);
      }
      print_serial("Wait 2\n");
      for(uint32_t wait = 0; wait < 0xFFFF; wait++){
         for(uint32_t wait2 = 0; wait2 < 0xFF; wait2++){
         
         }
      }
      print_serial("Wait Done\n");
      do {
         __asm__ __volatile__ ("pause" : : : "memory");
      } while(
         readAPICRegister(0x300) & (coreid << 12)
      );
   }
   apic_bspdone = 1;
}