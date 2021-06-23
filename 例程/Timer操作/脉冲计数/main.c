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

uint32_t PulseValue=0;
int16_t overflowTime=0;
__interrupt void TIMER_TA1_A1_IRQ_Handler()
{
   overflowTime++;
   LED_Turn(LED1);               //指示灯闪烁
}
__interrupt void TIMER_TA0_A0_IRQ_Handler()
{
   LED_Turn(LED2);               //指示灯闪烁   
   //读取脉冲数
   PulseValue = TIMER_Pluse_GetValue    (TIMER_A1,overflowTime);
   overflowTime=0;              //清除溢出次数
   TIMER_Pluse_Clear   (TIMER_A1);
}
void main()
{
  DisableInterrupts();          //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  OLED_Init();
  
  //定时器B0，频率10K
  //TIMER_PWM_MultiChannel_Init      (TIMER_B0,10000, 1 ,TIMER_CH5);   //定时器TB0，通道5，P3.5
  TIMER_PWM_Init(TIMER_B0,10000,TIMER_CH5);   //定时器TB0，通道5，P3.5
  
  Set_Vector_Handler(VECTOR_TIMER0_A0,TIMER_TA0_A0_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断
  TIMER_Interval_Ms(TIMER_A0,1000);                    //初始化一个1000ms的定时中断
  TIMER_ITConfig (TIMER_A0,TIMER_CCR0_IRQn,ENABLE);              //使能TIMER的某一个中断

  Set_Vector_Handler(VECTOR_TIMER1_A1,TIMER_TA1_A1_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断
  TIMER_Pluse_Init    (TIMER_A1);  //定时器A1用作脉冲计数，P1.6，默认以开启溢出中断
  TIMER_ITConfig (TIMER_A1,TIMER_OverFlow_IRQn,ENABLE);              //使能TIMER的某一个中断

  EnableInterrupts();
  while(1)
  {  
    OLED_DispDecAt(FONT_ASCII_6X8,0,0,PulseValue,8);    //在指定位置显示一个整形数字 
  }
}
