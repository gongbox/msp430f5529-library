#ifndef REMAP_H_
#define REMAP_H_

#include "common.h"
#include "msp430f5529_gpio.h"

typedef enum
{
  GPIO_Remap_NONE       =0u,//普通IO口
  GPIO_Remap_CBOUT0     =1u,//CB比较结果输出
  GPIO_Remap_TB0CLK     =1u,//
  GPIO_Remap_ADC12CLK   =2u,//
  GPIO_Remap_DMAE0      =2u,//
  GPIO_Remap_SVMOUT     =3u,//
  GPIO_Remap_TB0OUTH    =3u,//
  GPIO_Remap_TB0CCR0A   =4u,//定时器TB0CCR0，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR1A   =5u,//定时器TB0CCR1，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR2A   =6u,//定时器TB0CCR2，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR3A   =7u,//定时器TB0CCR3，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR4A   =8u,//定时器TB0CCR4，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR5A   =9u,//定时器TB0CCR5，PWM功能时，需要设为输出方向
  GPIO_Remap_TB0CCR6A   =10u,//定时器TB0CCR6，PWM功能时，需要设为输出方向
  GPIO_Remap_UCA1RXD    =11u,//UCXx通信接口
  GPIO_Remap_UCA1SOMI   =11u,//UCXx通信接口
  GPIO_Remap_UCA1TXD    =12u,//UCXx通信接口
  GPIO_Remap_UCA1SIMO   =12u,//UCXx通信接口
  GPIO_Remap_UCA1CLK    =13u,//UCXx通信接口
  GPIO_Remap_UCB1STE    =13u,//UCXx通信接口
  GPIO_Remap_UCB1SOMI   =14u,//UCXx通信接口
  GPIO_Remap_UCB1SCL    =14u,//UCXx通信接口
  GPIO_Remap_UCB1SIMO   =15u,//UCXx通信接口
  GPIO_Remap_UCB1SDA    =15u,//UCXx通信接口
  GPIO_Remap_UCB1CLK    =16u,//UCXx通信接口
  GPIO_Remap_UCA1STE    =16u,//UCXx通信接口
  GPIO_Remap_CBOUT1     =17u,//CB比较结果输出
  GPIO_Remap_MCLK       =18u,//主时钟输出,需要设为输出方向
  GPIO_Remap_ANALOG     =31u,//
/************************为使用方便，下面对部分常用功能进行定义**************************************************/  
  GPIO_Remap_UART1_RX  = GPIO_Remap_UCA1RXD,//UCXx通信接口
  GPIO_Remap_UART1_TX  = GPIO_Remap_UCA1TXD,//UCXx通信接口
  
  GPIO_Remap_SPI1_SOMI = GPIO_Remap_UCA1SOMI,//UCXx通信接口
  GPIO_Remap_SPI1_SIMO = GPIO_Remap_UCA1SIMO,//UCXx通信接口
  GPIO_Remap_SPI1_CLK  = GPIO_Remap_UCA1CLK,//UCXx通信接口
  GPIO_Remap_SPI1_STE  = GPIO_Remap_UCA1STE,//UCXx通信接口  
  
  GPIO_Remap_SPI3_SOMI = GPIO_Remap_UCB1SOMI,//UCXx通信接口
  GPIO_Remap_SPI3_SIMO = GPIO_Remap_UCB1SIMO,//UCXx通信接口
  GPIO_Remap_SPI3_CLK  = GPIO_Remap_UCB1CLK,//UCXx通信接口
  GPIO_Remap_SPI3_STE  = GPIO_Remap_UCB1STE,//UCXx通信接口 
   
  GPIO_Remap_I2C1_SCL  = GPIO_Remap_UCB1SCL,//UCXx通信接口
  GPIO_Remap_I2C1_SDA  = GPIO_Remap_UCB1SDA,//UCXx通信接口
  
}GPIO_Remap;

extern void GPIO_PinRemapConfig(PORTn, uint8_t pin, GPIO_Remap);                //GPIO端口重映射

#endif