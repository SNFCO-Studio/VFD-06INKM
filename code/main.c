#include "ioCC2530.h"
#include <stdio.h>
#include <stdint.h>

#define RESET P0_0
#define CS P0_1
#define CLK P0_2
#define DIN P0_3

static void initLed(void);
static void delayMs(uint16_t nMs);
void send(char data);
void initDis(void);
void sendBuff(char x,char *str);
void show(void);

void send(char data){ // 发送SPI数据
  for(int i=0;i<8;i++){
    CLK = 0;
    delayMs(1);
    if((data&0x01) == 0x01){
      DIN = 1;
    }else{
      DIN = 0;
    }
    data = data >> 1;
    delayMs(2);
    CLK = 1;
    delayMs(2);
  }
}

static void delayMs(uint16_t nMs) // 延时
{
    uint16_t i,j;
  
    for (i = 0; i < nMs; i++)
        for (j = 0; j < 535; j++);
}


void initDis(){ //初始化屏幕
  RESET = 0;
  delayMs(3);
  RESET = 1;
  
  CS = 0;
  send(0xe4);
  delayMs(2);
  send(0xff);
  CS = 1;
  delayMs(2);
  
  
  CS = 0;
  send(0xe0);
  delayMs(2);
  send(0x07);
  CS = 1;
  
  CS = 0;
  send(0xe4);
  delayMs(2);
  send(0xff);
  CS = 1;
  delayMs(2);
}

void sendBuff(char x,char *str){ // 显示字符
  CS = 0;
  delayMs(2);
  
  send(0x20 + x);
  while(*str){
    send(*str);
    str++;
  }
  CS = 1;
}

void show(){ // 屏幕打印
  CS = 0;
  delayMs(2);
  send(0xe8);
  CS = 1;
}

void main()
{
    initLed();
    initDis();
    
    sendBuff(0,"SFNCO");
    
    show();
    while(1) {
      // send(0xAC);
      delayMs(30);
    } /* while */
}

static void initLed() //初始化GPIO
{
    P0SEL &= ~(0<<1);
    P0DIR |=  (0<<1);
    
    P0SEL &= ~(1<<1);
    P0DIR |=  (1<<1);
    
    P0SEL &= ~(1<<2);
    P0DIR |=  (1<<2);
    
    P0SEL &= ~(1<<3);
    P0DIR |=  (1<<3);
}
