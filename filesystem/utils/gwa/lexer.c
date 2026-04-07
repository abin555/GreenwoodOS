#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

struct TokenStream lex_file(FILE *file){
    struct TokenStream stream = {0, NULL, 0};
    if(file == NULL) return stream;

    size_t size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    stream.max_tokens = 1000;
    stream.tokens = malloc(sizeof(TOKEN_t) * stream.max_tokens);

    char linebuf[1000];
    int line_idx = 0;
    int f_idx = 0;
    TOKEN_TYPE_t tracking = NONE;
    while(f_idx < size){
        char c;
        fread(&c, 1, 1, file);
        if(isdigit(c)){
            if(tracking == NONE) tracking = DECIMAL;
        }
    }
}