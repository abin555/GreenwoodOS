#include "string.h"

int STR_Compare(char *elem1, char *elem2, int start, int end){
  int equal = 0;
  for(int i = start; i < end; i++){
    if(elem1[i] == elem2[i]){
      equal++;
    }
  }
  return equal;
}

void STR_INSERT(char *in_str, char *out_str, int len, int write_index){
    for(int index = 0; index < len; index++){
        out_str[index+write_index] = in_str[index];
    }
}

void decodeData(char *Buffer, int in, int len, int start){
  for(int x = 0; x < len; x++){
    if((in >> x) & 1){
      Buffer[len-x+start] = '1';
    }
    else{
      Buffer[len-x+start] = '0';
    }
  }
}

char quadtoHex(char quad){
  switch(quad){
    case 0x00:
      return '0';
      break;
    case 0x01:
      return '1';
      break;
    case 0x02:
      return '2';
      break;
    case 0x03:
      return '3';
      break;
    case 0x04:
      return '4';
      break;
    case 0x05:
      return '5';
      break;
    case 0x06:
      return '6';
      break;
    case 0x07:
      return '7';
      break;
    case 0x08:
      return '8';
      break;
    case 0x09:
      return '9';
      break;
    case 0x0a:
      return 'A';
      break;
    case 0x0b:
      return 'B';
      break;
    case 0x0c:
      return 'C';
      break;
    case 0x0d:
      return 'D';
      break;
    case 0x0e:
      return 'E';
      break;
    case 0x0f:
      return 'F';
      break;
  }
  return 'x';
}

char hexToQuad(char hex){
  switch(hex){
    case '0':
      return 0x0;
      break;
    case '1':
      return 0x1;
      break;
    case '2':
      return 0x2;
      break;
    case '3':
      return 0x3;
      break;
    case '4':
      return 0x4;
      break;
    case '5':
      return 0x5;
      break;
    case '6':
      return 0x6;
      break;
    case '7':
      return 0x7;
      break;
    case '8':
      return 0x8;
      break;
    case '9':
      return 0x9;
      break;
    case 'A':
    case 'a':
      return 0xa;
      break;
    case 'B':
    case 'b':
      return 0xb;
      break;
    case 'C':
    case 'c':
      return 0xc;
      break;
    case 'D':
    case 'd':
      return 0xd;
      break;
    case 'E':
    case 'e':
      return 0xe;
      break;
    case 'F':
    case 'f':
      return 0xf;
      break;
  }
  return 0;
}

void decodeHex(char *Buffer, unsigned int in, int len, int start){
  for(int i = 0; i < len/4; i++){
    Buffer[start+(len/4 - i)] = quadtoHex((in >> 4*i) & 0x0F);
  }
}

unsigned int encodeHex(char *Buffer, int start, int end){
  unsigned int workInt = 0;
  
  for(int i = 0; i < (end-start); i++){
    workInt = (workInt << 4) | hexToQuad(Buffer[start+i]);
  }
  
  return workInt;
}

void strcpy(char *source, char *destination, unsigned int len){
  for(unsigned int i = 0; i < len; i++){
    source[i] = destination[i];
  }
}