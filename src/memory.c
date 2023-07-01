#include "memory.h"

uint32_t HEAP_BEGIN = 0;
uint32_t HEAP_END = 0;
uint32_t MEMORY_USED = 0;
uint32_t HEAP_BLOCK_SIZE = 0x1;

void memcpy(void* source, void* target, uint32_t size){
    asm volatile ("rep movsb"
    : "=D" (target),
    "=S" (source),
    "=c" (size)
    : "0" (target),
    "1" (source),
    "2" (size)
    : "memory");
}

void memset(void* target, int value, uint32_t size){
    uint8_t val = (uint8_t)(value & 0xFF);
    uint8_t *dest = (uint8_t*)target;

    uint32_t i = 0;
    while(i < size){
        dest[i] = val;
        i++;
    }
}

void* malloc(unsigned int size){
    uint32_t required_blocks = (size / HEAP_BLOCK_SIZE) + 1;
    print_serial("Malloc for size 0x%x requiring 0x%x blocks\n", size, required_blocks);

    uint32_t count_consecutive_unused = 0;
    uint32_t start_block_idx = 0;
    bool start_set = false;
    for(int block_idx = 0; block_idx < HEAP_NUMBER_OF_BLOCKS; block_idx++){
        uint8_t block_entry_byte = HEAP_ALLOC_TABLE[block_idx / 8];
        uint8_t block_entry = (block_entry_byte >> (block_idx % 8)) & 0b1;
        if(block_entry == 0){
            if(!start_set){
                start_block_idx = block_idx;
                start_set = true;
            }
            count_consecutive_unused++;
        }
        else if(block_entry == 1){
            count_consecutive_unused = 0;
            start_set = false;
        }

        if(count_consecutive_unused == required_blocks){
            print_serial("Found consecutive blocks starting at idx 0x%x\n", start_block_idx);
            break; 
        }
    }
    for(uint32_t block_idx = start_block_idx; block_idx < start_block_idx+required_blocks; block_idx++){
        uint8_t block_entry_byte = HEAP_ALLOC_TABLE[block_idx / 8];
        //uint8_t block_entry_byte = 0x12;
        block_entry_byte = (1 << (block_idx % 8)) | block_entry_byte;
        HEAP_ALLOC_TABLE[(block_idx / 8)] = block_entry_byte;
        //HEAP_ALLOC_TABLE[0] = 0xFF;
        print_serial("Setting Table byte 0x%x at 0x%x to 0x%x 0x%x 0x%x\n", block_idx / 8, block_idx, block_entry_byte, (1 << (block_idx % 8)), HEAP_ALLOC_TABLE[block_idx / 8]);
    }

    uint32_t address = (start_block_idx * HEAP_BLOCK_SIZE) + HEAP_BEGIN;
    print_serial("Malloc address is 0x%x\n", address);

    return (void*) address;
}

void free(void* memory){
    print_serial("Freeing Memory 0x%x\n", (uint32_t) memory);
    memory++;
}

unsigned int mgetSize(void *mem){
    alloc_t* alloc = (alloc_t *) (mem - sizeof(alloc_t));
    return alloc->size;
}

void initialize_heap(uint32_t heap_begin, uint32_t heap_size){
    fb_write_xy("HEAP START!", 11, 0, 0, 0xFFFFFF, 0);
    for(uint32_t i = 0; i < (heap_size / 0x400000); i++){
        create_page_entry(heap_begin+(i*0x400000), heap_begin+(i*0x400000), 0x83);
    }
    fb_write_xy("HEAP PAGE! ", 11, 0, 0, 0xFFFFFF, 0);
    HEAP_BEGIN = heap_begin;
    HEAP_END = HEAP_BEGIN + heap_size;
    HEAP_BLOCK_SIZE = heap_size / HEAP_NUMBER_OF_BLOCKS;
    print_serial("Heap block size is 0x%x with 0x%x blocks and 0x%x available memory\nMalloc Block table is 0x%x bytes at 0x%x\n", HEAP_BLOCK_SIZE, HEAP_NUMBER_OF_BLOCKS, heap_size, sizeof(HEAP_ALLOC_TABLE), &HEAP_ALLOC_TABLE);
    memset((char *) HEAP_BEGIN, 0, heap_size);
    memset(HEAP_ALLOC_TABLE, 0, (HEAP_NUMBER_OF_BLOCKS / 8));
    fb_write_xy("HEAP READY ", 11, 0, 0, 0xFFFFFF, 0);
}

void mem_dump(){
    //uint32_t mem = (uint32_t) HEAP_BEGIN;
    printk("Kernel Heap Dump:\n");
    printk("_____MEM | _Address | ____Size | __Status\n");

}