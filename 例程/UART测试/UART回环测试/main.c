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
/*
* ʹ��ʱ�̽�P3.3��P3.4�˿ڣ�����ͨ��Terminal I/O�ۿ����յ�������
*/
char databuf[256];
static int num=0;
__interrupt void UART_RX_IRQ_Handler()
{ 
  if(UART_GetITStatus(UART0,UART_RX_IRQn) == TRUE)   //�������ĳһ���жϱ�־
  {
    //�������ʸ�ʱ�������ж��ﲻҪ�������¼�������ᶪʧ���ݣ�������ʱ��ҲҪ���һ��
    databuf[num]=UART_GetChar     (UART0);                 //��ȡһ���ֽ�1���ֽ�
    num++;
    UART_ClearITPendingBit(UART0,UART_RX_IRQn);    //�������ĳһ���жϱ�־
  }
}
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  //OLED_Init();                    //OLED��ʼ��
  UART_Init        (UART0,115200);                      //��ʼ��uartxģ��,������115200�������ʸ�ʱ��ð���ʱ�� ���Щ
  Set_Vector_Handler(VECTOR_UART0,UART_RX_IRQ_Handler);    //���ý����ж�����
  UART_ITConfig  (UART0,UART_RX_IRQn,TRUE);                //�����ڽ����ж�
  
  EnableInterrupts();
  while(1)
  {  
    UART_Printf   (UART0," Hello-CTGU!\n UART-TEST!\n");         //�����ַ���
    DELAY_MS(500);
    if(num > 0)
    {
      printf("%s",databuf);//OLED_Printf_6x8(0,0,"%s",databuf);          //��ʾ���յ�������
      num = 0;
    }
  }
}