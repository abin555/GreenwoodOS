#include <ctype.h>
#include <limits.h>

/*
int (isalnum)(int c) {
	return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}

int (isalpha)(int c){
	return (_Ctype[c] & (_LO|_UP|_XA));
}

int (iscntrl)(int c){
	return (_Ctype[c] & (_BB|_CN));
}

int (isdigit)(int c) {
	return (_Ctype[c] & (_DI));
}
int (isgraph)(int c) {
	return (_Ctype[c] & (_DI|_LO|_PU|_UP|_XA));
}
int (islower)(int c) {
	return (_Ctype[c] & (_LO));
}
int (isprint)(int c) {
	return (_Ctype[c] & (_DI|_LO|_PU|_SP|_UP|_XA));
}
int (ispunct)(int c) {
	return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}
int (isspace)(int c) {
	return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}
int (isupper)(int c) {
	return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}
int (isxdigit)(int c) {
	return (_Ctype[c] & (_DI|_LO|_UP|_XA));
}
*/

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