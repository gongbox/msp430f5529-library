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
*ʹ��ʱ���غó���󣬴򿪴������֣����úô��ڲ����ʵ���Ϣ���򿪶˿ڣ����ɽ��յ���Ƭ�����͸��������ֵ���Ϣ
*/
char databuf[256];
static int num=0;
__interrupt void UART_RX_IRQ_Handler()
{ 
  if(UART_GetITStatus(UART1,UART_RX_IRQn) == TRUE)   //�������ĳһ���жϱ�־
  {
    //�������ʸ�ʱ�������ж��ﲻҪ�������¼�������ᶪʧ���ݣ�������ʱ��ҲҪ���һ��
    databuf[num]=UART_GetChar     (UART1);                 //��ȡһ���ֽ�1���ֽ�
    num++;
    UART_ClearITPendingBit(UART1,UART_RX_IRQn);    //�������ĳһ���жϱ�־
  }
}
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  
  UART_Init        (UART1,115200);                      //��ʼ��UART1ģ��,������115200�������ʸ�ʱ��ð���ʱ�� ���Щ
  Set_Vector_Handler(VECTOR_UART1,UART_RX_IRQ_Handler);    //���ý����ж�����
  UART_ITConfig  (UART1,UART_RX_IRQn,TRUE);                //�����ڽ����ж�
  
  EnableInterrupts();
  
  while(1)
  {  
    DELAY_MS(500);
    UART_Printf   (UART1," Hello-CTGU!\n UART-TEST!\n");         //�����ַ���
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