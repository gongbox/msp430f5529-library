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
 * @date       2016-11-26
 */
#include "include.h"
__interrupt void WatchDog_IRQ_Handler()
{
  LED_Turn  (LED1);               //ָʾ����
}
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  
  Set_Vector_Handler(VECTOR_WDT,WatchDog_IRQ_Handler);
  WatchDog_Timer_Init(500);  //500MS�ж�
  WatchDog_ITConfig(ENABLE);                                   //���ÿ��Ź��ж��Ƿ�ʹ��
  
  EnableInterrupts();              //�����ж�
  while(1)
  {
     DELAY_MS(500);
  }
}