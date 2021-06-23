#include "LCD_API.h"
#include "LCD_FONT.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//背景色，画笔色
uint16_t POINT_COLOR=RED,BACK_COLOR=WHITE; 
/*************************************************************************
*  函数名称：
*  功能说明：部分区域用一种颜色填充
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_FillPart(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat)
{
  uint16_t i,j;
  LCD_PTLON(xs,ys,xe,ye);
  for(j=0;j<(ye-ys+1);j++)
  {
    for(i=0;i<(xe-xs+1);i++)
    {
      LCD_WR_WORD(color_dat);
    }
  }  
}
/*************************************************************************
*  函数名称：
*  功能说明：全屏用一种颜色填充
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Clear(uint16_t color)
{
  unsigned int i,j;	
  LCD_PTLON(0,0,LCD_W,LCD_H);
  for(i=0;i<=LCD_H;i++)
  {
    for(j=0;j<=LCD_W;j++)
    {
       LCD_WR_WORD(color);
    }
  }
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置显示一个字符
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_DispChar(uint16_t x, uint16_t y, char c_dat,uint16_t word_color,uint16_t back_color)
{ 
  if(c_dat<' '||c_dat>'~')return;
  uint16_t i,j;
  char temp;
  c_dat=c_dat-' ';
  LCD_PTLON(x,y,x+7,y+15);			//定位字符显示区域
  for(j=0; j<16; j++)
  {
    temp=asc2_1608[c_dat][j];
    for(i=0; i<8; i++)
    {
      if((temp) & (0x01<<i))
      { 
        LCD_WR_WORD(word_color);
      }
      else
      {
        LCD_WR_WORD(back_color);
      }
    }
  }
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_DispStringLen(uint16_t x, uint16_t y, char *s_dat,int num,uint16_t word_color,uint16_t back_color)
{
  while(num--)
  {
    LCD_DispChar( x, y,*s_dat++, word_color, back_color);
    x+=8;
    if(x>LCD_W-8)
    {
       x=0;
       y+=16;
    }
  }
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置显示一个字符串
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_DispStringAt(uint16_t x, uint16_t y, char *s_dat,uint16_t word_color,uint16_t back_color)
{
  while(*s_dat!='\0') 
  {
    LCD_DispChar(x, y, *s_dat++,word_color,back_color);
    x+=8;
    if(x>LCD_W-8)
    {
       x=0;
       y+=16;
    }
  }
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置显示一个整数
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_DispDecAt(uint16_t x,uint16_t y,int number,uint16_t word_color,uint16_t back_color)
{ 
  char str[7],j,k=0,i;
  unsigned int c=10000;
  if(number<0)
    {str[0]='-';number=-number;}
  else str[0]=' ';
  for( i=1;i<=5;i++)
  {
    j=number/c%10;
    if(j==0&&k==0&&i<5)
    {
      str[i]=' ';
    }
    else
    {
      k=1;
      str[i]=j+'0';
    }
    c/=10;
  }
  str[6]='\0';
  LCD_DispStringAt(x,y,str,word_color,back_color);
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置显示一个浮点数，注意若是之前显示的浮点数位数多余当前的，则可能会保留上一次的显示
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_DispFolatAt(uint16_t x,uint16_t y,float number,int decimals,uint16_t word_color,uint16_t back_color)
{
  const float _fround[6]={0.49,0.05,0.005,0.0005,0.00005,0.000005};
  char Str[15];
  char * str=Str;
  float scale;
  unsigned char i,d;
  if (number<0.0) {number=-number; *str++='-';};
  if (decimals>5) decimals=5;
  number=number+_fround[decimals];
  i=0;
  scale=1.0;
  while (number>=scale) {scale=scale*10.0; ++i;};
  if (i==0) *str++='0';
  else
    while (i--)
    {
      scale=(float)((int32_t)(0.5+scale/10.0));
      d=(unsigned char) (number/scale);
      *str++=d+'0';
      number=number-scale*d;
     };
  if (decimals==0) {*str=0; LCD_DispStringAt(x,y,Str,word_color,back_color);return;};
  *str++='.';
  while (decimals--)
  {
    number=number*10.0;
    d=(unsigned char)number;
    *str++=d+'0';
    number=number-d;
  }
  *str=0;
  LCD_DispStringAt(x,y,Str,word_color,back_color);
}
/*************************************************************************
*  函数名称：
*  功能说明：指定两个点连线
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Draw_Line(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat)
{
  uint16_t i,ds;
  int dx,dy,inc_x, inc_y;
  int xerr = 0, yerr = 0;				//初始化变量

  if(xs==xe)	  						//如果是画垂直线则只需对竖直坐标计数
  {
    LCD_PTLON(xs,ys,xe,ye);//LCD_SetPos(xs,ys,xe,ye);
    for(i=0;i<(ye-ys+1);i++)
    {
       LCD_WR_WORD(color_dat);
    }
  }
  else if(ys==ye)						//如果是水平线则只需要对水平坐标计数
  {
    LCD_PTLON(xs,ys,xe,ye);//LCD_SetPos(xs,ys,xe,ye);
    for(i=0;i<(xe-xs+1);i++)
    {
       LCD_WR_WORD(color_dat);
    }
  } 
  else											//如果是斜线，则重新计算，使用画点函数画出直线
  {
    dx = xe - xs;								//计算坐标增量
    dy = ye - ys;

    if(dx > 0) inc_x = 1;						//设置单步方向
    else 
    {
      inc_x = -1; dx = -dx;
    }
    if(dy > 0) inc_y = 1;						//设置单步方向
    else
    {
      inc_y = -1; dy = -dy;
    }

    if(dx > dy) ds = dx;						//选取基本增量坐标轴
    else		
      ds = dy;
    for(i = 0; i <= ds+1; i++)					//画线输出
    {
      LCD_PTLON(xs,ys,xe,ye);//LCD_draw_dot(xs, ys,color_dat);	//画点
      xerr += dx;
      yerr += dy;
      if(xerr > ds)
      {
        xerr -= ds;
        xs   += inc_x;
      }
      if(yerr > ds)
      {
        yerr -= ds;
        ys   += inc_y;
      }
    }
  }  
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置画一个长方形
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Draw_Rectangle(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color_dat)
{
  LCD_Draw_Line(xs,ys,xs,ye,color_dat);	  //画矩形左边
  LCD_Draw_Line(xe,ys,xe,ye,color_dat);	  //画矩形右边
  LCD_Draw_Line(xs,ys,xe,ys,color_dat);	  //画矩形上边
  LCD_Draw_Line(xs,ye,xe,ye,color_dat);	  //画矩形下边
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置画一个确定半径的圆
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Draw_Circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color_dat)
{
  unsigned char dx, dy = r; 
  if((x>=r) &&((128-x)>=r) && (y>=r) && ((160-y)>=r))		//确定所画圆在屏幕范围内，没有超出最外边，(暂不支持与屏幕边相交)
  {
    for(dx = 0; dx <= r; dx++)
    {
      while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
      LCD_Draw_Dot(x + dx, y - dy, color_dat);
      LCD_Draw_Dot(x - dx, y - dy, color_dat);
      LCD_Draw_Dot(x - dx, y + dy, color_dat);
      LCD_Draw_Dot(x + dx, y + dy, color_dat);
      LCD_Draw_Dot(x + dy, y - dx, color_dat);
      LCD_Draw_Dot(x - dy, y - dx, color_dat);
      LCD_Draw_Dot(x - dy, y + dx, color_dat);
      LCD_Draw_Dot(x + dy, y + dx, color_dat);
    }
  }
}
/*************************************************************************
*  函数名称：
*  功能说明：指定位置画一个点
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Draw_Dot(uint16_t x,uint16_t y,uint16_t color_dat)
{
  LCD_PTLON(x,y,x,y); 
  LCD_WR_WORD(color_dat);
}
/*************************************************************************
*  函数名称：
*  功能说明：指定区域显示一副图像
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void LCD_Show_Picture(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t pic[])
{
  unsigned int i,j,k=0;

  LCD_PTLON(xs,ys,xe,ye);
  for(i=0;i<ye-ys;i++)
  { 
    for(j=0;j<xe-xs;j++)
    {
      LCD_WR_WORD(pic[k++]);	
    }
  }
}
/*************************************************************************
*  函数名称：LCD_Printf_8x16
*  功能说明：显示字符串
*  参数说明：
*  函数返回：无
*  使用示例：LCD_Printf_8x16("Hello!:i=%d,J=%f",i,1.0);  //发送一个字符串
*************************************************************************/  
void LCD_Printf_8x16(uint16_t x,uint16_t y,const char * fmt,...)
{
  static char LCD_String[256];
  va_list ap;
  va_start(ap,fmt);
  vsprintf(LCD_String,fmt,ap);
  uint8_t lastposition=0;
  int length=strlen(LCD_String);
  int TEXT_SIZE=LCD_W/8;
  for(int i=0;i <= length;i++)
  {
    if(LCD_String[i] == '\n')
    {
      uint8_t position=i;
      while(position>lastposition)
      {
        if((position-lastposition)>TEXT_SIZE)//如果一行要显示的字符串数量大于16，则需要换行
        {
           LCD_DispStringLen(x,y,&LCD_String[lastposition],TEXT_SIZE,POINT_COLOR,BACK_COLOR);
           y+=16;//换行
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
          LCD_DispStringLen(x,y,&LCD_String[lastposition],(position-lastposition),POINT_COLOR,BACK_COLOR);
          for(x+=(position-lastposition)*8;x < LCD_W;x+=8)   //将该行未显示的部分清零
          {
             LCD_DispChar(x,y,' ',POINT_COLOR,BACK_COLOR);
          }
          break;
        }
      } 
      lastposition=position+1;
      //换行后X=0,y下移一行
      x=0;
      y+=16;
      if(y>=LCD_H)y=0;
    }
  }
  if(lastposition<length)
    LCD_DispStringLen(x,y,&LCD_String[lastposition],(length-lastposition),POINT_COLOR,BACK_COLOR);
  va_end(ap);
  free(LCD_String);
}
/*************************************************************************
*  函数名称：LCD_Printf
*  功能说明：显示字符串
*  参数说明：
*  函数返回：无
*  使用示例：LCD_Printf"Hello!:i=%d,J=%f",i,1.0);  //发送一个字符串
*************************************************************************/  
void LCD_Printf(const char * fmt,...)
{
  static char LCD_String[256];
  static uint16_t x=0,y=0;
  va_list ap;
  va_start(ap,fmt);
  vsprintf(LCD_String,fmt,ap);
  uint8_t lastposition=0;
  int length=strlen(LCD_String);          
  int TEXT_SIZE=LCD_W/8;
  for(int i=0;i<=length;i++)
  {
    if(LCD_String[i]=='\n')     //遇到换行符
    {
      uint8_t position=i;
      while(position>lastposition)  //若有一段字符串没有显示，则显示完为止
      {
        if((position-lastposition)>TEXT_SIZE)//如果一行要显示的字符串数量大于16，则需要换行
        {
           LCD_DispStringLen(x,y,&LCD_String[lastposition],TEXT_SIZE,POINT_COLOR,BACK_COLOR);
           y+=16;//换行
           if(y>=LCD_H)
           {
             y=0;
             LCD_Clear(BACK_COLOR);
           }
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
          LCD_DispStringLen(x,y,&LCD_String[lastposition],(position-lastposition),POINT_COLOR,BACK_COLOR);
          for(x+=(position-lastposition)*8;x < LCD_W;x+=8)   //将该行未显示的部分清零
          {
             LCD_DispChar(x,y,' ',POINT_COLOR,BACK_COLOR);
          }
          break;
        }
      } 
      lastposition=position+1;
      //换行后X=0,y下移一行
      x=0;
      y+=16;
      if(y>=LCD_H)
      {
        y=0;
        LCD_Clear(BACK_COLOR);
      }
    }
  }
  if(lastposition<length)
  {
    LCD_DispStringLen(x,y,&LCD_String[lastposition],(length-lastposition),POINT_COLOR,BACK_COLOR);
    x+=(length-lastposition)*8;
    while(x>=LCD_W)
    {
       y+=16;
       x-=LCD_W;
       if(y>=LCD_H)
       {
         y=0;
         LCD_Clear(BACK_COLOR);
       }
    }
    for(int i=x;i < LCD_W;i++)   //将该行未显示的部分清零
    {
      LCD_DispChar(i,y,' ',POINT_COLOR,BACK_COLOR);
    }
  }
  va_end(ap);
}
