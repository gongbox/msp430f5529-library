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

void main()
{
  DisableInterrupts();          //禁止总中断
  
  LED_Init(LED1);              //LED灯初始化
  
  //定时器A0，频率1K,占空比为0,初始化4个通道，分别是：TIMER_CH1(P1.2), TIMER_CH2(P1.3), TIMER_CH3(P1.4), TIMER_CH4(P1.5)
  //TIMER_PWM_MultiChannel_Init(TIMER_A0,1000, 4, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);  
  TIMER_PWM_Init(TIMER_A0,1000, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);   
  
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH1,0);       //设置某一个定时器某一个通道的占空比
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH2,200);     //设置某一个定时器某一个通道的占空比
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH3,700);     //设置某一个定时器某一个通道的占空比
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH4,1000);    //设置某一个定时器某一个通道的占空比
  
  //定时器B0，频率1HZ，占空比为0,初始化一个通道：TIMER_CH5(P3.5)
  TIMER_PWM_MultiChannel_Init(TIMER_B0, 1, 1, TIMER_CH5);  
  //TIMER_PWM_Init(TIMER_B0, 1, TIMER_CH5); 
  
  TIMER_PWM_SetChannelDuty  (TIMER_B0,TIMER_CH5,500);     //设置某一个定时器某一个通道的占空比
  GPIO_PinRemapConfig(P4,7,GPIO_Remap_TB0CCR5A);        //GPIO端口重映射
  
  while(1)
  {  
    DELAY_MS(500);
    LED_Turn(LED1);              //指示灯闪烁
  }
}

