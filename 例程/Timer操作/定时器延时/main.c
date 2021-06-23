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

void main()
{
  DisableInterrupts();          //��ֹ���ж�
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  while(1)
  {  
    TIMER_Delay_Ms      (TIMER_A0,500);  //��ʱ500MS
    LED_Turn(LED_ALL);               //ָʾ����˸
  }
}
