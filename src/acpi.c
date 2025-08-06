#include "acpi.h"
#include "utils.h"
#include "memory.h"
#include "grub.h"
#include "apic.h"

struct RSDT *acpi_rsdt = NULL;

void acpi_init(void *rsdp_addr){
    if(rsdp_addr == NULL) return;
    struct RSDP_t *rsdp = rsdp_addr;
    print_serial("[ACPI] Init Start\n");
    print_serial("Signature: ");
    for(int i = 0; i < 8; i++){
        print_serial("%c", rsdp->Signature[i]);
    }
    print_serial("\n");
    print_serial("OEMID: ");
    for(int i = 0; i < 6; i++){
        print_serial("%c", rsdp->OEMID[i]);
    }
    print_serial("\n");
    print_serial("Revision: %d\n", rsdp->Revision);

    print_serial("RSDT Physical Address: 0x%x\n", rsdp->RsdtAddress);
    print_serial("Reserving RSDT Memory\n");
    MEM_reserveRegion((uint32_t)  rsdp->RsdtAddress, (uint32_t)  rsdp->RsdtAddress, SYSTEM);
    acpi_rsdt = (struct RSDT *) rsdp->RsdtAddress;

    print_serial("[ACPI] Initialized\n");

    print_serial("[ACPI] FADT Found at 0x%x\n", acpi_findFADT(acpi_rsdt));
    print_serial("[ACPI] MADT Found at 0x%x\n", acpi_findMADT(acpi_rsdt));
}

bool acpi_doChecksum(struct ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;

    for (uint32_t i = 0; i < tableHeader->Length; i++)
    {
        sum += ((char *) tableHeader)[i];
    }

    return sum == 0;
}

void *acpi_findFADT(struct RSDT *rsdt){
    if(rsdt == NULL) return NULL;
    int entries = (rsdt->h.Length - sizeof(struct ACPISDTHeader)) / 4;

    for(int i = 0; i < entries; i++){
        struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
        if (!strncmp(h->Signature, "FACP", 4)){
            return (void *) h;
        }
    }
    return NULL;
}

void *acpi_findMADT(struct RSDT *rsdt){
    if(rsdt == NULL) return NULL;
    int entries = (rsdt->h.Length - sizeof(struct ACPISDTHeader)) / 4;

    for(int i = 0; i < entries; i++){
        struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
        if (!strncmp(h->Signature, "APIC", 4)){
            return (void *) h;
        }
    }
    return NULL;
}

void acpi_initFADT(){
    print_serial("[FADT] Init\n");
    if(acpi_rsdt == NULL){
        print_serial("[FADT] FATAL: RSDT unidentified\n");
        return;
    }

    struct FADT *fadt = acpi_findFADT(acpi_rsdt);
    if(fadt == NULL){
        print_serial("[FADT] FATAL: FADT does not exist\n");
        return;
    }

    print_serial("ACPI Enable: %d\n", fadt->AcpiEnable & 0xFF);
    print_serial("ACPI Disable: %d\n", fadt->AcpiDisable & 0xFF);

    print_serial("Enabling ACPI\n");
    outb(fadt->SMI_CommandPort,fadt->AcpiEnable);
    
    while((inb(fadt->PM1aControlBlock) & 1) == 0){}

    print_serial("[FADT] Init Complete\n");
}

void acpi_shutdown(){
    print_serial("[SHUTDOWN] Start\n");
    if(acpi_rsdt == NULL){
        print_serial("[FADT] FATAL: RSDT unidentified\n");
        return;
    }

    struct FADT *fadt = acpi_findFADT(acpi_rsdt);
    if(fadt == NULL){
        print_serial("[FADT] FATAL: FADT does not exist\n");
        return;
    }
    //outw(fadt->PM1aControlBlock, SLP_TYPa | (1<<13) );
}

char *MADT_tag_strings[] = {
    "Processor Local APIC",
    "I/O APIC",
    "I/O APIC Interrupt Source Override",
    "I/O APIC Non-maskable interrupt source",
    "Local APIC Non-maskable interrupts",
    "Local APIC Address Override",
    "6?",
    "7?",
    "8?",
    "Processor Local"
};

void acpi_parseMADT(){
    print_serial("[MADT] Parse\n");
    if(acpi_rsdt == NULL){
        print_serial("[MADT] FATAL: RSDT unidentified\n");
        return;
    }

    struct MADT *madt = acpi_findMADT(acpi_rsdt);
    if(madt == NULL){
        print_serial("[MADT] FATAL: MADT does not exist\n");
        return;
    }

    if(!acpi_doChecksum(&madt->h)){
        print_serial("[MADT] Fails checksum!\n");
        return;
    }

    uint32_t madt_table_size = madt->h.Length - sizeof(struct ACPISDTHeader) - 2*sizeof(uint32_t);
    void *madt_base = madt->entries;
    uint32_t offset = 0;
    while(offset < madt_table_size){
        struct MADT_entry *entry = (struct MADT_entry *)(madt_base+offset);
        print_serial("[MADT] Entry Type: %s Len: %d\n", MADT_tag_strings[entry->type], entry->length);
        switch(entry->type){
            case 0://Processor Local APIC
            {
                print_serial("Processor Identified {\n");
                print_serial("\tProcessor ID: %d\n", entry->processorLocalAPIC.processor_id);
                print_serial("\t     APIC ID: %d\n", entry->processorLocalAPIC.apic_id);
                print_serial("\tFlags: %x\n", entry->processorLocalAPIC.flags);
                print_serial("}\n");
                apic_coreInfo.ids[apic_coreInfo.numcores] = entry->processorLocalAPIC.processor_id;
                apic_coreInfo.numcores++;
                break;
            }
            case 1://I/O APIC
            {
                print_serial("Found I/O APIC #%d @ 0x%x\n", entry->IO_APIC.id, entry->IO_APIC.addr);
                //MEM_reserveRegion(entry->IO_APIC.addr, entry->IO_APIC.addr, SYSTEM);
                break;
            }
            default:
                break;
        }
        offset += entry->length;
    }
}