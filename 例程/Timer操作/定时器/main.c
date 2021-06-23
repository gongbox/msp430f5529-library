/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 平台主程序

 * @author     CTGU-GB
 * @version    v2.1
 * @date       2016-01-30
 */
#include "include.h"

__interrupt void TIMER_TB0_A0_IRQ_Handler()
{
   LED_Turn(LED1);               //指示灯闪烁
}
__interrupt void TIMER_TA0_A0_IRQ_Handler()
{
   LED_Turn(LED2);               //指示灯闪烁
}
void main()
{
  DisableInterrupts();          //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  Set_Vector_Handler(VECTOR_TIMER0_B0,TIMER_TB0_A0_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断
  TIMER_Interval_Ms(TIMER_B0,500);                    //初始化一个500ms的定时中断
  TIMER_ITConfig (TIMER_B0,TIMER_CCR0_IRQn,ENABLE);              //使能TIMER的某一个中断

  Set_Vector_Handler(VECTOR_TIMER0_A0,TIMER_TA0_A0_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断
  TIMER_Interval_Us(TIMER_A0,1000000);                    //初始化一个1000ms的定时中断,不要写为1000*1000，否则会提示超出范围
  TIMER_ITConfig (TIMER_A0,TIMER_CCR0_IRQn,ENABLE);              //使能TIMER的某一个中断

  EnableInterrupts();
  while(1)
  {  
    DELAY_MS(500);                 //延时500MS，这里用的软件延时，在DELAY。h里可以设为用定时器延时
  }
}
