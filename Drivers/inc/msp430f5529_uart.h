#ifndef UART_H_
#define UART_H_

#include"common.h"

typedef enum 
{
  UART0               ,      //UCA0��TX_P33_RX_P34,
  UART1               ,      //UCA1��TX_P44_RX_P45
}UARTn;         //ģ��
typedef enum
{
  UART_RX_IRQn         =0x0001u, //�����ж�
  UART_TX_IRQn         =0x0002u, //�����ж�
}UART_IRQn;     //�ж�

extern STATUS  UART_Init        (UARTn, uint32_t BaudRate);                     //��ʼ��UARTģ��
extern void    UART_SetBaudRate (UARTn, uint32_t BaudRate);                     //���ô��ڲ�����
extern char    UART_GetChar     (UARTn);                                        //��ȡһ���ֽ�1���ֽ�
extern void    UART_PutChar     (UARTn, char data);                             //����1���ֽ�
extern void    UART_PutNChar    (UARTn, char * data, uint16_t Length);          //����n���ֽ�
extern void    UART_PutString   (UARTn, const char * str);                      //�����ַ���
extern void    UART_Printf      (UARTn, const char * fmt,...);                  //

extern void    UART_ITConfig    (UARTn, UART_IRQn, STATUS ITState);             //���ô����ж��Ƿ�ʹ��
extern STATUS  UART_GetITStatus (UARTn, UART_IRQn);                             //��ȡ����ĳһ���жϱ�־
extern void    UART_ClearITPendingBit(UARTn, UART_IRQn);                        //�������ĳһ���жϱ�־
 
#endif