#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

Symbol_t type_createStr(char *str){
    Symbol_t s = {STR, str};
    return s;
}

Symbol_t type_createEndl(){
    Symbol_t s = {ENDL, NULL};
    return s;
}

Symbol_t type_createUnkn(){
    Symbol_t s = {UNKN, NULL};
    return s;
}


char *typeToStr(SYMS_e t){
    switch(t){
        case STR:
            return "STR";
        case ENDL:
            return "ENDL";
        case UNKN:
        default:
            return "UNKN";
    }
}

char isEndLine(char c){
    return (c == '\n' || c == '\0');
}

char isBlankSpace(char c){
    if(c == ' ' || c == '\n' || c == '\t' || c == '\0') return true;
    return false;
}

char isLetterOrNumber(char c){
    if(
        (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z')
    ) return true;
    return false;
}

struct TokenStream lex(char *s, int len){
    struct TokenStream stream;
    stream.ntokens = 0;
    stream.valid = 0;
    stream.tokens = NULL;

    if(s == NULL || len <= 0){
        printf("[LEXER] Error: invalid input (s=%p, len=%d)\n", (void *)s, len);
        return stream;
    }

    stream.tokens = malloc(sizeof(Symbol_t) * MaxSymbols);
    if(stream.tokens == NULL){
        printf("[LEXER] Out of memory\n");
        return stream;
    }

    printf("[LEXER] Lexing %d bytes\n", len);
    int err = 0;

    char *start = s;
    char *end = s + len;

    while(start < end && *start != '\0'){
        // Bug fix: the original code had no limit here and would write past
        // the end of the fixed-size token buffer if a config had more than
        // ~100 tokens. Now we stop gracefully instead of corrupting memory.
        if(stream.ntokens >= MaxSymbols){
            printf("[LEXER] Token limit (%d) reached, truncating input\n", MaxSymbols);
            break;
        }

        if(*start == ' ' || *start == '\t'){
            start++;
            continue;
        }

        if(*start == '#'){ // Comment: skip to end of line (bounds-checked).
            while(start < end && !isEndLine(*start)) start++;
            continue;
        }

        if(*start == '\n'){
            stream.tokens[stream.ntokens++] = type_createEndl();
            start++;
            continue;
        }

        if(*start == '\"'){
            char *content_start = start + 1;
            char *content_end = content_start;
            while(content_end < end && *content_end != '\"' && *content_end != '\0'){
                content_end++;
            }

            if(content_end < end && *content_end == '\"'){
                int slen = content_end - content_start;
                char *str_buf = malloc(slen + 1);
                if(str_buf == NULL){
                    printf("[LEXER] Out of memory\n");
                    err = 1;
                    break;
                }
                memcpy(str_buf, content_start, slen);
                str_buf[slen] = '\0';
                stream.tokens[stream.ntokens++] = type_createStr(str_buf);
                start = content_end + 1; // Skip past the closing quote.
            } else {
                err = 1;
                printf("[LEXER] Did not find closing \" for string\n");
                start = end; // Bail out of the file.
            }
            continue;
        }

        start++; // Unrecognized character - skip it.
    }

    if(stream.ntokens < MaxSymbols){
        stream.tokens[stream.ntokens++] = type_createEndl();
    }

    printf("[LEXER] Identified %d tokens\n", stream.ntokens);
    stream.valid = !err;
    return stream;
}

void freeTokenStream(struct TokenStream *s){
    if(s == NULL || s->tokens == NULL) return;
    for(int i = 0; i < s->ntokens; i++){
        if(s->tokens[i].type == STR && s->tokens[i].str != NULL){
            free(s->tokens[i].str);
        }
    }
    free(s->tokens);
    s->tokens = NULL;
    s->ntokens = 0;
}

void printTokenStream(struct TokenStream s){
    for(int i = 0; i < s.ntokens; i++){
        switch(s.tokens[i].type){
            case STR:
                printf("STR \"%s\"\n", s.tokens[i].str);
                break;
            case ENDL:
                printf("ENDL\n");
                break;
            default:
                printf("UNKN\n");
                break;
        }
    }
}
