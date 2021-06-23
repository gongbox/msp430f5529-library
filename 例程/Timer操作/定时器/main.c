/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 ƽ̨������

 * @author     CTGU-GB
 * @version    v2.1
 * @date       2016-01-30
 */
#include "include.h"

__interrupt void TIMER_TB0_A0_IRQ_Handler()
{
   LED_Turn(LED1);               //ָʾ����˸
}
__interrupt void TIMER_TA0_A0_IRQ_Handler()
{
   LED_Turn(LED2);               //ָʾ����˸
}
void main()
{
  DisableInterrupts();          //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  Set_Vector_Handler(VECTOR_TIMER0_B0,TIMER_TB0_A0_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж�
  TIMER_Interval_Ms(TIMER_B0,500);                    //��ʼ��һ��500ms�Ķ�ʱ�ж�
  TIMER_ITConfig (TIMER_B0,TIMER_CCR0_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�

  Set_Vector_Handler(VECTOR_TIMER0_A0,TIMER_TA0_A0_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж�
  TIMER_Interval_Us(TIMER_A0,1000000);                    //��ʼ��һ��1000ms�Ķ�ʱ�ж�,��ҪдΪ1000*1000���������ʾ������Χ
  TIMER_ITConfig (TIMER_A0,TIMER_CCR0_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�

  EnableInterrupts();
  while(1)
  {  
    DELAY_MS(500);                 //��ʱ500MS�������õ������ʱ����DELAY��h�������Ϊ�ö�ʱ����ʱ
  }
}
