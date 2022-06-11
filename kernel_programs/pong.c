#include "pong.h"

#define paddleheight 100
#define paddlex 50
#define bounds 50
void pong(){
  unsigned int ascii_pointer = 0;
  unsigned int key_pointer = 0;
  int paddley = fb_height/2;
  int paddle2y = fb_height/2;
  int paddle2x = fb_width - paddlex;

  int ballx = fb_width/2;
  int bally = fb_height/2;
  int ballvx = -1;
  int ballvy = 1;
  u16 physicscounter = 0;
  int physdelay = 0xFFF;

  int paddlev = 0;
  int paddle2v = 0;

  uint8_t score[2] = {0, 0};

  fb_clear(0);
  while(1){
    //fb_clearBackBuffer(0x000000);
    physicscounter++;
    //fb_clear(0);

    

    if(physicscounter % physdelay == 0){
      pixelScaled(ballx,bally,10,0);
      ballx+=ballvx;
      bally+=ballvy;
      if(ballx > (int) fb_width - bounds || ballx < paddlex){
        if(ballx < paddlex){
          if(bally > paddley-(paddleheight / 2) && bally < paddley+(paddleheight / 2)){
            ballvx *= -1;
            
            score[1]++;
            //decodeData(STR_edit, score, 8, 0);
          }
          else{
            ballx = fb_width/2;
            bally = fb_height/2;
            ballvx*=-1;
            //decodeData(STR_edit, score, 8, 0);
          }
          //printChar_Scaled(fb_width/8-1,5,STR_edit[0],4);
          //printChar_Scaled(fb_width/8+1,5,STR_edit[1],4);
        }
        else if(ballx > paddle2x){
          if(bally > paddle2y-(paddleheight / 2) && bally < paddle2y+(paddleheight / 2)){
            ballvx *= -1;
            
            score[0]++;
            //decodeData(STR_edit, score, 8, 0);
          }
          else{
            ballx = fb_width/2;
            bally = fb_height/2;
            ballvx*=-1;
            //decodeData(STR_edit, score, 8, 0);
          }
        }
        else{
          ballvx *= -1;
        }
      }
      if(bally > (int) fb_height - bounds || bally < bounds){
        ballvy *= -1;
      }
    }

    if(physicscounter % (physdelay/2) == 0){
      if(paddlev < 0){
        if(paddley > paddleheight){
          paddley+=paddlev;
        }
      }
      else if(paddlev > 0){
        if(paddley < (int) fb_height - paddleheight){
          paddley+=paddlev;
        }
      }
      if(paddlev){
        for(int drawPaddle = 0; drawPaddle < (int) fb_height; drawPaddle++){
          fb_setPixel(paddlex, drawPaddle,0);
        }
      }

      if(paddle2v < 0){
        if(paddle2y > paddleheight){
          paddle2y+=paddle2v;
        }
      }
      else if(paddle2v > 0){
        if(paddle2y < (int) fb_height - paddleheight){
          paddle2y+=paddle2v;
        }
      }
      if(paddle2v){
        for(int drawPaddle = 0; drawPaddle < (int) fb_height; drawPaddle++){
          fb_setPixel(paddle2x, drawPaddle,0);
        }
      }
    }

    if(keyboard_KEYBUFFER_POINTER != key_pointer){
      key_pointer = keyboard_KEYBUFFER_POINTER;

      if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x11){
        paddlev = -1;
      }
      else if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x91 || keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x9F){
        paddlev = 0;
      }
      else if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x1F){
        paddlev = 1;
      }

      if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x48){
        paddle2v = -1;
      }
      else if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0xC8 || keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0xD0){
        paddle2v = 0;
      }
      else if(keyboard_KEYBUFFER[keyboard_KEYBUFFER_POINTER-1] == 0x50){
        paddle2v = 1;
      }
    }

    if(keyboard_ascii_pointer != ascii_pointer){
      ascii_pointer=keyboard_ascii_pointer;
      if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == '['){
        if(physdelay > 20){
          physdelay = physdelay >> 1;
        }
      }
      else if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == ']'){
        physdelay = (physdelay << 1) | 1;
      }
      else if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == '/'){
        printk("Score ptr: %32h", &score);
        return;
      }
    }
    pixelScaled(ballx ,bally,10,0xFFFFFF);
    //pixelScaled(paddlex, paddley, 10, 0xFFFFFF);
    for(int drawPaddle = 0; drawPaddle < paddleheight; drawPaddle++){
      fb_setPixel(paddlex, paddley-(paddleheight/2)+drawPaddle,0xFFFFFF);
      fb_setPixel(paddle2x, paddle2y-(paddleheight/2)+drawPaddle, 0xFFFFFF);
    }
    //fb_copyBuffer();
  }
}