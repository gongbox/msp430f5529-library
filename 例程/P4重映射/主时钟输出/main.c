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
  
  GPIO_PinRemapConfig(P4,0,GPIO_Remap_MCLK);             //P4.0�˿���ӳ��ΪMCLK���
  GPIO_PinRemapConfig(P4,7,GPIO_Remap_MCLK);             //P4.7�˿���ӳ��ΪMCLK���
  EnableInterrupts();                //ʹ���ж�
  while(1)
  {
    
  }
}


