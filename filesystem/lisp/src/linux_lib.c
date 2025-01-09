#include "linux_lib.h"

void *os_malloc(size_t size){
    return malloc(size);
}

void os_free(void *block){
    free(block);
}

void print(char *str){
    printf(str);
}

void print_str(char *str, char *arg){
    printf(str, arg);
}

void print_int(char *str, int arg){
    printf(str, arg);
}

void pchar(char c){
    putchar(c);
}

int toupper(int c){
    if((c >= 65 && c <= 90) || c >= 97 && c <= 122)
        return c & ~(0b00100000);
    return c;
}