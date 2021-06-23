#ifndef LCD_API_H_
#define LCD_API_H_

#include "common.h"
#include "lcd.h"

extern  uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

#if (1)

/*********************** API接口 ******************************/
//提供API接口给LCD调用

#define LCD_H                   320                 //高
#define LCD_W                   240                 //宽

#define LCD_INIT()              do{LCD_Init();LCD_Clear(BACK_COLOR);}while(0);   //初始化
#define LCD_PTLON(xs,ys,xe,ye)  LCD_SetPos(xs,ys,xe,ye);	    //开窗
#define LCD_WR_BYTE(data)       LCD_Write_DATA8(data)       //写数据
#define LCD_WR_WORD(data)       LCD_Write_DATA16(data)       //写数据

#endif
//宏定义显示字符串颜色
//颜色
#define		RED	        0xf800
#define		GREEN		0x07e0
#define		BLUE		0x001f
#define		PURPLE		0xf81f
#define		YELLOW		0xffe0
#define		CYAN		0x07ff 		//蓝绿色
#define		ORANGE		0xfc08
#define		BLACK		0x0000
#define		WHITE		0xffff

#define GRAY_TO_RGB565(gray)             ((uint16_t)((((uint8_t)(gray)>>3)<<11)|(((uint8_t)(gray)>>2)<<5)|((uint8_t)(gray)>>3)))
#define RGB565_TO_GRAY(rgb565)           ((uint8_t)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8)) 
#define RGB565_TO_COLOR(red,green,blue)  ((red<<11)|(green<<5)|(blue))
/*********************内部实现 ***********************/
extern void LCD_Clear(uint16_t color);
extern void LCD_FillPart(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat);
extern void LCD_DispChar(uint16_t x, uint16_t y, char c_dat,uint16_t word_color,uint16_t back_color);
extern void LCD_DispStringLen(uint16_t x, uint16_t y, char *s_dat,int num,uint16_t word_color,uint16_t back_color);
extern void LCD_DispStringAt(uint16_t x, uint16_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
extern void LCD_DispDecAt(uint16_t x,uint16_t y,int number,uint16_t word_color,uint16_t back_color);
extern void LCD_DispFolatAt(uint16_t x,uint16_t y,float number,int decimals,uint16_t word_color,uint16_t back_color);
extern void LCD_Draw_Line(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat);
extern void LCD_Draw_Rectangle(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat);
extern void LCD_Draw_Circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color_dat);
extern void LCD_Draw_Dot(uint16_t x,uint16_t y,uint16_t color_dat);	//画点，横坐标，纵坐标，颜色
extern void LCD_Show_Picture(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t pic[]);
extern void LCD_Printf_8x16(uint16_t x,uint16_t y,const char * fmt,...);
extern void LCD_Printf(const char * fmt,...);

#endif