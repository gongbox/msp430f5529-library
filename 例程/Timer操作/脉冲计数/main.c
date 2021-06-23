/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 ƽ̨������

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
   LED_Turn(LED1);               //ָʾ����˸
}
__interrupt void TIMER_TA0_A0_IRQ_Handler()
{
   LED_Turn(LED2);               //ָʾ����˸   
   //��ȡ������
   PulseValue = TIMER_Pluse_GetValue    (TIMER_A1,overflowTime);
   overflowTime=0;              //����������
   TIMER_Pluse_Clear   (TIMER_A1);
}
void main()
{
  DisableInterrupts();          //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  OLED_Init();
  
  //��ʱ��B0��Ƶ��10K
  //TIMER_PWM_MultiChannel_Init      (TIMER_B0,10000, 1 ,TIMER_CH5);   //��ʱ��TB0��ͨ��5��P3.5
  TIMER_PWM_Init(TIMER_B0,10000,TIMER_CH5);   //��ʱ��TB0��ͨ��5��P3.5
  
  Set_Vector_Handler(VECTOR_TIMER0_A0,TIMER_TA0_A0_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж�
  TIMER_Interval_Ms(TIMER_A0,1000);                    //��ʼ��һ��1000ms�Ķ�ʱ�ж�
  TIMER_ITConfig (TIMER_A0,TIMER_CCR0_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�

  Set_Vector_Handler(VECTOR_TIMER1_A1,TIMER_TA1_A1_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж�
  TIMER_Pluse_Init    (TIMER_A1);  //��ʱ��A1�������������P1.6��Ĭ���Կ�������ж�
  TIMER_ITConfig (TIMER_A1,TIMER_OverFlow_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�

  EnableInterrupts();
  while(1)
  {  
    OLED_DispDecAt(FONT_ASCII_6X8,0,0,PulseValue,8);    //��ָ��λ����ʾһ���������� 
  }
}
