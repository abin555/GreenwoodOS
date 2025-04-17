#include "descriptor_table.h"
#include "serial.h"
//Thank you Pretty OS

// GDT
#define NUMBER_GDT_GATES 6 // 0-5: Null, Kernel Code, Kernel Data, User Code, User Data, TSS

// Our GDT
static struct GDTentry gdt[NUMBER_GDT_GATES];

// Setup a descriptor in the Global Descriptor Table
void gdt_setGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    print_serial("[GDT] Gate %d - 0x%x 0x%x 0x%x 0x%x\n", num, base, limit, access, gran);
    // Setup the descriptor base address
    gdt[num].base_low    =  base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt[num].limit_low   = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access      = access;
}

void gdt_install(void)
{
    struct GDTptr gdt_register;
    print_serial("[GDT] Installing\n");

    // Setup the GDT pointer and limit
    gdt_register.limit = (sizeof(struct GDTentry) * NUMBER_GDT_GATES)-1;
    gdt_register.base  = (uint32_t)&gdt;

    // GDT GATES -  desriptors with pointers to the linear memory address
    gdt_setGate(0,0,0,0,0); // NULL descriptor

    //          num base limit    access                                              gran
    gdt_setGate(1,  0,   0xFFFFF, VALID | RING_0 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | USE32);
    gdt_setGate(2,  0,   0xFFFFF, VALID | RING_0 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | USE32);
    gdt_setGate(3,  0,   0xFFFFF, VALID | RING_3 | CODE_DATA_STACK | CODE_EXEC_READ,  _4KB_ | USE32);
    gdt_setGate(4,  0,   0xFFFFF, VALID | RING_3 | CODE_DATA_STACK | DATA_READ_WRITE, _4KB_ | USE32);

    tss_write(5, 0x10, 0x0); // num, ss0, esp0
    gdt_flush(&gdt_register);
    tss_flush();
}


/// TSS

static struct TSSentry tss = {};

// Initialise our task state segment structure.
void tss_write(int32_t num, uint16_t ss0, uint32_t esp0)
{
    // Firstly, let's compute the base and limit of our entry into the GDT.
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = sizeof(tss); //http://forum.osdev.org/viewtopic.php?f=1&t=19819&p=155587&hilit=tss_entry#p155587

    // Now, add our TSS descriptor's address to the GDT.
    gdt_setGate(num, base, limit, 0xE9, 0x00);

    tss.ss0  = ss0;  // Set the kernel stack segment.
    tss.esp0 = esp0; // Set the kernel stack pointer.

    tss.cs   = 0x08;
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x10;
}

void tss_switch(uint32_t esp0, uint32_t esp, uint32_t ss)
{
    tss.esp0 = esp0;
    tss.esp = esp;
    tss.ss = ss;
}