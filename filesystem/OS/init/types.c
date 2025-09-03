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

