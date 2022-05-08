#ifndef STRING_H
#define STRING_H

#include "ascii_tables.h"

char STR_edit[128];

int STR_Compare(char *elem1, char *elem2, int start, int end);

void STR_INSERT(char *in_str, char *out_str, int len, int write_index);

void decodeData(char *Buffer, int in, int len, int start);

void decodeHex(char *Buffer, int in, int len, int start);
void decodeInt(char *Buffer, int in, int len, int start);

unsigned int encodeHex(char *Buffer, int start, int end);

char quadToHex(char quad);
char hexToQuad(char hex);
void strcpy(char *source, char *destination, unsigned int len);
#endif