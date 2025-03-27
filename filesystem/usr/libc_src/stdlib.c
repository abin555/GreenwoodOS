#include <stdlib.h>
#include <stdio.h>
#include <sys/memory.h>

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
    memory.region_size = 0x00400000*15;
    memory.region_base = memory_requestRegion(memory.region_size);
    memory.region_head = memory.region_base;
    heap.head = memory.region_head;
}

void clean_stdlib(){
    memory_returnRegion(memory.region_base, memory.region_size);
}

void *malloc(size_t size){
    void *addr = heap.head;
    heap.head += size;
    return addr;
}

void *calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
}

#include <string.h>
void *realloc(void *ptr, size_t size){
	void *new_alloc = malloc(size);
	memcpy(new_alloc, ptr, size);
	return new_alloc;
}


int atoi(const char *arr){
  int val = 0;
  char neg;
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

void exit(int code){
	register unsigned int eax asm("eax");
	register unsigned int ebx asm("ebx");
	ebx = (unsigned int) code;
	eax = 0x1F;
	asm("int 0x80");
	return;
}