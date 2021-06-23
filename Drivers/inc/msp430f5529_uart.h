#ifndef UART_H_
#define UART_H_

#include"common.h"

typedef enum 
{
  UART0               ,      //UCA0，TX_P33_RX_P34,
  UART1               ,      //UCA1，TX_P44_RX_P45
}UARTn;         //模块
typedef enum
{
  UART_RX_IRQn         =0x0001u, //接收中断
  UART_TX_IRQn         =0x0002u, //发送中断
}UART_IRQn;     //中断

extern STATUS  UART_Init        (UARTn, uint32_t BaudRate);                     //初始化UART模块
extern void    UART_SetBaudRate (UARTn, uint32_t BaudRate);                     //设置串口波特率
extern char    UART_GetChar     (UARTn);                                        //读取一个字节1个字节
extern void    UART_PutChar     (UARTn, char data);                             //发送1个字节
extern void    UART_PutNChar    (UARTn, char * data, uint16_t Length);          //发送n个字节
extern void    UART_PutString   (UARTn, const char * str);                      //发送字符串
extern void    UART_Printf      (UARTn, const char * fmt,...);                  //

extern void    UART_ITConfig    (UARTn, UART_IRQn, STATUS ITState);             //设置串口中断是否使能
extern STATUS  UART_GetITStatus (UARTn, UART_IRQn);                             //获取串口某一个中断标志
extern void    UART_ClearITPendingBit(UARTn, UART_IRQn);                        //清除串口某一个中断标志
 
#endif