#ifndef _STDLIB_H
#define _STDLIB_H

#ifndef _SIZET
#define _SIZET
typedef long unsigned int size_t;
#endif

#ifndef NULL
#define NULL 0
#endif

void abort(void);
void *malloc(size_t size);
void free(void *);
void *calloc(size_t n, size_t size);
void *realloc(void  *ptr, size_t size);
void *reallocarray(void *ptr, size_t n, size_t size);
char *getenv(const char *name);
float atof(const char *arr);
int atoi(const char *nptr);
int abs(int i);
char *itoa(int n, char *buf, int radix);
void exit(int code);
void srand(unsigned int seed);
int rand();
int strtol(const char *nptr, char **endptr, register int base);

#endif