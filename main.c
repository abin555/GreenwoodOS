#include "io.h"
#include "multiboot.h"
#include "types.h"
#include "frame_buffer.h"
#include "keyboard.h"
#include "terminal.h"
#include "interrupts.h"
#include "keyboard_test.h"

extern void load_gdt();

u32 vga_width;
u32 vga_height;
u64* screen;

void flyingDot(){
  int x = 50;
  int y = 50;
  int dx = 3;
  int dy = 5;
  int color = 0xFF;
  while(1){
    for(int clear = 0; clear < 0xFF; clear++){
      //fb_setPixel(x,y,0x6f6f6f);
      x+=dx;
      y+=dy;
      fb_setPixel(x,y,0xFFFFFF);
      if(x >= (int) fb_width-10 || x <= 10){
        dx *= -1;
        if(dx < 0){
          dx--;
        }
        if(dx>0){
          dx++;
        }
      }
      if(y >= (int) fb_height-10 || y <= 10){
        dy *= -1;
        if(dy < 0){
          dy--;
        }
        if(dy>0){
          dy++;
        }
      }

      if(dx>50||dx<-50){dx=1;}
      if(dy>50||dy<-50){dy=1;}
      if(x>(int)fb_width || x<0){x=fb_width/2;}
      if(y>(int)fb_height || y<0){y=fb_height/2;}
      for(u32 t = 0; t < 0xFFFF; t++){}
    }
    for(int bx = 0; bx<500; bx++){
      for(int by = 0; by<500; by++){
        fb_setPixel(bx, by, ((color & 0xFF) << 16) | ((color & 0xFF) << 8) | color);
        color--;
        if(color == 0){
          color = 0xFF;
        }
        //for(u32 t = 0; t < 0xFFF; t++){}
      }
    }
  }
}

#define paddleheight 150
#define paddlex 100
#define bounds 50
void pong(){
  unsigned int ascii_pointer = 0;
  int paddley = fb_height/2;
  int ballx = fb_width/2;
  int bally = fb_height/2;
  int ballvx = -10;
  int ballvy = 10;
  while(1){
    fb_clearBackBuffer(0x000000);
    //physicscounter++;
    if(1 /* physicscounter % 0xFF == 0 */){
      //pixelScaled(20,20,4,0xFF00FF);
      ballx+=ballvx;
      bally+=ballvy;
      if(ballx > (int) fb_width - bounds || ballx < paddlex){
        if(ballx < paddlex){
          if(bally > paddley-(paddleheight / 2) && bally < paddley+(paddleheight / 2)){
            ballvx *= -1;
          }
          else{
            ballx = fb_width/2;
            bally = fb_height/2;
            ballvx*=-1;
            ballvx++;
            ballvy++;
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
    if(keyboard_ascii_pointer != ascii_pointer){
      ascii_pointer=keyboard_ascii_pointer;
      //printChar_Scaled(1,1,keyboard_ASCIIBuffer[keyboard_ascii_pointer-1],4);
      if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == 'w'){
        if(paddley > paddleheight){
          paddley-=25;
        }
      }
      else if(keyboard_ASCIIBuffer[keyboard_ascii_pointer-1] == 's'){
        if(paddley < (int) fb_height - paddleheight){
          paddley+=25;
        }
      }
    }
    pixelScaled(ballx ,bally,10,0xFFFFFF);
    //pixelScaled(paddlex, paddley, 10, 0xFFFFFF);
    for(int drawPaddle = 0; drawPaddle < paddleheight; drawPaddle++){
      fb_setPixel(paddlex, paddley-(paddleheight/2)+drawPaddle,0xFFFFFF);
    }
    fb_copyBuffer();
  }
}

int kmain(unsigned long magic, unsigned long magic_addr){
  struct multiboot_tag *tag;
  //unsigned size;
  if(magic != MULTIBOOT2_BOOTLOADER_MAGIC){
    return 0xFF;
  }
  //size = *(unsigned *) magic_addr;
  for(
    tag = (struct multiboot_tag *) (magic_addr + 8);
    tag->type != MULTIBOOT_TAG_TYPE_END;
    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))
  ){
    switch(tag->type){
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            struct multiboot_tag_framebuffer *tagfb
              = (struct multiboot_tag_framebuffer *) tag;
            init_fb(tagfb);
        }
    }
  }


  load_gdt();
  interrupt_install_idt();
  fb_set_color(0xFFFFFF,0);
  pong();
  return 0;
}