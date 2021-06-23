#include "OLED.h"
#include "OLED_FONT.h"
#include "OLED_FONT_CHINESE.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
typedef enum
{
  CMD   =0u,//命令时为低电平
  DATA  =1u,//数据时为高电平
}CMD_OR_DATA;
static const struct{
  uint8_t width;
  uint8_t height;
  const unsigned char *pData;
}Font[] =
{
  {.width=6,.height= 8,.pData= &ASCII_6X8[0][0]},
  {.width=8,.height=16,.pData=&ASCII_8X16[0][0]},
  {.width=16,.height=16,.pData=&CHINESE_16X16[0][0]},
};
#ifdef OLED_RAM //开启显存
static uint8_t OLED_RAM_DATA[(OLED_HEIGHT/8)][OLED_WIDTH] ={0}; //显存
static uint8_t line_position = 0,row_position = 0;   //光标位置
#endif
static void OLED_SetPosition(uint8_t line,uint8_t x);                              //设置坐标
/*******************************************************************************
*  函数名称：OLED_PORT_Init(void)
*  功能说明：OLED端口初始化程序
*  参数说明：无
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_PORT_Init(void)
{
#if(OLEDHardWareSPI)
  OLEDHardWareSPI_Init();
#else
  GPIO_Init(OLED_SCL_PORT,OLED_SCL_PIN,GPO);
  GPIO_Init(OLED_SDA_PORT,OLED_SDA_PIN,GPO);
#endif
  GPIO_Init(OLED_RST_PORT,OLED_RST_PIN,GPO);
  GPIO_Init(OLED_DC_PORT ,OLED_DC_PIN ,GPO);
}
/*******************************************************************************
*  函数名称：OLED_WriteByte(CMD_OR_DATA DCvalue,uint8_t data)
*  功能说明：OLED发送一字节数据
*  参数说明：CMD_OR_DATA DCvalue：命令或数据
             uint8_t data       ：发送的数据
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_WriteByte(CMD_OR_DATA DCvalue,uint8_t data)
{
  uint8_t i;
  OLED_DC = DCvalue; //DCvalue是1DC就为1 不是1DC就为0  1是数据 0是命?
#ifdef OLED_RAM //如果开启显存
  if(DCvalue == DATA) //发送数据时更新
  {
    OLED_RAM_DATA[line_position][row_position++] = data;
    if(row_position > 127)
    {
      row_position = 0;
      line_position ++;
    }
  }
#endif
#if(OLEDHardWareSPI)
  OLEDHardWareSPI_SendByte(data);
#else
  OLED_SCL=0;
  for(i=0;i < 8;i++)
  {
    OLED_SDA = (data&0x80) ? 1 : 0;//上升沿发送数据
    OLED_SCL = 1;
    OLED_SCL = 0;		
    data <<= 1;    
  }
#endif
}
/*******************************************************************************
*  函数名称：OLED_SetPosition(uint8_t line,uint8_t x)
*  功能说明：OLED设置光标位置
*  参数说明：uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_SetPosition(uint8_t line,uint8_t x)
{
  OLED_WriteByte(CMD,0xB0+line);            //设置行        
  OLED_WriteByte(CMD,x&0x0F);            //设置某列的低四位地址 0x00-0x0F
  OLED_WriteByte(CMD,(x>>4)|0x10);       //设置某列的高四位地址 0x10-0x1F
#ifdef OLED_RAM //如果开启显存
  line_position = line;
  row_position = x;
#endif
}
/*******************************************************************************
*  函数名称：OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t value)
*  功能说明：OLED在某一位置画点,需要开启显存
*  参数说明：
            uint8_t x：列位置（0-127）
            uint8_t y：行位置（0-64）
            uint8_t value：该点是否显示(0:不显示，1:显示)
*  函数返回：无
*  使用示例：
********************************************************************************/
#ifdef OLED_RAM //开启显存
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t value)
{
    uint8_t data_temp; //data1 当前点的数据
    uint8_t line = y/8;
    data_temp = OLED_RAM_DATA[line][x];
    if(value)
      data_temp |= 0x01<<(y%8);
    else
      data_temp &= ~(0x01<<(y%8));
    
    OLED_SetPosition(line,x);
    OLED_WriteByte(DATA,data_temp);
}
#endif
/*******************************************************************************
*  函数名称：OLED_FillRect(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t data)
*  功能说明：OLED填充某一矩形区域
*  参数说明：uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             uint8_t data：该区域是否显示(0:不显示，1:显示)
*  函数返回：无
*  使用示例：
********************************************************************************/
#ifdef OLED_RAM //开启显存
void OLED_FillRect(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t data)
{
  uint8_t y,x;
  for(y=y0;y <= y1;y++)
  { 
    for(x=x0;x <= x1;x++)
    {
      OLED_DrawPoint(x,y,data);
    }  
  }
}
#endif
/*******************************************************************************
*  函数名称：OLED_FillPart(uint8_t x0,uint8_t line0,uint8_t x1,uint8_t line1,uint8_t data)
*  功能说明：OLED填充某一矩形区域
*  参数说明：
             uint8_t x0  ：列起始位置（0-127）
             uint8_t line0：行起始位置（0-7）
             uint8_t x1  ：列结束位置（0-127）
             uint8_t line1：行结束位置（0-7）
             uint8_t data：该区域是否显示(0:不显示，1:显示) 

*  函数返回：无
*  使用示例：OLED_FillPart(0,0,OLED_WIDTH,7,0);  //清屏
********************************************************************************/
void OLED_FillPart(uint8_t x0,uint8_t line0,uint8_t x1,uint8_t line1,uint8_t data)
{
  uint8_t line,x;
  data = data ? 0xff : 0x00;
  for(line=line0;line <= line1;line++)
  {
    OLED_SetPosition(line,x0);
    for(x=x0;x < x1;x++)
    {
#ifdef OLED_RAM //开启显存
      OLED_RAM_DATA[line][x] = 0;
#endif
      OLED_WriteByte(DATA,data);
    }
  }
}
/*******************************************************************************
*  函数名称：OLED_Clear(void)
*  功能说明：OLED清屏，如果开启显存，则同样清零
*  参数说明：无
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_Clear(void)
{
  int line,x;
  
  for(line=0;line < (OLED_HEIGHT/8);line++)
  {
    OLED_SetPosition(line,0);
    for(x=0;x<OLED_WIDTH;x++)
    {
#ifdef OLED_RAM //开启显存
      OLED_RAM_DATA[line][x] = 0;
#endif
      OLED_WriteByte(DATA,0);
    }
  }
}
/*******************************************************************************
*  函数名称：OLED_Init(void)
*  功能说明：OLED初始化
*  参数说明：无
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_Init(void)
{
    uint16_t i;
    OLED_PORT_Init();
    // OLED_CS  = 0;  //芯片选择输入 低电平使能
    OLED_RST=0;  //复位信号,不可省略
    for(i=0;i<10000;i++);
    OLED_RST=1;
    
   
    OLED_WriteByte(CMD,0xA8);    //set Multiplex Ratio
    OLED_WriteByte(CMD,0x3F);  
    
    OLED_WriteByte(CMD,0xD3);    //设置显示偏移
    OLED_WriteByte(CMD,0x00);
    
    OLED_WriteByte(CMD,0x40);    //设置开始显示行 
    
    OLED_WriteByte(CMD,0xA1);    //设置段重映射  A0h/A1h
    
    OLED_WriteByte(CMD,0xC8);    //设置COM的扫描方向  C0h/C8h
    
    OLED_WriteByte(CMD,0xDA);    //设置COM管脚的硬件配置
    OLED_WriteByte(CMD,0x12);
    
    OLED_WriteByte(CMD,0x81);    //设置对比度
    OLED_WriteByte(CMD,0x7F);
    
    OLED_WriteByte(CMD,0xA4);    //禁止屏幕显示
    
    OLED_WriteByte(CMD,0xA6);    //设置正常显示
    
    OLED_WriteByte(CMD,0xD5);    //设置OSC频率
    OLED_WriteByte(CMD,0x80);
    
    OLED_WriteByte(CMD,0x8D);    //Enable charge pupm regulator
    OLED_WriteByte(CMD,0x14);
    
    OLED_WriteByte(CMD,0xAF);    //开显示
    
    OLED_Clear();                //清屏
}
/*******************************************************************************
*  函数名称：OLED_DispChar(OLED_FONT font,uint8_t line,uint8_t x,char ascii)
*  功能说明：OLED显示一个字符
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             char ascii  ：要显示的字符
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_DispChar(OLED_FONT font,uint8_t line,uint8_t x,char ascii)
{
    uint8_t offect,i,height;
    const unsigned char *pFontData;
    if(ascii<' '||ascii>'~')return;
    OLED_SetPosition(line,x);
    offect = (uint8_t)(ascii - ' ');
    //读取存放字体数据的起始位置
    pFontData = Font[font].pData+(Font[font].width*Font[font].height/8)*offect;
    
    for(height = 0;height < Font[font].height;height+=8)
    {
      OLED_SetPosition(line+(height/8),x);                   
      for(i=0;i<Font[font].width;i++)
      {
        OLED_WriteByte(DATA,*pFontData++);
      }
    }
}
/*******************************************************************************
*  函数名称：OLED_DispStringLen(OLED_FONT font,uint8_t line,uint8_t x,char *str,uint8_t len)
*  功能说明：OLED显示一个指定长度的字符串
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             char *str   ：字符串
             uint8_t len ：字符串长度
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_DispStringLen(OLED_FONT font,uint8_t line,uint8_t x,char *str,uint8_t len)
{
   while(len--)
   {
     if(*str)
     {
       OLED_DispChar(font,line,x,*str++);
     }
     else
     {
       OLED_DispChar(font,line,x,' ');
     }
     x += Font[font].width;
     if(x > (OLED_WIDTH - Font[font].width))
     {
        x = 0;
        line += (Font[font].height/8);
        if(line > ((OLED_HEIGHT/Font[font].height)-1))line = 0;
     }
   }
}
/*******************************************************************************
*  函数名称：OLED_DispStringAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  功能说明：OLED在指定位置显示一个的字符串
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             char *str   ：要显示的字符串
*  函数返回：无
*  使用示例：OLED_DispStringAt(FONT_6X8,0,0,"Hello!"); //显示一个字符串
********************************************************************************/
void OLED_DispStringAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
{
  while(*str != '\0')
  {
     OLED_DispChar(font,line,x,*str++);
     x += Font[font].width;
     if(x > (OLED_WIDTH - Font[font].width))
     {
        x = 0;
        line += (Font[font].height/8);
        if(line > ((OLED_HEIGHT/Font[font].height)-1))line = 0;
     }
  }
}
/*******************************************************************************
*  函数名称：OLED_DispStringAtCEOL(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  功能说明：OLED在指定位置显示一个的字符串，并清除显示至末尾
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             char *str   ：字符串
*  函数返回：无
*  使用示例：OLED_DispStringAtCEOL(FONT_6X8,1,0,"Hello!"); //显示一个字符串并清除显示至末尾
********************************************************************************/
void OLED_DispStringAtCEOL(OLED_FONT font,uint8_t line,uint8_t x,char *str)
{
    while(*str != '\0')
    {
      OLED_DispChar(font,line,x,*str++);
      x += Font[font].width;
      if(x > (OLED_WIDTH - Font[font].width))
      {
        x = 0;
        line += (Font[font].height/8);
        if(line > ((OLED_HEIGHT/Font[font].height)-1))line = 0;
      }
    }
    //对该行剩余显示区域清零
    OLED_FillPart(x,line,OLED_WIDTH-1,line,0);
}
/*******************************************************************************
*  函数名称：OLED_DispDecAt(OLED_FONT font,uint8_t line,uint8_t x,int32_t number,uint8_t len)
*  功能说明：OLED显示一个的整形数字
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             int32_t number：要显示的数字
             uint8_t len   ：显示的位数
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_DispDecAt(OLED_FONT font,uint8_t line,uint8_t x,int32_t number,uint8_t len)
{
  char buf[30];
  int8_t p,flag=0;
  uint32_t temp;
  if(number < 0)
  {
    number = -number;
    flag = 1;
  }
  temp = 10;
  for(p = len-1;p >= 0;p--)
  {
    buf[p] = ((number%temp)/(temp/10)) + '0';
    if(number < temp)break;
    temp = temp * 10;
  }
  if(flag)
  {
    buf[--p] = '-';
  }
   
  while(p-- > 0)
  {
    buf[p] = ' ';
  }

  buf[len] = 0;
  
  OLED_DispStringAt(font,line,x,buf);
}
/*******************************************************************************
*  函数名称：OLED_DispFolatAt(OLED_FONT font,uint8_t line,uint8_t x,float number,uint8_t len)
*  功能说明：OLED显示一个的浮点数
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             float number：要显示的浮点数
             uint8_t len ：显示的位数(不计算小数点)
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_DispFolatAt(OLED_FONT font,uint8_t line,uint8_t x,float number,uint8_t len)
{
  const float _fround[6]={0.49,0.05,0.005,0.0005,0.00005,0.000005};
  unsigned char i=0,d;
  char Str[20];
  char * str=Str;
  float scale = 1.0;
  
  if (number<0.0) 
  {
    number=-number;
    *str++='-';
  };
  if (len>5) 
    len=5;
  number = number + _fround[len];

  while (number>=scale) 
  {
    scale=scale*10.0;
    ++i;
  };
  if (i==0) 
    *str++='0';
  else
  {
    while (i--)
    {
      scale=(float)((int)(0.5+scale/10.0));
      d=(unsigned char) (number/scale);
      *str++=d+'0';
      number=number-scale*d;
     };
  }
  if (len==0) {*str=0; OLED_DispStringAt(font,line,x,Str);return;};
  *str++='.';
  while (len--)
  {
    number=number*10.0;
    d=(unsigned char)number;
    *str++=d+'0';
    number=number-d;
  }
  *str = 0;
  OLED_DispStringAt(font,line,x,Str);
  
}
/*******************************************************************************
*  函数名称：OLED_DispChineseAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  功能说明：OLED显示一个汉字 显示的内容在FONT_CHINESE.h中的，
            必须要先用汉字取模软件将数组存放在 CHINESE_16X16数组里，
                同时按相同的汉字顺序将汉字放入 FONT_CHINESE_TABLE数组里
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
             uint8_t x,char *str：要显示的汉字字符串
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_DispChineseAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
{
  const unsigned char *pFontData;
  uint8_t offect,i,height;
  
  int font_num = sizeof(FONT_CHINESE_TABLE)/sizeof(char *);

  while(*str)
  {
    for(offect=0;offect < font_num;offect++)
    {
      if(*str == *FONT_CHINESE_TABLE[offect] && *(str+1) == *(FONT_CHINESE_TABLE[offect]+1))
      {
        //读取存放字体数据的起始位置
        pFontData = Font[font].pData+(Font[font].width*Font[font].height/8)*offect;
        
        for(height = 0;height < Font[font].height;height+=8)
        {
          OLED_SetPosition(line+(height/8),x);                   
          for(i=0;i<Font[font].width;i++)
          {
            OLED_WriteByte(DATA,*pFontData++);
          }
        }
        x += Font[font].width;
        if(x > (OLED_WIDTH - Font[font].width))
        {
          x = 0;
          line += (Font[font].height/8);
          if(line > ((OLED_HEIGHT/Font[font].height)-1))line = 0;
        }
        break;
      }
    }
    str +=2;
  }
}
/*******************************************************************************
*  函数名称：OLED_PrintfAt(OLED_FONT font,uint8_t y,uint8_t x,const char * fmt,...)
*  功能说明：OLED在指定位置打印
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             uint8_t line：行位置（0-7）
             uint8_t x   ：列位置（0-127）
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_PrintfAt(OLED_FONT font,uint8_t line,uint8_t x,const char * fmt,...)
{
  char OLED_String[128];
  va_list ap;
  va_start(ap,fmt);
  vsprintf(OLED_String,fmt,ap);
  uint8_t lastposition=0;
  int length=strlen(OLED_String);
  int TEXT_SIZE=OLED_WIDTH/Font[font].width;
  for(int i=0;i<=length;i++)
  {
    if(OLED_String[i]=='\n')
    {
      uint8_t position=i;
      while(position>lastposition)
      {
        if((position-lastposition)>TEXT_SIZE)//如果一行要显示的字符串数量大于16，则需要换行
        {
           OLED_DispStringLen(font,line,x,&OLED_String[lastposition],TEXT_SIZE);
           line += (Font[font].height/8);//换行
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
           OLED_DispStringLen(font,line,x,&OLED_String[lastposition],(position-lastposition));
           for(x+=(position-lastposition)*Font[font].width;x < OLED_WIDTH;x++)   //将该行未显示的部分清零
           {
             OLED_WriteByte(DATA,0);  //写入数据
           }
           break;
        }
      } 
      lastposition=position+1;
      //换行后X=0,y下移一行
      x = 0;
      line += (Font[font].height/8);//换行
      if(line>(OLED_HEIGHT/Font[font].height - 1))line=0;
    }
  }
  if(lastposition<length)
  {
    OLED_DispStringLen(font,line,x,&OLED_String[lastposition],(length-lastposition));
  }
  va_end(ap);
} 
/*******************************************************************************
*  函数名称：OLEDPrintf(OLED_FONT font,uint8_t y,uint8_t x,const char * fmt,...)
*  功能说明：OLED打印
*  参数说明：OLED_FONT font：字体（OLED_FONT枚举）
             
*  函数返回：无
*  使用示例：
********************************************************************************/
void OLED_Printf(OLED_FONT font,const char * fmt,...)
{
  char OLED_String[128];
  static uint8_t x=0,y=0;
  va_list ap; 
  va_start(ap,fmt);
  vsprintf(OLED_String,fmt,ap);
  uint8_t lastposition=0;
  int length=strlen(OLED_String);
  int TEXT_SIZE=OLED_WIDTH/Font[font].width;
  for(int i=0;i<=length;i++)//查找换行符
  {
    if(OLED_String[i] == '\n')
    {
      uint8_t position=i;
      while(position>lastposition)
      {
        if((position-lastposition)>TEXT_SIZE)//如果一行要显示的字符串数量大于TEXT_SIZE，则也需要换行
        {
           OLED_DispStringLen(font,y,x,&OLED_String[lastposition],TEXT_SIZE);
           y += (Font[font].height/8);//换行
           if(y>(OLED_HEIGHT/Font[font].height - 1))
           {
             y=0;
             OLED_Clear();//全屏填充数据                                                
           }
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
           OLED_DispStringLen(font,y,x,&OLED_String[lastposition],(position-lastposition));  //显示该行字符串
           for(x+=(position-lastposition)*Font[font].width;x < OLED_WIDTH;x++)   //将该行未显示的部分清零
           {
             OLED_WriteByte(DATA,0);  //写入数据
           }
           break;
        }
      } 
      lastposition=position+1;
      //换行后X=0,y下移一行
      x=0;
      y += (Font[font].height/8);//换行
      if(y>(OLED_HEIGHT/Font[font].height - 1))
      {
         y=0;
         OLED_Clear();
      }
    }
  }
  if(lastposition<length)//没有换行符或有剩余部分没有显示时
  {
    OLED_DispStringLen(font,y,x,&OLED_String[lastposition],(length-lastposition));
    x+=(length-lastposition)*Font[font].width;
    while(x>=(TEXT_SIZE*Font[font].width))
    {
       x-=(TEXT_SIZE*Font[font].width);
       y += (Font[font].height/8);//换行
       if(y>(OLED_HEIGHT/Font[font].height - 1))
       {
         y=0;
         x=0;
         OLED_Clear();
       }
    }
    for(int i=x;i < OLED_WIDTH;i++)   //将该行未显示的部分清零
    {
      OLED_WriteByte(DATA,0);  //写入数据
    }
  }
  va_end(ap);
}