#ifndef GWOS_LIB_H
#define GWOS_LIB_H

void init_heap(int heap_size);
void *malloc(int size);
void free(void *mem);
void *os_malloc(int size);
void os_free(void *mem);


void print_str(char *str, char *arg);
void print_int(char *str, int arg);
void pchar(char c);

int strlen(const char *s);
int strtol(const char *nptr, char **endptr, int base);
int strspn(const char *s, const char *accept);
int strcspn(const char *s, const char *reject);
char *strchr(const char *s, int ch);
char *strdup(const char *s);

int toupper(int c);
void memset(void *mem, char v, int size);
#endif