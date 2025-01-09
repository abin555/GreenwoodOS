#include "gwos_lib.h"

void *heap_start;
uint32_t heap_idx;

void init_heap(int heap_size){
	heap_start = kmalloc(heap_size);
}

void *malloc(int size){
	void *addr = heap_start+heap_idx;
	heap_idx += size+10;
	return addr;
}

void free(void *mem){
	return;
}

void *os_malloc(int size){
	return malloc(size);
}

void os_free(void *mem){
	free(mem);
}

void print_str(char *str, char *arg){
	print_arg(str, (uint32_t) arg);
}

void print_int(char *str, int arg){
	print_arg(str, arg);
}

void pchar(char c){
	print_arg("%c", c);
}

int toupper(int c){
    if((c >= 65 && c <= 90) || c >= 97 && c <= 122)
        return c & ~(0b00100000);
    return c;
}

int strlen(const char *s){
	int i = 0;
	while(*s != 0){
		s++;
		i++;
	}
	return i+1;
}

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

#ifndef LONG_MAX
#define	LONG_MAX	((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF */
#endif

#ifndef LONG_MIN
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#endif

int ISDIGIT(char c){
	if(c <= '9' && c >= '0') return 1;
	return 0;
}
int ISALPHA(char c){
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return 1;
	return 0;
}
int ISUPPER(char c){
	if(c >= 'A' && c <= 'Z') return 1;
	return 0;
}

int strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (c == ' ');
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (ISDIGIT(c))
			c -= '0';
		else if (ISALPHA(c))
			c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

int strspn(const char *s, const char *accept){
	int i, j;
	for (i = 0; s[i] != '\0'; i++) {
        for (j = 0; accept[j] != s[i]; j++) {
            if (accept[j] == '\0')
                return i; // char not found, return index so far
        }
    }
    return i;  // complete string matches, return length
}

int strcspn(const char *s, const char *reject){
	unsigned int len = 0;
	if((s == 0) || reject == NULL){
		return len;
	}

	while(*s){
		if(strchr(reject, *s)){
			return len;
		}
		else{
			s++;
			len++;
		}
	}
	return len;
}

char *strchr(const char *s, int ch){
	char c;

	c = ch;
	for (;; ++s) {
		if (*s == c)
			return ((char *)s);
		if (*s == '\0')
			return (NULL);
	}
}

char *strdup(const char *s){
	int size = strlen(s);
	char *dup = malloc(size);
	for(int i = 0; i < size; i++){
		dup[i] = s[i];
	}
	return dup;
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}