#ifndef __LCD_H
#define __LCD_H	

#include "msp430f5529_gpio.h"

#define LCDHardWareSPI     (1) //宏定义是否使用硬件SPI，使用则宏定义为1，不使用则宏定义为0
#if(LCDHardWareSPI)
#include "msp430f5529_spi.h"    //需要包含spi.h的文件
//如果使用硬件SPI则需宏定义以下函数
//硬件SPI初始化函数，时钟线在没有数据传输时为高电平，主机先下降沿接收数据，再在上升沿发送数据
//波特率太慢也会显示乱码，不知道为什么
#define LCDHardWareSPI_Init()            SPI_Master_Init(SPI2,10MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_4)
#define LCDHardWareSPI_SendByte(data)    SPI_SendReadByte(SPI2,data)
#else
//如果使用IO口模拟SPI，则需宏定义以下IO口
#define LCD_SCL_PORT   P3
#define LCD_SCL_PIN    2

#define LCD_SDI_PORT   P3
#define LCD_SDI_PIN    0

#define LCD_SCL    PIN_OUT(LCD_SCL_PORT,LCD_SCL_PIN)
#define LCD_SDI    PIN_OUT(LCD_SDI_PORT,LCD_SDI_PIN)
#endif
//以下几个引脚  与是否使用SPI无关，所以必须宏定义使用
#define LCD_CS_PORT    P2
#define LCD_CS_PIN     7

#define LCD_RST_PORT   P2
#define LCD_RST_PIN    4

#define LCD_DC_PORT    P2
#define LCD_DC_PIN     0

#define LCD_CS     PIN_OUT(LCD_CS_PORT,LCD_CS_PIN)
#define LCD_RST    PIN_OUT(LCD_RST_PORT,LCD_RST_PIN)
#define LCD_DC     PIN_OUT(LCD_DC_PORT,LCD_DC_PIN)

void LCD_Init(void); 
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Write_DATA8(char da);
void LCD_Write_DATA16(uint16_t da);
void LCD_Write_REG(char da);
					  		 
#endif  
	 
	 



