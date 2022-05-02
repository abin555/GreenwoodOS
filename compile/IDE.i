# 1 "DRIVERS/IDE.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "DRIVERS/IDE.c"
# 1 "./include/IDE.h" 1


# 1 "./include/io.h" 1



# 1 "./include/gcc_stdint.h" 1
# 34 "./include/gcc_stdint.h"
typedef signed char int8_t;


typedef short int int16_t;


typedef int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef int intptr_t;


typedef unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 5 "./include/io.h" 2

extern void outb(unsigned short port, unsigned char data);

extern unsigned char inb(unsigned short pos);

void WriteMem(uint32_t Address, uint32_t Value);
uint32_t ReadMem(uint32_t Address);

extern void loadGDT(unsigned short GDT);

void pic_acknowledge(unsigned int interrupt);

extern void software_int();
unsigned char *INT_Software_Value;
void software_interrupt(unsigned char interrupt);

extern void restore_kernel();
extern void PROGA();

extern unsigned int *externalProgram;
# 4 "./include/IDE.h" 2
# 82 "./include/IDE.h"
struct IDEChannelRegisters {
   unsigned short base;
   unsigned short ctrl;
   unsigned short bmide;
   unsigned char nIEN;
} channels[2];



unsigned char ide_buf[2048] = {0};
static volatile unsigned char ide_irq_invoked = 0;


struct ide_device {
   unsigned char Reserved;
   unsigned char Channel;
   unsigned char Drive;
   unsigned short Type;
   unsigned short Signature;
   unsigned short Capabilities;
   unsigned int CommandSets;
   unsigned int Size;
   unsigned char Model[41];
} ide_devices[4];

unsigned char ide_read(unsigned char channel, unsigned char reg);
void ide_write(unsigned char channel, unsigned char reg, unsigned char data);
void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer, unsigned int quads);
unsigned char ide_polling(unsigned char channel, unsigned int advanced_check);



void ide_initialize(
    unsigned int BAR0,
    unsigned int BAR1,
    unsigned int BAR2,
    unsigned int BAR3,
    unsigned int BAR4);
# 2 "DRIVERS/IDE.c" 2

unsigned char ide_read(unsigned char channel, unsigned char reg) {
   unsigned char result;
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      result = inb(channels[channel].base + reg - 0x00);
   else if (reg < 0x0C)
      result = inb(channels[channel].base + reg - 0x06);
   else if (reg < 0x0E)
      result = inb(channels[channel].ctrl + reg - 0x0A);
   else if (reg < 0x16)
      result = inb(channels[channel].bmide + reg - 0x0E);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, channels[channel].nIEN);
   return result;
}

void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      outb(channels[channel].base + reg - 0x00, data);
   else if (reg < 0x0C)
      outb(channels[channel].base + reg - 0x06, data);
   else if (reg < 0x0E)
      outb(channels[channel].ctrl + reg - 0x0A, data);
   else if (reg < 0x16)
      outb(channels[channel].bmide + reg - 0x0E, data);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, channels[channel].nIEN);
}

void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer,
                     unsigned int quads) {




   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, 0x80 | channels[channel].nIEN);
   asm("pushw %es; movw %ds, %ax; movw %ax, %es");
   if (reg < 0x08)
      insl(channels[channel].base + reg - 0x00, buffer, quads);
   else if (reg < 0x0C)
      insl(channels[channel].base + reg - 0x06, buffer, quads);
   else if (reg < 0x0E)
      insl(channels[channel].ctrl + reg - 0x0A, buffer, quads);
   else if (reg < 0x16)
      insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
   asm("popw %es;");
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, 0x0C, channels[channel].nIEN);
}

unsigned char ide_polling(unsigned char channel, unsigned int advanced_check) {



   for(int i = 0; i < 4; i++)
      ide_read(channel, 0x0C);



   while (ide_read(channel, 0x07) & 0x80)
      ;

   if (advanced_check) {
      unsigned char state = ide_read(channel, 0x07);



      if (state & 0x01)
         return 2;



      if (state & 0x20)
         return 1;




      if ((state & 0x08) == 0)
         return 3;

   }

   return 0;

}
# 122 "DRIVERS/IDE.c"
void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3,
unsigned int BAR4) {

   int j, k, count = 0;


   channels[ATA_PRIMARY ].base = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
   channels[ATA_PRIMARY ].ctrl = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
   channels[ATA_SECONDARY].base = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
   channels[ATA_SECONDARY].ctrl = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
   channels[ATA_PRIMARY ].bmide = (BAR4 & 0xFFFFFFFC) + 0;
   channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8;

   ide_write(ATA_PRIMARY , 0x0C, 2);
   ide_write(ATA_SECONDARY, 0x0C, 2);

   for (i = 0; i < 2; i++)
      for (j = 0; j < 2; j++) {

         unsigned char err = 0, type = 0x00, status;
         ide_devices[count].Reserved = 0;


         ide_write(i, 0x06, 0xA0 | (j << 4));
         sleep(1);


         ide_write(i, 0x07, 0xEC);
         sleep(1);



         if (ide_read(i, 0x07) == 0) continue;

         while(1) {
            status = ide_read(i, 0x07);
            if ((status & 0x01)) {err = 1; break;}
            if (!(status & 0x80) && (status & 0x08)) break;
         }



         if (err != 0) {
            unsigned char cl = ide_read(i, 0x04);
            unsigned char ch = ide_read(i, 0x05);

            if (cl == 0x14 && ch ==0xEB)
               type = 0x01;
            else if (cl == 0x69 && ch == 0x96)
               type = 0x01;
            else
               continue;

            ide_write(i, 0x07, 0xA1);
            sleep(1);
         }


         ide_read_buffer(i, 0x00, (unsigned int) ide_buf, 128);


         ide_devices[count].Reserved = 1;
         ide_devices[count].Type = type;
         ide_devices[count].Channel = i;
         ide_devices[count].Drive = j;
         ide_devices[count].Signature = *((unsigned short *)(ide_buf + 0));
         ide_devices[count].Capabilities = *((unsigned short *)(ide_buf + 98));
         ide_devices[count].CommandSets = *((unsigned int *)(ide_buf + 164));


         if (ide_devices[count].CommandSets & (1 << 26))

            ide_devices[count].Size = *((unsigned int *)(ide_buf + 200));
         else

            ide_devices[count].Size = *((unsigned int *)(ide_buf + 120));


         for(k = 0; k < 40; k += 2) {
            ide_devices[count].Model[k] = ide_buf[54 + k + 1];
            ide_devices[count].Model[k + 1] = ide_buf[54 + k];}
         ide_devices[count].Model[40] = 0;

         count++;
      }


   for (i = 0; i < 4; i++)
      if (ide_devices[i].Reserved == 1) {
         printk(" Found %s Drive %dGB - %s\n",
            (const char *[]){"ATA", "ATAPI"}[ide_devices[i].Type],
            ide_devices[i].Size / 1024 / 1024 / 2,
            ide_devices[i].Model);
      }
}
