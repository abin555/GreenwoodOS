#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define var_keyword "VAR"
#define exec_keyword "EXEC"

typedef enum {
    VAR,
    EXEC,
    STR,
    IDEN
} SYMS_e;

typedef struct Symbol {
    SYMS_e type;
    union {
        char *str;
        char *iden;
    };
} Symbol_t;

Symbol_t type_createVar();
Symbol_t type_createExec();
Symbol_t type_createStr(char *str);
Symbol_t type_createIden(char *iden);

bool is_varkeyword(char *s);
bool is_execkeyword(char *s);

#endif