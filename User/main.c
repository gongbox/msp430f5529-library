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

uint16_t data=1000;
float fdata=1.0; 
__interrupt void TIMER1_A0_IRQ_Handler()
{
   //GPIO_TurnBit  (PB, 15);//PIN_OUT(P4,7) = !PIN_OUT(P4,7);
   GPIO_TurnBit  (P4, 7);
   data++;  
   fdata+=0.1;
}
void main()
{
  DisableInterrupts();            //禁止总中断
  SMCLK_OUT();
  LED_Init(LED_ALL);              //LED灯初始化
  OLED_Init();                       //OLED初始化
  //LCD_INIT();
    
  Set_Vector_Handler(VECTOR_TIMER_A1_CCR0, TIMER1_A0_IRQ_Handler);   //设置定时器TA1的中断服务程序为：TIMER1_A0_IRQ_Handler
  TIMER_Interval_Ms(TIMER_A1, 500);    //设置TA1为定时中断，中断时间为500Ms
  TIMER_ITConfig (TIMER_A1, TIMER_CCR0_IRQn, ENABLE);            //使能TIMER的某一个中断
  
  //OLED显示字符，字符串，整数，小数，一个函数就够了
  OLED_PrintfAt(FONT_ASCII_6X8,0,0,"A");//显示一个字符
  OLED_PrintfAt(FONT_ASCII_6X8,0,6,"BCDE!");//显示一个字符串
  OLED_PrintfAt(FONT_ASCII_6X8,1,0,"%d",-100);//显示一个整数
  OLED_PrintfAt(FONT_ASCII_6X8,2,0,"%.5f",-100.008310);//显示一个浮点数
  
  EnableInterrupts();
  while(1)
  {  
    //OLED_PrintfAt(FONT_ASCII_6X8,3,0,"data=%d\nfdata=%f",data,fdata);//显示更新 
     DELAY_MS(500);
     GPIO_TurnBit  (P1, 0);
     //GPIO_TurnBit  (PA, 0);
     //PIN_OUT(P1,0) = !PIN_OUT(P1,0);
  }
}