/********************************************************   
【平    台】龙丘K10/60DN/DX多功能开发板
【编    写】龙丘
【Designed】by Chiu Sir
【E-mail  】chiusir@yahoo.cn
【软件版本】V1.0
【最后更新】2013年1月16日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】CodeWarrior IAR6.10,6.30
【Target  】K60DX/DN
【Crystal 】50.000Mhz
【busclock】???.000MHz
【pllclock】100.000MHz    
***************************
------------------------------------
  使用说明： 
OLED电源使用3.3V。   
----------------
G    电源地
3.3V 接3.3V电源
CS   PORTc12  
SCK  PORTc13
SDA  PORTc14 
A0   PORTc15
RST  PORTc16  已接地，不用接
（5V的单片机需要在信号线上串接1K电阻，电源必须保证3.3V）
********************************************************/

#ifndef __SGP18T_LCD_BASE_H__
#define __SGP18T_LCD_BASE_H__

#include "gpio.h"
#include "spi.h"

#define LCDHardWareSPI     (1) //宏定义是否使用硬件SPI，使用则宏定义为1，不使用则宏定义为0
#if(LCDHardWareSPI)
//如果使用硬件SPI则需宏定义以下函数
//硬件SPI初始化函数，时钟线在没有数据传输时为高电平，主机先下降沿接收数据，再在上升沿发送数据
//波特率测试可以达到10M,波特率太慢也会显示乱码，不知道为什么
#define LCDHardWareSPI_Init()        SPI_Master_Init(SPI0,10000000,MSB_First,STE_None,SPI_Mode_4)
#define LCDHardWareSPI_Send(data)    SPI_SendReadByte(SPI0,data)
#else
//如果使用IO口模拟SPI，则需宏定义以下IO口

#define LCD_SCL_Port   P2
#define LCD_SDI_Port   P3

#define LCD_SCL_Pin    7
#define LCD_SDI_Pin    3

#define LCD_SCL    PIN_OUT(LCD_SCL_Port,LCD_SCL_Pin)
#define LCD_SDI    PIN_OUT(LCD_SDI_Port,LCD_SDI_Pin)
#endif
//以下几个引脚  与是否使用SPI无关，所以必须宏定义使用
#define LCD_CS_Port    P3
#define LCD_RST_Port   P2
#define LCD_DC_Port    P2

#define LCD_CS_Pin     2
#define LCD_RST_Pin    4
#define LCD_DC_Pin     0

#define LCD_CS     PIN_OUT(LCD_CS_Port,LCD_CS_Pin)
#define LCD_RST    PIN_OUT(LCD_RST_Port,LCD_RST_Pin)
#define LCD_DC     PIN_OUT(LCD_DC_Port,LCD_DC_Pin)

extern  uint16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_Init(void); 
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA16(uint16 da);
void LCD_WR_REG(char da);



#endif /*SGP18T_ILI9163B.h*/