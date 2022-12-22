#include "utilities.h"

int check_str_equ(char *str1, char *str2){
    while(*str1){
        if(*str1 != *str2){
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}