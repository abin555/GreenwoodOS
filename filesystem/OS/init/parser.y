%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

/* Define semantic value types */
%union {
    char *str;
}

%token VAR EXEC
%token <str> IDENTIFIER STRING

%%

program
    : /*empty*/
    | program statement
    ;

statement
    : var_statement
    | exec_statement
    ;

var_statement
    : VAR IDENTIFIER STRING
        { printf("VAR %s = %s\n", $2, $3); }
    ;

exec_statement
    : EXEC exec_target exec_args
        { printf("EXEC statement parsed.\n"); }
    ;

exec_target
    : IDENTIFIER
    | STRING
    ;

exec_args
    : /*empty*/
    | exec_args exec_arg
    ;

exec_arg
    : IDENTIFIER
        { printf(" arg: %s\n", $1); }
    | STRING
        { printf(" arg: %s\n", $1); }
    ;

%%

void yyerror(const char *s){
    printf("Parse Error: %s\n", s);
}