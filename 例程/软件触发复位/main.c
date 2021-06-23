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
  DisableInterrupts();             //禁止总中断
  
  LED_Init(LED_ALL);               //LED灯初始化
  
  EnableInterrupts();              //是能中断
  while(1)
  {
     SoftWare_RESET();             //软件触发一次复位
     LED_On  (LED0);               //指示灯亮，由于软件触发了复位，所以这里LED不会点亮
  }
}