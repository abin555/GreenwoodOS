#include "string.h"

void STR_INSERT(char *in_str, char *out_str, int len, int write_index){
    for(int index = 0; index < len; index++){
        out_str[index+write_index] = in_str[index];
    }
}

void decodeData(char *Buffer, int in, int len, int start){
  for(int x = 1; x < len; x++){
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

void decodeHex(char *Buffer, int in, int len, int start){
  for(int i = 0; i < len/4; i++){
    Buffer[start+(len/4 - i)] = quadtoHex((in >> 4*i) & 0x0F);
  }
}