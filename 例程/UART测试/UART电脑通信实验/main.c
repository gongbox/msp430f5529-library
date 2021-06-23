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
 * @date       2016-01-30
 */
#include "include.h"
/*
*使用时下载好程序后，打开串口助手，设置好串口波特率等信息，打开端口，即可接收到单片机发送给串口助手的信息
*/
char databuf[256];
static int num=0;
__interrupt void UART_RX_IRQ_Handler()
{ 
  if(UART_GetITStatus(UART1,UART_RX_IRQn) == TRUE)   //清除串口某一个中断标志
  {
    //传输速率高时，接收中断里不要做其他事件，否则会丢失数据，另外主时钟也要设高一点
    databuf[num]=UART_GetChar     (UART1);                 //读取一个字节1个字节
    num++;
    UART_ClearITPendingBit(UART1,UART_RX_IRQn);    //清除串口某一个中断标志
  }
}
void main()
{
  DisableInterrupts();            //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  
  UART_Init        (UART1,115200);                      //初始化UART1模块,波特率115200，波特率高时最好把主时钟 设高些
  Set_Vector_Handler(VECTOR_UART1,UART_RX_IRQ_Handler);    //设置接收中断向量
  UART_ITConfig  (UART1,UART_RX_IRQn,TRUE);                //开串口接收中断
  
  EnableInterrupts();
  
  while(1)
  {  
    DELAY_MS(500);
    UART_Printf   (UART1," Hello-CTGU!\n UART-TEST!\n");         //发送字符串
    if(num > 0)
    {
      printf("%s",databuf);
      for(int i=0;i < num;i++)
      {
        databuf[i] = 0;
      }
      num = 0;
    }
  }
}