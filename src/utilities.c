#include "utilities.h"

int check_str_equ(char *str1, char *str2){
    while(*str1){
        if(*str1 != *str2){
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

uint32_t get_bitfield(uint32_t field, uint32_t bit){
    uint32_t val = field & bit;
    while((bit & 1) == 0){
        val >>= 1;
        bit >>= 1;
    }
    return val;
}

int calculate_shift(uint32_t field){
    int shift = 0;
    while((field & 1) == 0 && shift < 32){
        field >>= 1;
        shift++;
    }
    return shift;
}

uint8_t get_8_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint8_t *) address);
}

uint16_t get_16_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint16_t *) address);
}

uint32_t get_32_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return *((uint32_t *) address);
}

uint32_t get_24_offset(uint32_t BAR, uint32_t offset){
    uint32_t address = BAR + offset;
    return (*((uint32_t *) address) & 0x00FFFFFF);
}

void set_8_offset(uint32_t BAR, uint32_t offset, uint8_t val){
    uint32_t address = BAR + offset;
    uint8_t *reg = (uint8_t *) address;
    *reg = val; 
}

void set_16_offset(uint32_t BAR, uint32_t offset, uint16_t val){
    uint32_t address = BAR + offset;
    uint16_t *reg = (uint16_t *) address;
    *reg = val; 
}

void set_32_offset(uint32_t BAR, uint32_t offset, uint32_t val){
    uint32_t address = BAR + offset;
    uint32_t *reg = (uint32_t *) address;
    *reg = val;
}