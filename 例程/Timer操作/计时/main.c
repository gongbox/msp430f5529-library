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

uint32_t time=0;
int16_t overflowTime=0;
__interrupt void TIMER_TB0_B0_IRQ_Handler()
{
   LED_Turn(LED1);               //ָʾ����˸
   if(TIMER_GetITStatus(TIMER_B0,TIMER_OverFlow_IRQn) == TRUE)//������
   {
     overflowTime++;          //���ֵ��1
     TIMER_ClearITPendingBit(TIMER_B0,TIMER_OverFlow_IRQn);     //���TIMER��ĳһ���жϱ�־
   }
}
void main()
{
  DisableInterrupts();          //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  OLED_Init();
  
  Set_Vector_Handler(VECTOR_TIMER0_B1,TIMER_TB0_B0_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж� 
  TIMER_Timer_Init (TIMER_B0);                                      //��ʱ���ܳ�ʼ��
  TIMER_ITConfig (TIMER_B0,TIMER_OverFlow_IRQn,TRUE);              //ʹ��TIMER��ĳһ���ж�

  EnableInterrupts();
  while(1)
  {  
    TIMER_Timer_Start   (TIMER_B0);                               //��ʱ��������
    TIMER_Delay_Ms      (TIMER_A0,500);  //��ʱ500MS
    TIMER_Timer_Stop    (TIMER_B0);                               //��ʱ����ֹͣ�����ؼ���ֵ
    time = TIMER_Timer_GetTime_Us(TIMER_B0, overflowTime);          //��ȡ��ʱʱ��(��λ��US) 
    overflowTime = 0;
    OLED_PrintfAt(FONT_ASCII_6X8,0,0,"time:\n%ld US",time);         //��ָ��λ�ô�ӡ
  }
}