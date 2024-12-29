#ifndef UTILS_H
#define UTILS_H

typedef unsigned int size_t;
typedef unsigned int* uintptr_t;
typedef void* ptr_t;

void *malloc(int size);
void free(void *mem);
void *calloc(size_t nmemb, size_t size);

int memcmp(const void *s1, const void *s2, size_t n);
void *memset (void *dest, register int val, register size_t len);
void *memcpy(void *dest, const void *src, int n);
void *realloc(void *ptr, size_t size);
int strncmp(const char *s1, const char *s2, register size_t n);
size_t strlen (const char *str);
char *strcpy(char *dest, const char *src);
char *strtok_r (char *s, const char *delim, char **save_ptr);
size_t strspn (const char *str, const char *accept);
size_t strcspn (const char *str, const char *reject);
char *strchr (const char *s, int c_in);
#endif