#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum {
    NONE,
    ENDL,
    COLON,
    COMMA,
    DECIMAL,
    HEXADECIMAL,
    FLOAT,
    STRING,
    KEYWORD
} TOKEN_TYPE_t;

typedef struct Token {
    TOKEN_TYPE_t type;
    union {
        int integer;
        unsigned int hexadecimal;
        float float_v;
        char *string;
        char *keyword;
    } data;
} TOKEN_t;

struct TokenStream {
    int count;
    TOKEN_t *tokens;
    int max_tokens;
};

struct TokenStream lex_file(FILE *file);
void lex_printStream(struct TokenStream *stream);

#endif