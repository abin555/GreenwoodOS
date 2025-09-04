#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/task.h>

#include "lexer.h"

struct Variable {
    char *name;
    Symbol_t sym;
};

struct Execute {
    char should_wait;
    char *prog;
    int argc;
    char **argv;
};

#define MAXVARS 20
#define MAXEXECS 20
struct Program {
    int num_variables;
    int numExecutes;
    
    struct Variable vars[MAXVARS];
    struct Execute execs[MAXEXECS];
};

Symbol_t prog_getVariable(struct Program *p, char *name){
    if(p == NULL || name == NULL) return type_createUnkn();
    for(int i = 0; i < p->num_variables; i++){
        if(!strcmp(name, p->vars[i].name)){
            return p->vars[i].sym;
        }
    }
    return type_createUnkn();
}

int prog_addVariable(struct Program *p, char *name, Symbol_t s){
    if(p == NULL || name == NULL) return 1;
    if(p->num_variables >= MAXVARS) return 1;
    p->vars[p->num_variables++] = (struct Variable) {
        name, s
    };
    printf("Added Variable %s = (%d) %s\n", name, s.type, s.str);
    return 0;
}

void parse(struct Program *p, struct TokenStream *s){
    for(int hidx = 0; hidx < s->ntokens; hidx++){
        printf("HIDX: %d - %d\n", hidx, s->tokens[hidx].type);
        if(s->tokens[hidx].type == VAR && hidx <= s->ntokens - 4){//Create Variable
            if(
                s->tokens[hidx + 1].type == IDEN &&
                s->tokens[hidx + 2].type == STR &&
                s->tokens[hidx + 3].type == ENDL
            ){
                prog_addVariable(p, s->tokens[hidx+1].iden, s->tokens[hidx+2]);
                hidx+=3;
            }
            else{
                printf("Error, VAR definition incorrect.");
                break;
            }
        }
        if((s->tokens[hidx].type == EXEC || s->tokens[hidx].type == WEXEC) && hidx <= s->ntokens - 2){//Execute Calls.
            SYMS_e exec_type = s->tokens[hidx].type;
            if(
                s->tokens[hidx + 1].type == IDEN ||
                s->tokens[hidx + 1].type == STR
            ){
                struct Execute exec = {
                    (exec_type == WEXEC) ? 1 : 0,
                    NULL,
                    0,
                    NULL
                };
                hidx++;
                if(s->tokens[hidx].type == IDEN){//Need to resolve a variable for program name.
                    printf("EXEC program - Looking for variable %s\n", exec.argc, s->tokens[hidx].iden);
                    Symbol_t sym = prog_getVariable(p, s->tokens[hidx].iden);
                    if(sym.type == STR){
                        printf("Found: \"%s\"\n", sym.str);
                        exec.prog = sym.str;
                    }
                    else{
                        printf("Error resolving type in EXEC, expected string got %s\n", typeToStr(sym.type));
                        break;
                    }                    
                }
                else if(s->tokens[hidx].type == STR){
                    exec.prog = s->tokens[hidx].str;
                }
                printf("Exec Program: %s\n", exec.prog);

                exec.argc = 1;
                exec.argv = malloc(sizeof(char *));
                exec.argv[0] = exec.prog;

                hidx++;
                while(s->tokens[hidx].type != ENDL){//Consume Args.
                    if(s->tokens[hidx].type == STR){
                        exec.argv = realloc(exec.argv, sizeof(char *) * exec.argc);
                        exec.argv[exec.argc++] = s->tokens[hidx].str;
                    }
                    else if(s->tokens[hidx].type == IDEN){
                        printf("EXEC arg #%d - Looking for variable %s\n", exec.argc, s->tokens[hidx].iden);
                        Symbol_t sym = prog_getVariable(p, s->tokens[hidx].iden);
                        if(sym.type == STR){  
                            printf("Found: \"%s\"\n", sym.str);
                            exec.argv = realloc(exec.argv, sizeof(char *) * exec.argc);
                            exec.argv[exec.argc++] = sym.str;
                        }
                        else{
                            printf("Error resolving variable type in EXEC, expected string got %s\n", typeToStr(sym.type));
                            break;
                        }
                    }
                    else{
                        printf("Warn: unknown type in exec args. Got %s\n", typeToStr(s->tokens[hidx].type == IDEN));
                        break;
                    }
                    hidx++;
                }

                p->execs[p->numExecutes++] = exec;
            }
            else{
                printf("Error, VAR definition incorrect.");
                break;
            }
        }
    }
}

void run(struct Program *p){
    if(p == NULL) return;
    for(int i = 0; i < p->numExecutes; i++){
        int pid = exec(p->execs[i].prog, p->execs[i].argc, p->execs[i].argv);
        printf("[INIT] Exec'd with PID %d\n", pid);
        if(p->execs[i].should_wait){
            waitpid(pid);
            printf("[INIT] Wait complete!\n");
        }
        else{
            yield();
        }
    }
}

int main(int argc, char **argv){
    freopen("/-/dev/serial", "w", stdout);
    FILE *autostart = fopen("/A/OS/autostart.ini", "r");
    if(autostart == NULL){
        printf("[INIT] Unable to open autostart.ini file!\n");
        return 1;
    }

    fseek(autostart, 0, SEEK_END);
    int size = ftell(autostart);
    char *text = malloc(size);
    fseek(autostart, 0, SEEK_SET);
    fread(text, size, 1, autostart);
    fclose(autostart);

    printf("Autostart File:\n%s\n", text);

    struct TokenStream s = lex(text, size);

    printf("Token Parse\n");
    printTokenStream(s);

    struct Program p = {
        0, 0
    };

    parse(&p, &s);
    printf("There are %d variables and %d exec statements\n", p.num_variables, p.numExecutes);
    run(&p);

    return 0;
}