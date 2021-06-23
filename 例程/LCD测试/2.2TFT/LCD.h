#ifndef __LCD_H
#define __LCD_H	

#include "gpio.h"
#include "spi.h"

#define LCDHardWareSPI     (1) //宏定义是否使用硬件SPI，使用则宏定义为1，不使用则宏定义为0
#if(LCDHardWareSPI)
//如果使用硬件SPI则需宏定义以下函数
//硬件SPI初始化函数，时钟线在没有数据传输时为高电平，主机先下降沿接收数据，再在上升沿发送数据
//波特率测试可以达到10M,波特率太慢也会显示乱码，不知道为什么
#define LCDHardWareSPI_Init()        SPI_Master_Init(SPI2,10000000,MSB_First,STE_None,SPI_Mode_4)
#define LCDHardWareSPI_Send(data)    SPI_SendReadByte(SPI2,data)
#else
//如果使用IO口模拟SPI，则需宏定义以下IO口

#define LCD_SCL_Port   P3
#define LCD_SDI_Port   P3

#define LCD_SCL_Pin    2
#define LCD_SDI_Pin    0

#define LCD_SCL    PIN_OUT(LCD_SCL_Port,LCD_SCL_Pin)
#define LCD_SDI    PIN_OUT(LCD_SDI_Port,LCD_SDI_Pin)
#endif
//以下几个引脚  与是否使用SPI无关，所以必须宏定义使用
#define LCD_CS_Port    P2
#define LCD_RST_Port   P2
#define LCD_DC_Port    P2

#define LCD_CS_Pin     7
#define LCD_RST_Pin    4
#define LCD_DC_Pin     0

#define LCD_CS     PIN_OUT(LCD_CS_Port,LCD_CS_Pin)
#define LCD_RST    PIN_OUT(LCD_RST_Port,LCD_RST_Pin)
#define LCD_DC     PIN_OUT(LCD_DC_Port,LCD_DC_Pin)

extern  uint16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_Init(void); 
void LCD_Clear(uint16 Color);
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Write_DATA8(char da);
void LCD_Write_DATA16(uint16 da);
void LCD_Write_REG(char da);
					  		 
#endif  
	 
	 



