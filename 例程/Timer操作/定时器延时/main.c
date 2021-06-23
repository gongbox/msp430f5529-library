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
 * @date       2016-01-30
 */
#include "include.h"

void main()
{
  DisableInterrupts();          //禁止总中断
  LED_Init(LED_ALL);              //LED灯初始化
  while(1)
  {  
    TIMER_Delay_Ms      (TIMER_A0,500);  //延时500MS
    LED_Turn(LED_ALL);               //指示灯闪烁
  }
}
