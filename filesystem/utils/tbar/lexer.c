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
    return (c == '\n' || c== '\0');
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
    stream.tokens = malloc(sizeof(Symbol_t) * MaxSymbols);

    if(s == NULL || len == 0){
        printf("Error, s = 0x%x, len = %d\n", s, len);
        return stream;
    }

    printf("Lexing %d bytes\n%s\n\nStart:\n", len, s);
    int err = 0;

    char *start = s;
    char *end = s+len;
    int i = 0;
    while(start < end && *start != '\0'){
        i++;
        if(*start == ' '){
            start++;
            continue;
        }
        if(*start == '#'){//Ignore comments
            while(!isEndLine(*start++)){}
            start--;//Backup so that the line end can be consumed.
            continue;
        }
        if(*start == '\n'){//Match for ENDL (endline)
            stream.tokens[stream.ntokens++] = type_createEndl();
            start++;
            continue;
        }
        if(*start == '\"'){//Match for strings.
            start++;
            char *str_start = start;
            char *str_end = str_start+1;
            int end_found = 0;
            while(str_end < end && *str_end != '\0'){
                start++;
                if(*str_end == '\"'){
                    end_found = 1;
                    break;
                }
                str_end++;
            }
            if(end_found){
                int len = str_end - str_start;
                char *str_buf = malloc(len+1);
                memset(str_buf, 0, len+1);
                strncpy(str_buf, str_start, len);
                stream.tokens[stream.ntokens++] = type_createStr(str_buf);
                printf("STR \"%s\"\n", str_buf);
            }
            else{
                err = 1;
                printf("Did not find ending \" for string.\n");
            }
        }
        start++;
    }
    stream.tokens[stream.ntokens++] = type_createEndl();
    printf("There were %d tokens identified\n", stream.ntokens);
    if(!err){
        stream.valid = 1;
    }
    return stream;
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