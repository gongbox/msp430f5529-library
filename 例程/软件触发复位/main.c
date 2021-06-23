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

void main()
{
  DisableInterrupts();             //��ֹ���ж�
  
  LED_Init(LED_ALL);               //LED�Ƴ�ʼ��
  
  EnableInterrupts();              //�����ж�
  while(1)
  {
     SoftWare_RESET();             //�������һ�θ�λ
     LED_On  (LED0);               //ָʾ������������������˸�λ����������LED�������
  }
}