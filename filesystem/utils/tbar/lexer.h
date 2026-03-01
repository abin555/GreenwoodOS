#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
    STR,
    ENDL,
    UNKN
} SYMS_e;

typedef struct Symbol {
    SYMS_e type;
    union {
        char *str;
    };
} Symbol_t;


Symbol_t type_createStr(char *str);
Symbol_t type_createEndl();
Symbol_t type_createUnkn();

char *typeToStr(SYMS_e t);

#define MaxSymbols 100

struct TokenStream {
    int ntokens;
    char valid;
    Symbol_t *tokens;
};

struct TokenStream lex(char *s, int len);
void printTokenStream(struct TokenStream s);

#endif