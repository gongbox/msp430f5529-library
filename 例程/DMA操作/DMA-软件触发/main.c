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
  
  DMA_Init(DMA_CH2, &Sdata, &DData,DMATSEL_DMA_REQ,255,  DMA_SRCAddr_KeepOn|DMA_DSTAddr_KeepOn|DMA_WORD|DMA_Single|DMA_Edge);
           //ͨ��1��Դ��ַ��Ŀ�ĵ�ַ������Դ������255���֣�Դ��ַ���䣬Ŀ�ĵ�ַ���䣬�ִ��䣬���δ��������ش���
  EnableInterrupts();              //ʹ���ж�
  while(1)
  {  
    DMA_SoftwareTrigger(DMA_CH2);                      //DMA����һ�δ���
    OLED_PrintfAt(FONT_ASCII_6X8,3,0,"Sdata=%d,DData=%d",Sdata,DData);//��ʾ���� 
    DELAY_MS(200);
    Sdata++;
  }
}



















