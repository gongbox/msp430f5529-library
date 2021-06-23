#ifndef REMAP_H_
#define REMAP_H_

#include "common.h"
#include "msp430f5529_gpio.h"

typedef enum
{
  GPIO_Remap_NONE       =0u,//��ͨIO��
  GPIO_Remap_CBOUT0     =1u,//CB�ȽϽ�����
  GPIO_Remap_TB0CLK     =1u,//
  GPIO_Remap_ADC12CLK   =2u,//
  GPIO_Remap_DMAE0      =2u,//
  GPIO_Remap_SVMOUT     =3u,//
  GPIO_Remap_TB0OUTH    =3u,//
  GPIO_Remap_TB0CCR0A   =4u,//��ʱ��TB0CCR0��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR1A   =5u,//��ʱ��TB0CCR1��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR2A   =6u,//��ʱ��TB0CCR2��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR3A   =7u,//��ʱ��TB0CCR3��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR4A   =8u,//��ʱ��TB0CCR4��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR5A   =9u,//��ʱ��TB0CCR5��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_TB0CCR6A   =10u,//��ʱ��TB0CCR6��PWM����ʱ����Ҫ��Ϊ�������
  GPIO_Remap_UCA1RXD    =11u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCA1SOMI   =11u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCA1TXD    =12u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCA1SIMO   =12u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCA1CLK    =13u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1STE    =13u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1SOMI   =14u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1SCL    =14u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1SIMO   =15u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1SDA    =15u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCB1CLK    =16u,//UCXxͨ�Žӿ�
  GPIO_Remap_UCA1STE    =16u,//UCXxͨ�Žӿ�
  GPIO_Remap_CBOUT1     =17u,//CB�ȽϽ�����
  GPIO_Remap_MCLK       =18u,//��ʱ�����,��Ҫ��Ϊ�������
  GPIO_Remap_ANALOG     =31u,//
/************************Ϊʹ�÷��㣬����Բ��ֳ��ù��ܽ��ж���**************************************************/  
  GPIO_Remap_UART1_RX  = GPIO_Remap_UCA1RXD,//UCXxͨ�Žӿ�
  GPIO_Remap_UART1_TX  = GPIO_Remap_UCA1TXD,//UCXxͨ�Žӿ�
  
  GPIO_Remap_SPI1_SOMI = GPIO_Remap_UCA1SOMI,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI1_SIMO = GPIO_Remap_UCA1SIMO,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI1_CLK  = GPIO_Remap_UCA1CLK,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI1_STE  = GPIO_Remap_UCA1STE,//UCXxͨ�Žӿ�  
  
  GPIO_Remap_SPI3_SOMI = GPIO_Remap_UCB1SOMI,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI3_SIMO = GPIO_Remap_UCB1SIMO,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI3_CLK  = GPIO_Remap_UCB1CLK,//UCXxͨ�Žӿ�
  GPIO_Remap_SPI3_STE  = GPIO_Remap_UCB1STE,//UCXxͨ�Žӿ� 
   
  GPIO_Remap_I2C1_SCL  = GPIO_Remap_UCB1SCL,//UCXxͨ�Žӿ�
  GPIO_Remap_I2C1_SDA  = GPIO_Remap_UCB1SDA,//UCXxͨ�Žӿ�
  
}GPIO_Remap;

extern void GPIO_PinRemapConfig(PORTn, uint8_t pin, GPIO_Remap);                //GPIO�˿���ӳ��

#endif