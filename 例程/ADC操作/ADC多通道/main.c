/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 平台主程序

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

float ad1,ad3;
void main()
{
  DisableInterrupts();          //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  OLED_Init();
  
  ADC_Init(ADC_CH1+ADC_CH3,ADC_VREF_3_3V,ADC_10bit);       //初始化通道,P6.1,P6.3
  EnableInterrupts();
  while(1)
  {  
    ADC_StartConv();                                              //启动一次转换
    ad1 = ADC_ReadChanelValue (ADC_CH1)*3.3/1023;
    ad3 = ADC_ReadChanelValue (ADC_CH3)*3.3/1023;  
    OLED_PrintfAt(FONT_ASCII_6X8,0,0,"ADValue:\n%f V\n%f V",ad1,ad3);                     //显示一个整数
  }
}




