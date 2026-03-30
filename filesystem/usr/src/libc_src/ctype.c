#include <ctype.h>
#include <limits.h>

int isdigit(int c){
	if(c <= '9' && c >= '0') return 1;
	return 0;
}
int isalpha(int c){
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return 1;
	return 0;
}
int isupper(int c){
	if(c >= 'A' && c <= 'Z') return 1;
	return 0;
}

int tolower(int c){
	if(c >= 'A' && c <= 'Z')
		return c - 'A';
	else
		return c;
}

int toupper(int c){
	if(c >= 'a' && c <= 'z')
		return c - ('a' - 'A');
	else
		return c;
}

int isspace(int c){
	return c == ' '  ||
           c == '\t' ||
           c == '\n' ||
           c == '\r' ||
           c == '\f' ||
           c == '\v';
}

int isxdigit(int c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

int ispunct(int c)
{
    return (c >= '!' && c <= '/') ||
           (c >= ':' && c <= '@') ||
           (c >= '[' && c <= '`') ||
           (c >= '{' && c <= '~');
}
