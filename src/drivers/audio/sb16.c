#include "sb16.h"

#define SB16_IRQ 5
#define SB16_IRQ_DATA  0x2
//SB16 Ports
#define DSP_MIXER_PORT 0x224
#define DSP_MIXER_DATA_PORT 0x225
#define DSP_RESET 0x226
#define DSP_READ 0x22A
#define DSP_WRITE 0x22C
#define DSP_READ_STS 0x22E
#define DSP_16_INT_ACK 0x22F

//SB16 config commands
#define DSP_PROG_16     0xB0
#define DSP_PROG_8      0xC0
#define DSP_AUTO_INIT   0x06
#define DSP_PLAY        0x00
#define DSP_RECORD      0x08
#define DSP_MONO        0x00
#define DSP_STEREO      0x20
#define DSP_UNSIGNED    0x00
#define DSP_SIGNED      0x10

#define DMA_CHANNEL_16  5
#define DMA_FLIP_FLOP   0xD8
#define DMA_BASE_ADDR   0xC4
#define DMA_COUNT       0xC6

// commands for DSP_WRITE
#define DSP_SET_TIME    0x40
#define DSP_SET_RATE    0x41
#define DSP_ON          0xD1
#define DSP_OFF         0xD3
#define DSP_OFF_8       0xD0
#define DSP_ON_8        0xD4
#define DSP_OFF_16      0xD5
#define DSP_ON_16       0xD6
#define DSP_VERSION     0xE1

// commands for DSP_MIXER
#define DSP_VOLUME  0x22
#define DSP_IRQ     0x80

#define SAMPLE_RATE     48000
#define BUFFER_MS       40

#define BUFFER_SIZE ((uint32_t) (SAMPLE_RATE * (BUFFER_MS / 1000.0)))

uint16_t *sb16_buffer;
bool buffer_flip = false;

uint64_t sample = 0;

uint8_t volume_master;

int reset_sb16(){
    printk("[SB16] Reset\n");
    outb(DSP_RESET, 1);

    //delay(100);
    
    uint8_t status = inb(DSP_READ_STS);
    if(~status & 128){
        printk("1 %2x\n", status);
        goto fail;
    }

    status = inb(DSP_READ);
    if(status != 0xAA){
        printk("2 %2x\n", status);
        goto fail;
    }

    outb(DSP_WRITE, DSP_VERSION);
    uint8_t major_ver = inb(DSP_READ);
    uint8_t minor_ver = inb(DSP_READ);
    printk("[SB16] Ver: %2x%2x\n", major_ver, minor_ver);
    if(major_ver < 4){
        status = (major_ver << 4) | minor_ver;
        goto fail;
    }

    return 0;
    fail:
    printk("[SB16] Failed to reset! Status: %x\n", status);
    return 1;
}

void config_sb16(){
    interrupt_add_handle(32+SB16_IRQ, sb16_interrupt_handler);
    outb(DSP_MIXER_PORT, DSP_IRQ);
    outb(DSP_MIXER_DATA_PORT, SB16_IRQ_DATA);
}

void sb16_transfer(void *buf, uint32_t len){
    uint8_t mode = 0x48;

    outb(DSP_ON_8, 4+(DMA_CHANNEL_16 % 4));

    outb(DMA_FLIP_FLOP, 1);

    outb(DSP_ON_16, (DMA_CHANNEL_16 % 4) | mode | (1 << 4));

    uint16_t offset = (((uint32_t) buf) / 2) % 65536;
    outb(DMA_BASE_ADDR, (uint8_t) ((offset >> 0) & 0xFF));
    outb(DMA_BASE_ADDR, (uint8_t) ((offset >> 8) & 0xFF));

    outb(DMA_COUNT, (uint8_t) ((len - 1) & 0xFF));
    outb(DMA_COUNT, (uint8_t) (((len - 1) >> 8) & 0xFF));

    outb(0x8B, ((uint32_t) buf) >> 16);

    outb(0xD4, DMA_CHANNEL_16 % 4);
}

void dsp_write(uint8_t b) {
    while (inb(DSP_WRITE) & 0x80);
    outb(DSP_WRITE, b);
}

void dsp_read(uint8_t b){
    while (inb(DSP_READ_STS) & 0x80);
    outb(DSP_READ, b);
}

void sb16_set_sample_rate(uint16_t freq){
    dsp_write(DSP_SET_RATE);
    dsp_write((uint8_t) ((freq >> 8) & 0xFF));
    dsp_write((uint8_t) (freq & 0xFF));
}

struct cpu_state sb16_interrupt_handler(struct cpu_state cpu __attribute__((unused)), struct stack_state stack __attribute__((unused))){
    printk("[SB16] Interrupt!\n");

    inb(DSP_READ_STS);
    inb(DSP_16_INT_ACK);

    return cpu;
}


void init_soundblaster(){
    interrupt_add_handle(32+SB16_IRQ, sb16_interrupt_handler);
    if(reset_sb16()){
        printk("[SB16] Init fail\n");
        return;
    }
    sb16_buffer = (uint16_t *) malloc(BUFFER_SIZE);
    config_sb16();

    sb16_transfer(sb16_buffer, BUFFER_SIZE);
    sb16_set_sample_rate(SAMPLE_RATE);

    uint16_t sample_count = (BUFFER_SIZE / 2) - 1;

    dsp_write(DSP_PLAY | DSP_PROG_16 | DSP_AUTO_INIT);
    dsp_write(DSP_PLAY | DSP_PROG_16 | DSP_AUTO_INIT);
    dsp_write(DSP_SIGNED | DSP_MONO);
    dsp_write((uint8_t) ((sample_count >> 0) & 0xFF));
    dsp_write((uint8_t) ((sample_count >> 8) & 0xFF));

    dsp_write(DSP_ON);
    dsp_write(DSP_ON_16);

    printk("[SB16] Initialized\n");
}