#include <stdio.h>
#include <string.h>
#include "types.h"

Symbol_t type_createVar(){
    Symbol_t s = {VAR, NULL};
    return s;
}

Symbol_t type_createExec(){
    Symbol_t s = {EXEC, NULL};
    return s;
}

Symbol_t type_createStr(char *str){
    Symbol_t s = {STR, str};
    return s;
}

Symbol_t type_createIden(char *iden){
    Symbol_t s = {IDEN, iden};
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

bool is_varkeyword(char *s){
    return !strncmp(s, var_keyword, 3);
}

bool is_execkeyword(char *s){
    return !strncmp(s, exec_keyword, 4);
}

char *typeToStr(SYMS_e t){
    switch(t){
        case VAR:
            return "VAR";
        case EXEC:
            return "EXEC";
        case STR:
            return "STR";
        case IDEN:
            return "IDEN";
        case ENDL:
            return "ENDL";
        case UNKN:
        default:
            return "UNKN";
    }
}