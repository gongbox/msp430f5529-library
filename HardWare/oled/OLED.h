#ifndef _OLED_H_
#define _OLED_H_

#include "msp430f5529_gpio.h"

#define OLEDHardWareSPI     (0) //宏定义是否使用硬件SPI，使用则宏定义为1，不使用则宏定义为0
#if(OLEDHardWareSPI)
#include "msp430f5529_spi.h"    //需要包含spi.h的文件
//如果使用硬件SPI则需宏定义以下函数
//硬件SPI初始化函数，时钟线在没有数据传输时为高电平，主机先下降沿接收数据，再在上升沿发送数据
//波特率太慢也会显示乱码，不知道为什么
#define OLEDHardWareSPI_Init()            SPI_Master_Init(SPI3,30MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_4)
#define OLEDHardWareSPI_SendByte(data)    SPI_SendReadByte(SPI3,data)
#else
//如果使用IO口模拟SPI，则需宏定义以下IO口
#define OLED_SCL_PORT   P4
#define OLED_SCL_PIN    3

#define OLED_SDA_PORT   P4
#define OLED_SDA_PIN    0

#define OLED_SCL    PIN_OUT(OLED_SCL_PORT,OLED_SCL_PIN)
#define OLED_SDA    PIN_OUT(OLED_SDA_PORT,OLED_SDA_PIN)
#endif
//以下几个引脚  与是否使用SPI无关，所以必须宏定义使用
#define OLED_RST_PORT   P3
#define OLED_RST_PIN    7

#define OLED_DC_PORT    P8
#define OLED_DC_PIN     2

#define OLED_RST    PIN_OUT(OLED_RST_PORT,OLED_RST_PIN)
#define OLED_DC     PIN_OUT(OLED_DC_PORT,OLED_DC_PIN)

#define OLED_HEIGHT          64                 //高
#define OLED_WIDTH          128                 //宽

#define OLED_RAM                //宏定义是否开启开启显存，会消耗1K的RAM，但是只有开启后才能使用画点函数

typedef enum
{
    FONT_ASCII_6X8, FONT_ASCII_8X16, FONT_CHINESE_16X16,
} OLED_FONT; //OLED字体

void OLED_Init(void);                                                      //初始化
void OLED_Clear(void);                                                      //清屏
#ifdef OLED_RAM //开启显存
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t value);                   //画点
void OLED_FillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t data);   //画矩形区域
#endif
void OLED_FillPart(uint8_t x0, uint8_t line0, uint8_t x1, uint8_t line1, uint8_t data);     //画矩形区域
void OLED_DispChar(OLED_FONT font, uint8_t line, uint8_t x, char ascii); //显示一个字符        
void OLED_DispStringAt(OLED_FONT font, uint8_t line, uint8_t x, char *str); //在指定位置显示一个英文字符串        
void OLED_DispStringAtCEOL(OLED_FONT font, uint8_t line, uint8_t x, char *str); //在指定位置显示一个英文字符串并清除至末尾 
void OLED_DispStringLenAt(OLED_FONT font, uint8_t line, uint8_t x, char *str, uint8_t len); //在指定位置显示一个指定字符长度的英文字符串
void OLED_DispDecAt(OLED_FONT font, uint8_t line, uint8_t x, int32_t number, uint8_t len);  //在指定位置显示一个整形数字
void OLED_DispFolatAt(OLED_FONT font, uint8_t line, uint8_t x, float number, uint8_t len);  //在指定位置显示一个浮点数
void OLED_PrintfAt(OLED_FONT font, uint8_t line, uint8_t x, const char *fmt, ...);         //在指定位置打印
void OLED_Printf(OLED_FONT font, const char *fmt, ...);                    //打印 
void OLED_DispChineseAt(OLED_FONT font, uint8_t line, uint8_t x, char *str); //在指定位置显示一个中文字符串 

#endif
