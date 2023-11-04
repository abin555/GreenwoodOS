#include "utilities.hpp"

void memfcpy(void* target, void* source, uint32_t size){
    asm volatile ("rep movsb"
    : "=D" (target),
    "=S" (source),
    "=c" (size)
    : "0" (target),
    "1" (source),
    "2" (size)
    : "memory");
}

float atof(char *arr){
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

int atoi(char *arr){
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

float sqrtf(float x){
  union {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1 << 22);

  u.x = u.x + x/u.x;
  u.x = 0.25f*u.x + x/u.x;

  return u.x;
}

void memset(void *mem, char v, int size){
	for(int i = 0; i < size; i++){
		((char *) mem)[i] = v;
	}
}

void memcpy(void *dest, void *src, unsigned int len){
  //print_arg("Copy from 0x%x to ", (uint32_t) src);
  //print_arg("0x%x for ", (uint32_t) dest);
  //print_arg("%d bytes\n", len);
  for(int i = 0; i < len; i++){
    ((char *) dest)[i] = ((char *) src)[i];
  }
}

#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float cos(float x)
{
  x = modd(x, PI2);
  return 1 - ((x * x) / (2)) + ((x * x * x * x) / (24)) - ((x * x * x * x * x * x) / (720)) + ((x * x * x * x * x * x * x * x) / (40320)) - ((x * x * x * x * x * x * x * x * x * x) / (3628800)) + ((x * x * x * x * x * x * x * x * x * x * x * x) / (479001600));
}

float sin(float x){
  return cos(x - (PI / 2));
}

int abs(int x){
  if(x < 0) return x * -1;
  return x;
}

float min3(float a, float b, float c){
  if(a <= b && a <= c) return a;
  if(b <= a && b <= c) return b;
  if(c <= a && c <= b) return c;
}

float max3(float a, float b, float c){
  if(a >= b && a >= c) return a;
  if(b >= a && b >= c) return b;
  if(c >= a && c >= b) return c;
}

int max(int a, int b){if(a >= b) return a; return b;}
int min(int a, int b){if(a <= b) return a; return b;}