#ifndef _STDIO_H
#define _STDIO_H

#ifndef _STD_fileListing_h
#define _STD_fileListing_h
extern struct FILE **fileListing;
#endif

#define stdin ((FILE *) fileListing[0])
#define stdout ((FILE *) fileListing[1])
#define stderr ((FILE *) fileListing[2])

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct FILE;
typedef struct FILE FILE;

#ifndef _SIZET
#define _SIZET
typedef long unsigned int size_t;
#endif

FILE *fopen(const char *pathname, const char *mode);
int fprintf(FILE *, const char *, ...);
int fflush(FILE *);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
int fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t n, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
int feof(FILE *stream);

int printf(const char *format, ...);
int sprintf(char *str, const char *format, ...);
int fputs(const char *s, FILE *file);
int puts(const char *s);

int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);

FILE *freopen(const char *pathname, const char *mode, FILE *stream);

int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);

#include <stdarg.h>
int vfprintf(FILE *stream, const char *format, va_list apq);

#endif