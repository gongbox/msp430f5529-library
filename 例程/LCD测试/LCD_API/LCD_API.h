#ifndef LCD_API_H_
#define LCD_API_H_

#include "common.h"
#include "lcd.h"
//用户配置
#define DIR_DEFAULT     1       //定义默认方向 (0~3)

#if (1)

/*********************** API接口 ******************************/
//提供API接口给LCD调用

#define LCD_H                   320                 //高
#define LCD_W                   240                 //宽

#define LCD_INIT()              LCD_Init()              //初始化
#define LCD_PTLON(xs,ys,xe,ye)  LCD_SetPos(xs,ys,xe,ye);	    //开窗
#define LCD_WR_BYTE(data)       LCD_Write_DATA8(data)       //写数据
#define LCD_WR_WORD(data)       LCD_Write_DATA16(data)       //写数据

#endif
//宏定义显示字符串颜色
#define         TEXT_COLOR      RED
#define         BACK_COLOR      WHITE
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

/*********************内部实现 ***********************/
extern void LCD_Display_Full(uint16 color);
extern void LCD_Draw_Part(uint16 xs,uint16 ys,uint16 xe,uint16 ye,uint16 color_dat);
extern void LCD_Show_Char8X16(uint16 x, uint16 y, char c_dat,uint16 word_color,uint16 back_color);
extern void LCD_Show_NChar8X16(uint16 x, uint16 y, char *s_dat,int num,uint16 word_color,uint16 back_color);
extern void LCD_Show_String8X16(uint16 x, uint16 y, char *s_dat,uint16 word_color,uint16 back_color);
extern void LCD_Show_IntNumber8X16(uint16 x,uint16 y,int number,uint16 word_color,uint16 back_color);
extern void LCD_Show_FloatNumber8X16(uint16 x,uint16 y,float number,int decimals,uint16 word_color,uint16 back_color);
extern void LCD_Draw_Line(uint16 xs,uint16 ys,uint16 xe,uint16 ye,uint16 color_dat);
extern void LCD_Draw_Rectangle(uint16 xs,uint16 ys,uint16 xe,uint16 ye,uint16 color_dat);
extern void LCD_Draw_Circle(uint16 x,uint16 y,uint16 r,uint16 color_dat);
extern void LCD_Draw_Dot(uint16 x,uint16 y,uint16 color_dat);	//画点，横坐标，纵坐标，颜色
extern void LCD_Show_Picture(uint16 xs,uint16 ys,uint16 xe,uint16 ye,uint16 pic[]);
extern void LCD_Printf_8x16(uint16 y,uint16 x,const char * fmt,...);

#endif