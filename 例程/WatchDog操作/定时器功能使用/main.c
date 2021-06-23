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
__interrupt void WatchDog_IRQ_Handler()
{
  LED_Turn  (LED1);               //指示灯亮
}
void main()
{
  DisableInterrupts();            //禁止总中断
  LED_Init(LED_ALL);              //LED灯初始化
  
  Set_Vector_Handler(VECTOR_WDT,WatchDog_IRQ_Handler);
  WatchDog_Timer_Init(500);  //500MS中断
  WatchDog_ITConfig(ENABLE);                                   //设置看门狗中断是否使能
  
  EnableInterrupts();              //是能中断
  while(1)
  {
     DELAY_MS(500);
  }
}