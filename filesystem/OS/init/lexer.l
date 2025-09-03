%{
#include "y.tab.h"
#include <string.h>
%}

%%
"VAR"   { return VAR; }
"EXEC"  { return EXEC; }

[a-zA-Z_][a-zA-Z0-9_]*   {
                           yylval.str = strdup(yytext);
                           return IDENTIFIER;
                        }

\"([^\\\"]|\\.)*\"  {
                       char *str = strdup(yytext);
                       str[strlen(str)-1] = '\0';  /* remove trailing quote */
                       yylval.str = strdup(str+1); /* remove leading quote */
                       free(str);
                       return STRING;
                    }

[ \t\r\n]+      { /* ignore */ }

.               { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}