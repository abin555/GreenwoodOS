#include "utils.h"
#include "allocator.h"
#include <stdarg.h>

char quadToHex(uint8_t quad){
	switch(quad){
		case 0x00:
		return '0';
		break;
		case 0x01:
		case 0x10:
		return '1';
		break;
		case 0x02:
		case 0x20:
		return '2';
		break;
		case 0x03:
		case 0x30:
		return '3';
		break;
		case 0x04:
		case 0x40:
		return '4';
		break;
		case 0x05:
		case 0x50:
		return '5';
		break;
		case 0x06:
		case 0x60:
		return '6';
		break;
		case 0x07:
		case 0x70:
		return '7';
		break;
		case 0x08:
		case 0x80:
		return '8';
		break;
		case 0x09:
		case 0x90:
		return '9';
		break;
		case 0x0a:
		case 0xa0:
		return 'A';
		break;
		case 0x0b:
		case 0xb0:
		return 'B';
		break;
		case 0x0c:
		case 0xc0:
		return 'C';
		break;
		case 0x0d:
		case 0xd0:
		return 'D';
		break;
		case 0x0e:
		case 0xe0:
		return 'E';
		break;
		case 0x0f:
		case 0xf0:
		return 'F';
		break;
	}
	return 'x';
}

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

char *utoa(unsigned int k, char *buf, int radix){
	unsigned int i = k;
	if (i == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}
  
	int idx = 0;
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
					unsigned int val = va_arg(listpd, unsigned int);
					s = utoa(val, wbuf, 16);
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

int atoi(const char *arr){
    int val = 0;
    char neg = 0;
    if(*arr == '-'){
      arr++;
      neg = 1;
    }
    while(*arr != 0 && *arr != ' ' && *arr != '\n'){
      val = val * 10 + (*arr - '0');
      arr++;
    }
    //print_arg("ATOI: %d\n", val);
    if(neg) return -val;
    return val;
}
