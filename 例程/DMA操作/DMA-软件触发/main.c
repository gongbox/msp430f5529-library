/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 平台主程序

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

uint16_t Sdata=10,DData=0;
void main()
{
  DisableInterrupts();            //禁止总中断
  LED_Init(LED_ALL);              //LED灯初始化
  OLED_Init();                       //OLED初始化
  
  DMA_Init(DMA_CH2, &Sdata, &DData,DMATSEL_DMA_REQ,255,  DMA_SRCAddr_KeepOn|DMA_DSTAddr_KeepOn|DMA_WORD|DMA_Single|DMA_Edge);
           //通道1，源地址，目的地址，触发源，传输255个字，源地址不变，目的地址不变，字传输，单次触发，边沿触发
  EnableInterrupts();              //使能中断
  while(1)
  {  
    DMA_SoftwareTrigger(DMA_CH2);                      //DMA启动一次传输
    OLED_PrintfAt(FONT_ASCII_6X8,3,0,"Sdata=%d,DData=%d",Sdata,DData);//显示更新 
    DELAY_MS(200);
    Sdata++;
  }
}



















