#include <ctype.h>
#include <limits.h>

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isalpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int isspace(int c) {
    return c == ' '  || c == '\t' || c == '\n' ||
           c == '\r' || c == '\f' || c == '\v';
}

int isxdigit(int c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

int ispunct(int c) {
    return (c >= '!' && c <= '/') ||
           (c >= ':' && c <= '@') ||
           (c >= '[' && c <= '`') ||
           (c >= '{' && c <= '~');
}

int iscntrl(int c) {
    return (c >= 0 && c <= 31) || c == 127;
}

int isgraph(int c) {
    return c > ' ' && c <= '~';
}

int isprint(int c) {
    return c >= ' ' && c <= '~';
}

int tolower(int c) {
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');  /* was: c - 'A'  ← the bug */
    return c;
}

int toupper(int c) {
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');
    return c;
}