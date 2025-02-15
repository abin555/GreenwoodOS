#ifndef _STDIO_H
#define _STDIO_H

#define stdin ((FILE *) 0)
#define stdout ((FILE *) 0)
#define stderr ((FILE *) 0)

struct FILE;
typedef struct FILE FILE;

int fprintf(FILE *, const char *, ...);
int fflush(FILE *);

#endif