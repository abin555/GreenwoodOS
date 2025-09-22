#include <stdlib.h>
#include <stdio.h>
#include <sys/memory.h>
#include <ctype.h>

struct STDLIB_Memory {
    void *region_base;
    int region_size;
    void *region_head;
};

struct STDLIB_Memory memory;

struct Heap {
    void *head;
};

struct Heap heap;


void init_stdlib(){
    memory.region_size = 0x00400000*4;
    memory.region_base = memory_requestRegion(memory.region_size);
    memory.region_head = memory.region_base;
    heap.head = memory.region_head;
}

void clean_stdlib(){
    memory_returnRegion(memory.region_base, memory.region_size);
}

void *malloc(size_t size){
    void *addr = heap.head;
    heap.head += size+0x10;
    if(heap.head >= memory.region_base + memory.region_size){
      printf("Alloc Error Out of Memory!\n");
      while(1){

      }
    }
    return addr;
}

void *calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
}

void free(void *mem){
  return;
}

#include <string.h>
void *realloc(void *ptr, size_t size){
	void *new_alloc = malloc(size);
	memcpy(new_alloc, ptr, size);
  free(ptr);
	return new_alloc;
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

float atof(const char *arr){
  float val = 0;
  char afterdot = 0;
  float scale = 1;
  char neg = 0;
  while(*arr == ' ') arr++;

  if(*arr == '-'){
    arr++;
    neg = 1;
  }
  while(*arr != 0 && *arr != ' ' && *arr != '\n'){
    if(afterdot){
      scale = scale / 10;
      val = val + (*arr - '0')*scale;
    }
    else{
      if(*arr == '.'){
        afterdot++;
      }
      else{
        val = val * 10.0 + (*arr - '0');
      }
    }
    arr++;
  }
  //print_arg("ATOF: %x\n", val);
  if(neg) return -val;
  return val;
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

  return buf;
}

void exit(int code){
	register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = (unsigned int) code;
	eax = 0x1F;
	asm("int 0x80");
	return;
}

void srand(unsigned int seed){
	register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = seed;
	eax = 0x1D;
	asm("int 0x80");
}

int rand(){
	register unsigned int eax asm("eax");
	eax = 0x1E;
	asm("int 0x80");
	return eax;
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
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
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