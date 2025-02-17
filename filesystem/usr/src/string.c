#include <string.h>

void *memchr(const void *s, int c, size_t n){
	const unsigned char uc = c;
	const unsigned char *su = (const unsigned char *) s;

	for(; 0 < n; ++su, --n){
		if(*su == uc) return ((void *) su);
	}
	return(NULL);
}

int memcmp(const void *s1, const void *s2, size_t n){
	const unsigned char *su1 = (const unsigned char *) s1;
	const unsigned char *su2 = (const unsigned char *) s2;

	for(; 0 < n; ++su1, ++su2, --n){
		if(*su1 != *su2){
			return(*su1 < *su2 ? -1 : +1);
		}
	}
	return 0;
}

void *memcpy(void *s1, const void *s2, size_t n){
	char *su1 = (char *)s1;
	const char *su2 = (const char *)s2;

	for(; 0 < n; ++su1, ++su2, --n){
		*su1 = *su2;
	}
	return s1;
}

void *memmove(void *s1, const void *s2, size_t n){
	char *sc1 = (char *) s1;
	const char *sc2 = (const char *) s2;

	if(sc2 < sc1 && sc1 < sc2 + n){
		for(sc1 += n, sc2 +=n; 0 < n; --n){
			*--sc1 = *--sc2;
		}
	}
	else{
		for(; 0 < n; --n){
			*sc1++ = *sc2++;
		}
	}
	return s1;
}

void *memset(void *s, int c, size_t n){
	const unsigned char uc = c;
	unsigned char *su = (unsigned char *)s;

	for (; 0 < n; ++su, --n){
		*su = uc;
	}
	return s;
}

char *strcat(char *s1, const char *s2){
	char *s;

	for (s = s1; *s != '\0'; ++s)
		;			/* find end of s1[] */
	for (; (*s = *s2) != '\0'; ++s, ++s2)
		;			/* copy s2[] to end */
	return s1;
}

char *strchr(const char *s, int c){
	const char ch = c;

	for (; *s != ch; ++s)
		if (*s == '\0')
			return (NULL);
	return ((char *)s);
}


int strcmp(const char *s1, const char *s2){
	for (; *s1 == *s2; ++s1, ++s2)
		if (*s1 == '\0')
			return (0);
	return (*(unsigned char *)s1 < *(unsigned char *)s2
		? -1 : +1);
}

int strcoll(const char *s1, const char *s2){
	return strcmp(s1, s2);
}

char *strcpy(char *s1, const char *s2){
	char *s = s1;

	for (s = s1; (*s++ = *s2++) != '\0'; )
		;

	return s1;
}