#include "utils.h"
#include "allocator.h"
#include <stdarg.h>

int strcmp(const char *s1, const char *s2){
	while(*s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *(const unsigned char *)s1 - *(const unsigned char*)s2;
}

int strlen(char *s){
	if(s == NULL) return 0;
	int len = 0;
	while(*s++) len++;
	return len;
}

char *strdup(char *s){
	if(s == NULL) return NULL;
	int len = strlen(s);
	char *newS = malloc(len+1);
	for(int i = 0; i < len; i++){
		newS[i] = s[i];
	}
	newS[len] = '\0';
	return newS;
}

char *strndup(char *s, int n){
	if(s == NULL){
		print_serial("[STRNCPY] S is null\n");
		return NULL;
	}
	int len = strlen(s);
	if(len > n) len = n;
	char *newS = malloc(len+1);
	for(int i = 0; i < len; i++){
		newS[i] = s[i];
	}
	newS[len] = '\0';
	print_serial("[STRNCPY] Copied %d chars %s -> %s\n", len, s, newS);
	return newS;
}

int strncmp( const char * s1, const char * s2, size_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

char* itoa(int k, char *buf, int radix){
    int i = k < 0 ? -k : k;
    if (i == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    int idx = k < 0 ? 1 : 0;
    int j = i;
    while (j)
    {
        j /= radix;
        idx++;
    }
    buf[idx] = '\0';

    if (radix == 10)
    {
        while (i)
        {
            buf[--idx] = '0' + (i % 10);
            i /= 10;
        }
    }
    else
    {
        while (i)
        {
            int k = (i & 0xF);
            if (k >= 10)
                buf[--idx] = 'A' + ((i & 0xF) - 10);
            else
                buf[--idx] = '0' + (i & 0xF);
            i >>= 4;
        }
    }

    if (k < 0) buf[0] = '-';

    return buf;
}

int sprintf(char *str, const char *format, ...){
	va_list listpd;
	va_start(listpd, format);
	if(str == NULL || format == NULL) return 0;
	int idx = 0;
	char *s;
	int n = 0;
	while(*format != '\0'){
		if(*format != '%'){
			str[idx++] = *format++;
			n++;
		}
		else{
			format++;
			switch(*format){
				case 's':{
					s = va_arg(listpd, char *);
					int l = strlen(s);
					for(int i = 0; i < l; i++){
						str[idx++] = s[i];
						n++;
					}
				}
			}
			format++;
		}
	}
	return n;
}

int snprintf(char *str, size_t size, const char *format, ...){
	va_list listpd;
	va_start(listpd, format);
	if(str == NULL || format == NULL || size <= 0) return 0;
	char wbuf[100];
	int idx = 0;
	char *s;
	size_t n = 0;
	while(*format != '\0' && n < size){
		if(*format != '%'){
			str[idx++] = *format++;
			n++;
		}
		else{
			format++;
			switch(*format){
				case 's':{
					s = va_arg(listpd, char *);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
				case 'd':{
					int val = va_arg(listpd, int);
					s = itoa(val, wbuf, 10);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
				case 'x':{
					int val = va_arg(listpd, int);
					s = itoa(val, wbuf, 16);
					int l = strlen(s);
					for(int i = 0; i < l && n < size; i++){
						str[idx++] = s[i];
						n++;
					}
					break;
				}
			}
			format++;
		}
	}
	return n;
}