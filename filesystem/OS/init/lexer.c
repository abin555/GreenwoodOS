#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

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

    //printf("Lexing\n%s\n\nStart:\n", s);
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
            }
            else{
                err = 1;
                printf("Did not find ending \" for string.\n");
            }
        }
        if(is_varkeyword(start) && isBlankSpace(*(start+sizeof(var_keyword)-1))){//match for VAR (variable keyword)
            stream.tokens[stream.ntokens++] = type_createVar();
            start += sizeof(var_keyword)-1;
            continue;
        }
        if(is_execkeyword(start) && isBlankSpace(*(start+sizeof(exec_keyword)-1))){//match for EXEC (execute keyword)
            stream.tokens[stream.ntokens++] = type_createExec();
            start += sizeof(exec_keyword)-1;
            continue;
        }
        if(is_wexeckeyword(start) && isBlankSpace(*(start+sizeof(wexec_keyword)-1))){//match for EXEC (execute keyword)
            stream.tokens[stream.ntokens++] = type_createWExec();
            start += sizeof(wexec_keyword)-1;
            continue;
        }
        if(isLetterOrNumber(*start)){
            char *iden_start = start;
            char *iden_end = iden_start;
            while(iden_end < end && isLetterOrNumber(*iden_end)){
                iden_end++;
            }
            int len = iden_end - iden_start;
            char *iden_buf = malloc(len+1);
            memset(iden_buf, 0, len+1);
            strncpy(iden_buf, iden_start, len);
            stream.tokens[stream.ntokens++] = type_createIden(iden_buf);
            start += len;
            continue;
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
            case VAR:
                printf("VAR\n");
                break;
            case EXEC:
                printf("EXEC\n");
                break;
            case WEXEC:
                printf("EXEC\n");
                break;
            case STR:
                printf("STR \"%s\"\n", s.tokens[i].str);
                break;
            case IDEN:
                printf("IDEN %s\n", s.tokens[i].iden);
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