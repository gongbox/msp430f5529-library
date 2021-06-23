/*
Äþ²¨µ¥Æ¬»ú
pin   description
1     VCC(3.3V)
2     GND
3     CE
4     RST
5     DC
6     DIN
7     CLK
8     Vlcd(+5V)

*/
#ifndef __nokia_5110_h_
#define __nokia_5110_h_

#include "msp430g2553.h"

#define LCD_5110_DIR            P2DIR
#define LCD_5110_OUT		P2OUT

#define   LCD_RST   1
#define   LCD_CE    3
#define   LCD_DC    5
#define   LCD_DIN   6
#define   LCD_CLK   7

extern const unsigned char code_tu1[];
extern const unsigned char code_tu2[];
extern const unsigned char code_tu3[];
extern const unsigned char code_tu4[];
extern const unsigned char code_tu5[];
extern const unsigned char code_tu6[];
extern const unsigned char code_tu7[];
extern const unsigned char code_tu8[];
extern const unsigned char code_tu9[];
extern const unsigned char code_tu10[];
extern const unsigned char code_tu11[];
extern const unsigned char code_tu12[];
   

void LCD_init(void);
void LCD_clear(void);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row);
void LCD_write_char(unsigned char c);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_ShowFloat(unsigned char x,unsigned char y,float number,unsigned char decimal);
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,const unsigned char *map, unsigned char Pix_x,unsigned char Pix_y);
#endif
