#ifndef LCD_API_H_
#define LCD_API_H_

#include "common.h"
#include "lcd.h"
//�û�����
#define DIR_DEFAULT     1       //����Ĭ�Ϸ��� (0~3)

#if (1)

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   320                 //��
#define LCD_W                   240                 //��

#define LCD_INIT()              LCD_Init()              //��ʼ��
#define LCD_PTLON(xs,ys,xe,ye)  LCD_SetPos(xs,ys,xe,ye);	    //����
#define LCD_WR_BYTE(data)       LCD_Write_DATA8(data)       //д����
#define LCD_WR_WORD(data)       LCD_Write_DATA16(data)       //д����

#endif
//�궨����ʾ�ַ�����ɫ
#define         TEXT_COLOR      RED
#define         BACK_COLOR      WHITE
//��ɫ
#define		RED	        0xf800
#define		GREEN		0x07e0
#define		BLUE		0x001f
#define		PURPLE		0xf81f
#define		YELLOW		0xffe0
#define		CYAN		0x07ff 		//����ɫ
#define		ORANGE		0xfc08
#define		BLACK		0x0000
#define		WHITE		0xffff

/*********************�ڲ�ʵ�� ***********************/
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
extern void LCD_Draw_Dot(uint16 x,uint16 y,uint16 color_dat);	//���㣬�����꣬�����꣬��ɫ
extern void LCD_Show_Picture(uint16 xs,uint16 ys,uint16 xe,uint16 ye,uint16 pic[]);
extern void LCD_Printf_8x16(uint16 y,uint16 x,const char * fmt,...);

#endif