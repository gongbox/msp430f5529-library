#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "common.h"
#include "msp430f5529_gpio.h"

#define NRF24L01HardWareSPI     (1) //宏定义是否使用硬件SPI，使用则宏定义为1，不使用则宏定义为0
#if(NRF24L01HardWareSPI)
#include "msp430f5529_spi.h"    //需要包含spi.h的文件
//如果使用硬件SPI则需宏定义以下函数
//硬件SPI初始化函数，SPI模式3：时钟线在没有数据传输时为低电平，主机先上升沿接收数据，再在下降沿发送数据
#define NRF2401HardWareSPI_Init()            SPI_Master_Init(SPI3,5MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_3)
#define NRF2401HardWareSPI_SendByte(data)    SPI_SendReadByte(SPI3,data)
#else
//如果使用IO口模拟SPI，则需宏定义以下IO口
#define NRF2401_SCL_PORT    P4
#define NRF2401_SCL_PIN     3

#define NRF2401_SIMO_PORT   P4
#define NRF2401_SIMO_PIN    1

#define NRF2401_SOMI_PORT   P4
#define NRF2401_SOMI_PIN    2

#define NRF2401_SCL    PIN_OUT(NRF2401_SCL_PORT,NRF2401_SCL_PIN)
#define NRF2401_SIMO   PIN_OUT(NRF2401_SIMO_PORT,NRF2401_SIMO_PIN)
#define NRF2401_SOMI   PIN_IN(NRF2401_SOMI_PORT,NRF2401_SOMI_PIN)
#endif
//以下几个引脚  与是否使用SPI无关，所以必须宏定义使用
#define NRF2401_CE_PORT     P3
#define NRF2401_CE_PIN      7

#define NRF2401_CSN_PORT    P8
#define NRF2401_CSN_PIN     2
    
#define NRF2401_IRQ_PORT    P2
#define NRF2401_IRQ_PIN     2

#define NRF2401_CE     PIN_OUT(NRF2401_CE_PORT,NRF2401_CE_PIN)
#define NRF2401_CSN    PIN_OUT(NRF2401_CSN_PORT,NRF2401_CSN_PIN)
#define NRF2401_IRQ    PIN_IN(NRF2401_IRQ_PORT,NRF2401_IRQ_PIN)
    
//返回常量
#define NRF_OK      (0x00)
#define NRF_ERR     (0x01)
#define MAX_TX      (0x10)     //达到最大发送次数中断
#define TX_OK       (0x20)     //TX发送完成中断
#define RX_OK       (0x40)     //接收到数据中断

//24L01发送接收数据宽度定义
#define ADR_WIDTH       5       //定义地址长度（3~5）
#define TX_ADR_WIDTH    ADR_WIDTH   	//5字节的地址宽度
#define RX_ADR_WIDTH    ADR_WIDTH   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
    
//本构件所实现的接口函数    
typedef struct s_NRF24L01
{
  volatile uint8_t * pReceiveData;    //接收数据指针
  void (*CallBack_IRQ_Init)   (void); //中断引脚初始化回调函数，请在该函数中配置为上升沿中断,并在该函数中调用NRF2401_IRQ_Handler函数
  void (*CallBack_RecevieData)(void); //接收到数据回调函数
  void (*CallBack_SendDataOk) (void); //发送数据成功回调函数
  void (*CallBack_SendOutTime)(void); //发送超时回调函数
  void (*CallBack_TxFull)     (void); //发送缓冲区满回调函数
}s_NRF24L01;

extern uint8_t NRF2401_Init(s_NRF24L01);                             //初始化NRF24L01模块

extern void  NRF2401_SetRXMode(void);                              //配置为接收模式
extern void  NRF2401_SetTXMode(void);                              //配置为发送模式

extern void  NRF2401_SendData(uint8_t *txbuf);                     //发送数据
extern uint8_t NRF2401_RecData(uint8_t *rxbuf);                      //接收数据

extern void  NRF2401_IRQ_Handler(void);                            //NRF24L01中断处理函数
#endif



