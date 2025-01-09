#ifndef linux_lib_H
#define linux_lib_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *os_malloc(size_t size);

void os_free(void *block);

void print(char *str);
void print_str(char *str, char *arg);
void print_int(char *str, int arg);
void pchar(char c);

int toupper(int c);



#endif
