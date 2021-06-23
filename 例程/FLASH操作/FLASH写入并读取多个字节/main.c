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


__interrupt void TIMER1_A0_IRQ_Handler()
{
   P4OUT ^= BIT7;
}
uint8_t Data[5];
void main()
{
  DisableInterrupts();            //禁止总中断
  LED_Init(LED_ALL);              //LED灯初始化
  
  OLED_Init();                       //OLED初始化
 
  for(int i=0;i<5;i++)
  {
    Data[i] = Flash_Read(MAX_SEGMENT,i,uint8_t); 
    OLED_PrintfAt(FONT_ASCII_6X8,i,0,"Data[%d]=%d\n",i,Data[i]);//显示更新
  } 
  for(int i=0;i<5;i++)
  {
    Data[i]=i;
  }
  Flash_Erase_Segment(MAX_SEGMENT); 
  Flash_Write_buf (MAX_SEGMENT,0,5,Data);
  for(int i=0;i<5;i++)
  {
    Data[i]=0;
  }
  for(int i=0;i<5;i++)
  {
    Data[i]=Flash_Read(MAX_SEGMENT,i,uint8_t); 
    OLED_PrintfAt(FONT_ASCII_6X8,i,80,"%d\n",i,Data[i]);//显示更新
  }
  while(1)
  {  
    DELAY_MS(500);
  }
  
}




