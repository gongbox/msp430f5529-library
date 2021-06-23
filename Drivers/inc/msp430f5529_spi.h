#ifndef SPI_H_
#define SPI_H_

#include"common.h"

typedef enum
{                      //         STE    CLK    SOMI   SIMO
  SPI0            ,    //UCA0     P3.2   P2.7   P3.4   P3.3
  SPI1            ,    //UCA1     P4.3   P4.0   P4.5   P4.4
  SPI2            ,    //UCB0     P2.7   P3.2   P3.1   P3.0 
  SPI3            ,    //UCB1     P4.0   P4.3   P4.2   P4.1 
}SPIn;     //SPI模块
typedef enum
{
  SPI_LSB_First       ,  //先发送低位数据
  SPI_MSB_First       ,  //先发送高位数据
}SPI_FirstBit;       //发送首位选择
typedef enum
{
  SPI_STE_None        ,  //3线SPI
  SPI_STE_High_Enable ,  //4线SPI，从机给高电平使能主机
  SPI_STE_Low_Enable  ,  //4线SPI，从机给低电平使能主机
}SPI_STE_Mode;    //片选信号模式
typedef enum
{
  SPI_CLK_Mode_1      ,  //SPI模式1：时钟线在没有数据传输时为低电平，主机先上升沿发送数据，再在下降沿接收数据
  SPI_CLK_Mode_2      ,  //SPI模式2：时钟线在没有数据传输时为高电平，主机先下降沿发送数据，再在上升沿接收数据
  SPI_CLK_Mode_3      ,  //SPI模式3：时钟线在没有数据传输时为低电平，主机先上升沿接收数据，再在下降沿发送数据
  SPI_CLK_Mode_4      ,  //SPI模式4：时钟线在没有数据传输时为高电平，主机先下降沿接收数据，再在上升沿发送数据
}SPI_CLK_Mode;   //时钟线模式
typedef enum
{
  SPI_RX_IRQn         =0x0001u, //接收中断
  SPI_TX_IRQn         =0x0002u, //发送中断
}SPI_IRQn;   //中断

extern void    SPI_Master_Init (SPIn, uint32_t BaudRate, SPI_FirstBit, SPI_STE_Mode, SPI_CLK_Mode);//SPIn模块初始化
extern void    SPI_SetBaudRate (SPIn, uint32_t BaudRate);                       //设置SPI波特率
extern uint8_t SPI_SendReadByte(SPIn, uint8_t data);                            //发送并读取一字节数据
extern void    SPI_SendReadString(SPIn, char * SendData, char * ReceiveData, uint16_t Length); //发送并接收字符串

extern void    SPI_ITConfig   (SPIn, SPI_IRQn, STATUS ITState);                 //设置SPI中断是否使能 
extern STATUS  SPI_GetITStatus(SPIn, SPI_IRQn);                                 //获取SPI某一个中断标志
extern void    SPI_ClearITPendingBit(SPIn, SPI_IRQn);                           //清除SPI某一个中断标志
#endif