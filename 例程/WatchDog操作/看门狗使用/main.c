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
  DisableInterrupts();            //��ֹ���ж�
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  WatchDog_Init(200);     //500MS�����Ź�����
  EnableInterrupts();              //�����ж�
  while(1)
  {
     DELAY_MS(500);
     LED_On  (LED1);               //ָʾ������������ʱʱ����ڿ��Ź�ʱ��200ms����������LED�������
  }
}