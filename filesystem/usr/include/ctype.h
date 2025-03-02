#ifndef _CTYPE_H
#define _CTYPE_H

#define _XA 0x200
#define _XS 0x100
#define _BB 0x80
#define _CN 0x40
#define _DI 0x20
#define _LO 0x10
#define _PU 0x08
#define _SP 0x04
#define _UP 0x02
#define _XD 0x01

int isalnum(int), isalpha(int), iscntrl(int), isdigit(int);
int isgraph(int), islower(int), isprint(int), ispunct(int);
int isspace(int), isupper(int), isxdigit(int);
int tolower(int), toupper(int);

extern const short *_Ctype, *_Tolower, *_Toupper;

#define isalnum(c) (_Ctype[(int) c] & (_DI|_LO|_UP|_XA))
#define isalpha(c) (_Ctype[(int) c] & (_LO|_UP|_XA))
#define iscntrl(c) (_Ctype[(int) c] & (_BB|_CN))
#define isdigit(c) (_Ctype[(int) c] & (_DI))
#define isgraph(c) (_Ctype[(int) c] & (_DI|_LO|_PU|_UP|XA))
#define islower(c) (_Ctype[(int) c] & (_LO))
#define isprint(c) (_Ctype[(int) c] & (_DI|_LO|_PU|SP|_UP|_XA))
#define ispunct(c) (_Ctype[(int) c] & (_DI|_LO|_UP|_XA))
#define isspace(c) (_Ctype[(int) c] & (_DI|_LO|_UP|_XA))
#define isupper(c) (_Ctype[(int) c] & (_DI|_LO|_UP|_XA))
#define isxdigit(c) (_Ctype[(int) c] & (_DI|_LO|_UP|_XA))

#endif