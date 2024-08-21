#include "utils.h"
#include "allocator.h"

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