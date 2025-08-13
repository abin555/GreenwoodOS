#include <ctype.h>
#include "greenwood-lisp.h"

void print_expr(Atom atom){
    switch(atom.type){
        case Atom_NIL:
            puts("NIL");
            break;
        case Atom_PAIR:
            putchar('(');
            print_expr(car(atom));
            atom = cdr(atom);
            while(!(nilp(atom))){
                if(atom.type == Atom_PAIR){
                    putchar(' ');
                    print_expr(car(atom));
                    atom = cdr(atom);
                }
                else{
                    puts(" . ");
                    print_expr(atom);
                    break;
                }
            }
            putchar(')');
            break;
        case Atom_SYMBOL:
            printf("%s", (char *) atom.value.symbol);
            break;
        case Atom_INT:
            printf("%d", atom.value.integer);
            break;
        case Atom_BUILTIN:
            printf("#<BUILTIN: 0x%x>", (uint32_t) atom.value.builtin);
            break;
        case Atom_STRING:
            printf("\"%s\"", atom.value.string);
            break;
        case Atom_REAL:
            printf("%d", atom.value.real);
            printf(".%d", (atom.value.real - ((int) atom.value.real)) * 100);
            break;
        case Atom_CLOSURE:
            puts("<Closure Fn>");
            break;
        case Atom_MACRO:
            puts("<MACRO>");
            break;
    }
    //putchar('\n');
}
