#ifndef _STDLIB_H
#define _STDLIB_H

#ifndef _SIZET
#define _SIZET
typedef long unsigned int size_t;
#endif

void abort(void);
void *malloc(size_t size);
void free(void *);

#endif