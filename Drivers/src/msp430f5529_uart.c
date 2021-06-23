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
}; //引脚数组
/*******************************************************************************
*  函数名称：UART_Init(UARTn uartn,uint32_t BaudRate)
*  功能说明：初始化串口，设置波特率
*  参数说明：UARTn uartn       模块号（UART0~UART1）
             uint32_t BaudRate : 波特率，如9600、19200、115200等
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用示例：UART_Init(UART0,115200);   //初始化一个UART模块，波特率为115200
*******************************************************************************/
STATUS UART_Init(UARTn uartn,uint32_t BaudRate)
{      
  //引脚初始化
  GPIO_Init(UART_PIN[uartn].TX.Port,UART_PIN[uartn].TX.Pin,SEL);
  GPIO_Init(UART_PIN[uartn].RX.Port,UART_PIN[uartn].RX.Pin,SEL);
  
  UART_SetBaudRate (uartn, BaudRate);                            //设置串口波特率
  
  USCIX[uartn]->IE = 0u;                                         //关闭所有中断
  USCIX[uartn]->RXIFG = RESET;                                   //清除接收数据标志
  USCIX[uartn]->TXIFG = BIT_SET;                                     //置位发送缓冲区为空标志
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：UART_SetBaudRate (UARTn uartn, uint32_t BaudRate)
*  功能说明：设置串口波特率
*  参数说明：UARTn uartn       模块号（UART0~UART1）
             uint32_t BaudRate : 波特率，如9600、19200、115200等
*  函数返回：无
*******************************************************************************/
void UART_SetBaudRate (UARTn uartn, uint32_t BaudRate)
{
  float BRTemp = (float)g_sClock.SMCLK.nHZ/(float)BaudRate;
  
  USCIX[uartn]->CTL1 = UCSSEL__SMCLK + UCSWRST;                //设置时钟源
 
  USCIX[uartn]->BRW = (uint16_t)BRTemp;                                 //设置波特率
  USCIX[uartn]->BRS = (uint16_t)((BRTemp-(uint16_t)(BRTemp))*8.0);        //设置调整波特率
  USCIX[uartn]->BRF = 0u;
  
  //过采样模式
  /*
  USCIX[uartn]->BRW =(uint16_t)(BRTemp/16);                                 //设置波特率
  USCIX[uartn]->BRS = 0u;
  USCIX[uartn]->BRF =(uint16_t)((BRTemp/16.0-(uint16_t)(BRTemp/16.0))*16.0);  //设置调整波特率
  */
  USCIX[uartn]->CTL1 &= ~UCSWRST;       
}
/*******************************************************************************
*  函数名称：UART_GetChar    (UARTn uartn)
*  功能说明：无限时间等待串口接受一个字节
*  参数说明：UARTn uartn     模块号（UART0~UART1）
*  函数返回：接收到的字节数据
********************************************************************************/
char UART_GetChar    (UARTn uartn)
{
  while(USCIX[uartn]->RXIFG == RESET);               //等待接收到数据
  return (USCIX[uartn]->RXBUF);
}
/*************************************************************************
*  函数名称：UART_PutChar
*  功能说明：串口发送一个字节
*  参数说明：UARTn uartn      模块号（UART0~UART1）
*  函数返回：无
*  使用示例：UART_PutChar(UART0,0x02);   //发送一字节数据
*************************************************************************/  
void UART_PutChar(UARTn uartn,char data)
{
    if(data == '\n')
    {
      while (USCIX[uartn]->TXIFG == RESET);          //等待发送缓冲区为空
      USCIX[uartn]->TXBUF = '\r';                    // 
    }
    while (USCIX[uartn]->TXIFG == RESET);            //等待发送缓冲区为空
    USCIX[uartn]->TXBUF = data;                      // 
}
/*************************************************************************
*  函数名称：Uart_PutNChar
*  功能说明：串口发送指定长度个字节数据
*  参数说明：UARTn uartn      模块号（UART0~UART1）
*  函数返回：无
*  使用示例：char Buff[10];UART_PutNChar(UART0,&Buff[0],5);   //发送一个字符串的前5个字节
*************************************************************************/ 
void UART_PutNChar(UARTn uartn,char * data,uint16_t Length)
{
  for(int i=0; i < Length; i++)
  {
    UART_PutChar(uartn,*data++);
  }
}
/*************************************************************************
*  函数名称：Uart_PutString
*  功能说明：串口发送字符串
*  参数说明：UARTn uartn       模块号（UART0~UART1）
*  函数返回：无
*  使用示例：UART_PutString  (UART0 ,"Hello!");  //发送一个字符串
*************************************************************************/  
void UART_PutString  (UARTn uartn ,const char * str)
{
    while(*str != '\0')
    {
      UART_PutChar(uartn,*str++);
    }
}
/*************************************************************************
*  函数名称：UART_Printf
*  功能说明：串口发送字符串
*  参数说明：UARTn uartn     模块号（UART0~UART1）
*  函数返回：无
*  使用示例：UART_Printf(UART0 ,"Hello!:i=%d,J=%f",i,1.0);  //发送一个字符串
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
*  函数名称：UART_ITConfig      (UARTn uartn,UART_IRQn irqn,STATUS ITState)
*  功能说明：设置串口中断
*  参数说明：UARTn uartn       模块号（UART0~UART1）
             UART_IRQn irqn   ：中断事件
             STATUS ITState   ：是否使能中断
*  函数返回：无
*  使用示例：UART_ITConfig      (UART0,UART_RX_IRQn,ENABLE);  //设置UART0接收中断允许
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
*  函数名称：UART_GetITStatus  (UARTn uartn,UART_IRQn irqn)
*  功能说明：清除串口某一个中断标志
*  参数说明：UARTn uartn      模块号（UART0~UART1）
             UART_IRQn irqn  ：中断事件
*  函数返回：STATUS : TRUE-中断事件发生 FALSE-中断事件未发生
*  使用示例：if(TRUE == UART_GetITStatus  (UART0,UART_RX_IRQn)){...} //判断UART0接收中断事件是否发生
*************************************************************************/ 
STATUS UART_GetITStatus  (UARTn uartn,UART_IRQn irqn)
{
  return ((USCIX[uartn]->IFG & irqn) ? TRUE : FALSE);
}
/*************************************************************************
*  函数名称：UART_ClearITPendingBit(UARTn uartn,UART_IRQn irqn)
*  功能说明：清除串口某一个中断标志
*  参数说明：UARTn uartn      模块号（UART0~UART1）
             UART_IRQn irqn  ：中断事件
*  函数返回：无
*  使用示例：UART_ClearITPendingBit(UART0,UART_RX_IRQn);  //清除UART0接收中断标志位
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