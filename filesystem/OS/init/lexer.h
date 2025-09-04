#ifndef LEXER_H
#define LEXER_H

#include "types.h"

#define MaxSymbols 100

struct TokenStream {
    int ntokens;
    char valid;
    Symbol_t *tokens;
};

struct TokenStream lex(char *s, int len);
void printTokenStream(struct TokenStream s);

#endif