#include "msp430f5529_uart.h"
#include "msp430f5529_clock.h"
#include "msp430f5529_system.h"
#include "msp430f5529_gpio.h"
#include <stdio.h>
#include <stdarg.h>
const struct
{
  GPIO_PIN TX;
  GPIO_PIN RX;
}UART_PIN[]=
{
  /*UART0*/{.TX={P3,3},.RX={P3,4}},        
  /*UART1*/{.TX={P4,4},.RX={P4,5}}
}; //��������
/*******************************************************************************
*  �������ƣ�UART_Init(UARTn uartn,uint32_t BaudRate)
*  ����˵������ʼ�����ڣ����ò�����
*  ����˵����UARTn uartn       ģ��ţ�UART0~UART1��
             uint32_t BaudRate : �����ʣ���9600��19200��115200��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ��ʾ����UART_Init(UART0,115200);   //��ʼ��һ��UARTģ�飬������Ϊ115200
*******************************************************************************/
STATUS UART_Init(UARTn uartn,uint32_t BaudRate)
{      
  //���ų�ʼ��
  GPIO_Init(UART_PIN[uartn].TX.Port,UART_PIN[uartn].TX.Pin,SEL);
  GPIO_Init(UART_PIN[uartn].RX.Port,UART_PIN[uartn].RX.Pin,SEL);
  
  UART_SetBaudRate (uartn, BaudRate);                            //���ô��ڲ�����
  
  USCIX[uartn]->IE = 0u;                                         //�ر������ж�
  USCIX[uartn]->RXIFG = RESET;                                   //����������ݱ�־
  USCIX[uartn]->TXIFG = BIT_SET;                                     //��λ���ͻ�����Ϊ�ձ�־
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�UART_SetBaudRate (UARTn uartn, uint32_t BaudRate)
*  ����˵�������ô��ڲ�����
*  ����˵����UARTn uartn       ģ��ţ�UART0~UART1��
             uint32_t BaudRate : �����ʣ���9600��19200��115200��
*  �������أ���
*******************************************************************************/
void UART_SetBaudRate (UARTn uartn, uint32_t BaudRate)
{
  float BRTemp = (float)g_sClock.SMCLK.nHZ/(float)BaudRate;
  
  USCIX[uartn]->CTL1 = UCSSEL__SMCLK + UCSWRST;                //����ʱ��Դ
 
  USCIX[uartn]->BRW = (uint16_t)BRTemp;                                 //���ò�����
  USCIX[uartn]->BRS = (uint16_t)((BRTemp-(uint16_t)(BRTemp))*8.0);        //���õ���������
  USCIX[uartn]->BRF = 0u;
  
  //������ģʽ
  /*
  USCIX[uartn]->BRW =(uint16_t)(BRTemp/16);                                 //���ò�����
  USCIX[uartn]->BRS = 0u;
  USCIX[uartn]->BRF =(uint16_t)((BRTemp/16.0-(uint16_t)(BRTemp/16.0))*16.0);  //���õ���������
  */
  USCIX[uartn]->CTL1 &= ~UCSWRST;       
}
/*******************************************************************************
*  �������ƣ�UART_GetChar    (UARTn uartn)
*  ����˵��������ʱ��ȴ����ڽ���һ���ֽ�
*  ����˵����UARTn uartn     ģ��ţ�UART0~UART1��
*  �������أ����յ����ֽ�����
********************************************************************************/
char UART_GetChar    (UARTn uartn)
{
  while(USCIX[uartn]->RXIFG == RESET);               //�ȴ����յ�����
  return (USCIX[uartn]->RXBUF);
}
/*************************************************************************
*  �������ƣ�UART_PutChar
*  ����˵�������ڷ���һ���ֽ�
*  ����˵����UARTn uartn      ģ��ţ�UART0~UART1��
*  �������أ���
*  ʹ��ʾ����UART_PutChar(UART0,0x02);   //����һ�ֽ�����
*************************************************************************/  
void UART_PutChar(UARTn uartn,char data)
{
    if(data == '\n')
    {
      while (USCIX[uartn]->TXIFG == RESET);          //�ȴ����ͻ�����Ϊ��
      USCIX[uartn]->TXBUF = '\r';                    // 
    }
    while (USCIX[uartn]->TXIFG == RESET);            //�ȴ����ͻ�����Ϊ��
    USCIX[uartn]->TXBUF = data;                      // 
}
/*************************************************************************
*  �������ƣ�Uart_PutNChar
*  ����˵�������ڷ���ָ�����ȸ��ֽ�����
*  ����˵����UARTn uartn      ģ��ţ�UART0~UART1��
*  �������أ���
*  ʹ��ʾ����char Buff[10];UART_PutNChar(UART0,&Buff[0],5);   //����һ���ַ�����ǰ5���ֽ�
*************************************************************************/ 
void UART_PutNChar(UARTn uartn,char * data,uint16_t Length)
{
  for(int i=0; i < Length; i++)
  {
    UART_PutChar(uartn,*data++);
  }
}
/*************************************************************************
*  �������ƣ�Uart_PutString
*  ����˵�������ڷ����ַ���
*  ����˵����UARTn uartn       ģ��ţ�UART0~UART1��
*  �������أ���
*  ʹ��ʾ����UART_PutString  (UART0 ,"Hello!");  //����һ���ַ���
*************************************************************************/  
void UART_PutString  (UARTn uartn ,const char * str)
{
    while(*str != '\0')
    {
      UART_PutChar(uartn,*str++);
    }
}
/*************************************************************************
*  �������ƣ�UART_Printf
*  ����˵�������ڷ����ַ���
*  ����˵����UARTn uartn     ģ��ţ�UART0~UART1��
*  �������أ���
*  ʹ��ʾ����UART_Printf(UART0 ,"Hello!:i=%d,J=%f",i,1.0);  //����һ���ַ���
*************************************************************************/ 
void UART_Printf(UARTn uartn ,const char * fmt,...)
{
  char UART_String[128];
  va_list ap;
  va_start(ap,fmt);
  vsprintf(UART_String,fmt,ap);
  UART_PutString  (uartn ,UART_String);
  va_end(ap);
}
/*************************************************************************
*  �������ƣ�UART_ITConfig      (UARTn uartn,UART_IRQn irqn,STATUS ITState)
*  ����˵�������ô����ж�
*  ����˵����UARTn uartn       ģ��ţ�UART0~UART1��
             UART_IRQn irqn   ���ж��¼�
             STATUS ITState   ���Ƿ�ʹ���ж�
*  �������أ���
*  ʹ��ʾ����UART_ITConfig      (UART0,UART_RX_IRQn,ENABLE);  //����UART0�����ж�����
*************************************************************************/ 
void  UART_ITConfig      (UARTn uartn,UART_IRQn irqn,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    USCIX[uartn]->IE |= irqn;
  }
  else
  {
    USCIX[uartn]->IE &=~irqn;
  }
}
/*************************************************************************
*  �������ƣ�UART_GetITStatus  (UARTn uartn,UART_IRQn irqn)
*  ����˵�����������ĳһ���жϱ�־
*  ����˵����UARTn uartn      ģ��ţ�UART0~UART1��
             UART_IRQn irqn  ���ж��¼�
*  �������أ�STATUS : TRUE-�ж��¼����� FALSE-�ж��¼�δ����
*  ʹ��ʾ����if(TRUE == UART_GetITStatus  (UART0,UART_RX_IRQn)){...} //�ж�UART0�����ж��¼��Ƿ���
*************************************************************************/ 
STATUS UART_GetITStatus  (UARTn uartn,UART_IRQn irqn)
{
  return ((USCIX[uartn]->IFG & irqn) ? TRUE : FALSE);
}
/*************************************************************************
*  �������ƣ�UART_ClearITPendingBit(UARTn uartn,UART_IRQn irqn)
*  ����˵�����������ĳһ���жϱ�־
*  ����˵����UARTn uartn      ģ��ţ�UART0~UART1��
             UART_IRQn irqn  ���ж��¼�
*  �������أ���
*  ʹ��ʾ����UART_ClearITPendingBit(UART0,UART_RX_IRQn);  //���UART0�����жϱ�־λ
*************************************************************************/ 
void  UART_ClearITPendingBit(UARTn uartn,UART_IRQn irqn)
{
  USCIX[uartn]->IFG &=~ irqn;
}

#if defined(DEBUG)&&defined(DEBUG_UART_PRINT)

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  UART_PutChar(Print_UART,(char)ch);
  return ch;
}
 
#endif