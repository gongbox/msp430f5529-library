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

uint32_t time=0;
int16_t overflowTime=0;
__interrupt void TIMER_TB0_B0_IRQ_Handler()
{
   LED_Turn(LED1);               //指示灯闪烁
   if(TIMER_GetITStatus(TIMER_B0,TIMER_OverFlow_IRQn) == TRUE)//如果溢出
   {
     overflowTime++;          //溢出值加1
     TIMER_ClearITPendingBit(TIMER_B0,TIMER_OverFlow_IRQn);     //清除TIMER的某一个中断标志
   }
}
void main()
{
  DisableInterrupts();          //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  OLED_Init();
  
  Set_Vector_Handler(VECTOR_TIMER0_B1,TIMER_TB0_B0_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断 
  TIMER_Timer_Init (TIMER_B0);                                      //计时功能初始化
  TIMER_ITConfig (TIMER_B0,TIMER_OverFlow_IRQn,TRUE);              //使能TIMER的某一个中断

  EnableInterrupts();
  while(1)
  {  
    TIMER_Timer_Start   (TIMER_B0);                               //计时功能启动
    TIMER_Delay_Ms      (TIMER_A0,500);  //延时500MS
    TIMER_Timer_Stop    (TIMER_B0);                               //计时功能停止，返回计数值
    time = TIMER_Timer_GetTime_Us(TIMER_B0, overflowTime);          //读取计时时间(单位：US) 
    overflowTime = 0;
    OLED_PrintfAt(FONT_ASCII_6X8,0,0,"time:\n%ld US",time);         //在指定位置打印
  }
}