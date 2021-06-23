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

uint16_t Sdata=10,DData=0;
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  OLED_Init();                       //OLED��ʼ��
  
  TIMER_Interval_Ms(TIMER_A1,500);    //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
  TIMER_ITConfig(TIMER_A1,TIMER_CCR0_IRQn,DISABLE);                    //��ֹTIMER��ĳһ���ж�,ʹ��DMAʱ����ص��ж�
  
  DMA_Init(DMA_CH1, &Sdata, &DData,DMATSEL_TA1CCR0,255,  DMA_SRCAddr_KeepOn|DMA_DSTAddr_KeepOn|DMA_WORD|DMA_Single|DMA_Edge);
           //ͨ��1��Դ��ַ��Ŀ�ĵ�ַ������Դ������255���֣�Դ��ַ���䣬Ŀ�ĵ�ַ���䣬�ִ��䣬���δ��������ش���
  EnableInterrupts();              //ʹ���ж�
  while(1)
  {  
    OLED_PrintfAt(FONT_ASCII_6X8,3,0,"Sdata=%d,DData=%d",Sdata,DData);//��ʾ���� 
    DELAY_MS(200);
    Sdata++;
  }
}















