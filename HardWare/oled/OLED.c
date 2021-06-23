#include "OLED.h"
#include "OLED_FONT.h"
#include "OLED_FONT_CHINESE.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
typedef enum
{
  CMD   =0u,//����ʱΪ�͵�ƽ
  DATA  =1u,//����ʱΪ�ߵ�ƽ
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
#ifdef OLED_RAM //�����Դ�
static uint8_t OLED_RAM_DATA[(OLED_HEIGHT/8)][OLED_WIDTH] ={0}; //�Դ�
static uint8_t line_position = 0,row_position = 0;   //���λ��
#endif
static void OLED_SetPosition(uint8_t line,uint8_t x);                              //��������
/*******************************************************************************
*  �������ƣ�OLED_PORT_Init(void)
*  ����˵����OLED�˿ڳ�ʼ������
*  ����˵������
*  �������أ���
*  ʹ��ʾ����
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
*  �������ƣ�OLED_WriteByte(CMD_OR_DATA DCvalue,uint8_t data)
*  ����˵����OLED����һ�ֽ�����
*  ����˵����CMD_OR_DATA DCvalue�����������
             uint8_t data       �����͵�����
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void OLED_WriteByte(CMD_OR_DATA DCvalue,uint8_t data)
{
  uint8_t i;
  OLED_DC = DCvalue; //DCvalue��1DC��Ϊ1 ����1DC��Ϊ0  1������ 0����?
#ifdef OLED_RAM //��������Դ�
  if(DCvalue == DATA) //��������ʱ����
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
    OLED_SDA = (data&0x80) ? 1 : 0;//�����ط�������
    OLED_SCL = 1;
    OLED_SCL = 0;		
    data <<= 1;    
  }
#endif
}
/*******************************************************************************
*  �������ƣ�OLED_SetPosition(uint8_t line,uint8_t x)
*  ����˵����OLED���ù��λ��
*  ����˵����uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void OLED_SetPosition(uint8_t line,uint8_t x)
{
  OLED_WriteByte(CMD,0xB0+line);            //������        
  OLED_WriteByte(CMD,x&0x0F);            //����ĳ�еĵ���λ��ַ 0x00-0x0F
  OLED_WriteByte(CMD,(x>>4)|0x10);       //����ĳ�еĸ���λ��ַ 0x10-0x1F
#ifdef OLED_RAM //��������Դ�
  line_position = line;
  row_position = x;
#endif
}
/*******************************************************************************
*  �������ƣ�OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t value)
*  ����˵����OLED��ĳһλ�û���,��Ҫ�����Դ�
*  ����˵����
            uint8_t x����λ�ã�0-127��
            uint8_t y����λ�ã�0-64��
            uint8_t value���õ��Ƿ���ʾ(0:����ʾ��1:��ʾ)
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
#ifdef OLED_RAM //�����Դ�
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t value)
{
    uint8_t data_temp; //data1 ��ǰ�������
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
*  �������ƣ�OLED_FillRect(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t data)
*  ����˵����OLED���ĳһ��������
*  ����˵����uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             uint8_t data���������Ƿ���ʾ(0:����ʾ��1:��ʾ)
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
#ifdef OLED_RAM //�����Դ�
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
*  �������ƣ�OLED_FillPart(uint8_t x0,uint8_t line0,uint8_t x1,uint8_t line1,uint8_t data)
*  ����˵����OLED���ĳһ��������
*  ����˵����
             uint8_t x0  ������ʼλ�ã�0-127��
             uint8_t line0������ʼλ�ã�0-7��
             uint8_t x1  ���н���λ�ã�0-127��
             uint8_t line1���н���λ�ã�0-7��
             uint8_t data���������Ƿ���ʾ(0:����ʾ��1:��ʾ) 

*  �������أ���
*  ʹ��ʾ����OLED_FillPart(0,0,OLED_WIDTH,7,0);  //����
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
#ifdef OLED_RAM //�����Դ�
      OLED_RAM_DATA[line][x] = 0;
#endif
      OLED_WriteByte(DATA,data);
    }
  }
}
/*******************************************************************************
*  �������ƣ�OLED_Clear(void)
*  ����˵����OLED��������������Դ棬��ͬ������
*  ����˵������
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void OLED_Clear(void)
{
  int line,x;
  
  for(line=0;line < (OLED_HEIGHT/8);line++)
  {
    OLED_SetPosition(line,0);
    for(x=0;x<OLED_WIDTH;x++)
    {
#ifdef OLED_RAM //�����Դ�
      OLED_RAM_DATA[line][x] = 0;
#endif
      OLED_WriteByte(DATA,0);
    }
  }
}
/*******************************************************************************
*  �������ƣ�OLED_Init(void)
*  ����˵����OLED��ʼ��
*  ����˵������
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void OLED_Init(void)
{
    uint16_t i;
    OLED_PORT_Init();
    // OLED_CS  = 0;  //оƬѡ������ �͵�ƽʹ��
    OLED_RST=0;  //��λ�ź�,����ʡ��
    for(i=0;i<10000;i++);
    OLED_RST=1;
    
   
    OLED_WriteByte(CMD,0xA8);    //set Multiplex Ratio
    OLED_WriteByte(CMD,0x3F);  
    
    OLED_WriteByte(CMD,0xD3);    //������ʾƫ��
    OLED_WriteByte(CMD,0x00);
    
    OLED_WriteByte(CMD,0x40);    //���ÿ�ʼ��ʾ�� 
    
    OLED_WriteByte(CMD,0xA1);    //���ö���ӳ��  A0h/A1h
    
    OLED_WriteByte(CMD,0xC8);    //����COM��ɨ�跽��  C0h/C8h
    
    OLED_WriteByte(CMD,0xDA);    //����COM�ܽŵ�Ӳ������
    OLED_WriteByte(CMD,0x12);
    
    OLED_WriteByte(CMD,0x81);    //���öԱȶ�
    OLED_WriteByte(CMD,0x7F);
    
    OLED_WriteByte(CMD,0xA4);    //��ֹ��Ļ��ʾ
    
    OLED_WriteByte(CMD,0xA6);    //����������ʾ
    
    OLED_WriteByte(CMD,0xD5);    //����OSCƵ��
    OLED_WriteByte(CMD,0x80);
    
    OLED_WriteByte(CMD,0x8D);    //Enable charge pupm regulator
    OLED_WriteByte(CMD,0x14);
    
    OLED_WriteByte(CMD,0xAF);    //����ʾ
    
    OLED_Clear();                //����
}
/*******************************************************************************
*  �������ƣ�OLED_DispChar(OLED_FONT font,uint8_t line,uint8_t x,char ascii)
*  ����˵����OLED��ʾһ���ַ�
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             char ascii  ��Ҫ��ʾ���ַ�
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void OLED_DispChar(OLED_FONT font,uint8_t line,uint8_t x,char ascii)
{
    uint8_t offect,i,height;
    const unsigned char *pFontData;
    if(ascii<' '||ascii>'~')return;
    OLED_SetPosition(line,x);
    offect = (uint8_t)(ascii - ' ');
    //��ȡ����������ݵ���ʼλ��
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
*  �������ƣ�OLED_DispStringLen(OLED_FONT font,uint8_t line,uint8_t x,char *str,uint8_t len)
*  ����˵����OLED��ʾһ��ָ�����ȵ��ַ���
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             char *str   ���ַ���
             uint8_t len ���ַ�������
*  �������أ���
*  ʹ��ʾ����
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
*  �������ƣ�OLED_DispStringAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  ����˵����OLED��ָ��λ����ʾһ�����ַ���
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             char *str   ��Ҫ��ʾ���ַ���
*  �������أ���
*  ʹ��ʾ����OLED_DispStringAt(FONT_6X8,0,0,"Hello!"); //��ʾһ���ַ���
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
*  �������ƣ�OLED_DispStringAtCEOL(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  ����˵����OLED��ָ��λ����ʾһ�����ַ������������ʾ��ĩβ
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             char *str   ���ַ���
*  �������أ���
*  ʹ��ʾ����OLED_DispStringAtCEOL(FONT_6X8,1,0,"Hello!"); //��ʾһ���ַ����������ʾ��ĩβ
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
    //�Ը���ʣ����ʾ��������
    OLED_FillPart(x,line,OLED_WIDTH-1,line,0);
}
/*******************************************************************************
*  �������ƣ�OLED_DispDecAt(OLED_FONT font,uint8_t line,uint8_t x,int32_t number,uint8_t len)
*  ����˵����OLED��ʾһ������������
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             int32_t number��Ҫ��ʾ������
             uint8_t len   ����ʾ��λ��
*  �������أ���
*  ʹ��ʾ����
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
*  �������ƣ�OLED_DispFolatAt(OLED_FONT font,uint8_t line,uint8_t x,float number,uint8_t len)
*  ����˵����OLED��ʾһ���ĸ�����
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             float number��Ҫ��ʾ�ĸ�����
             uint8_t len ����ʾ��λ��(������С����)
*  �������أ���
*  ʹ��ʾ����
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
*  �������ƣ�OLED_DispChineseAt(OLED_FONT font,uint8_t line,uint8_t x,char *str)
*  ����˵����OLED��ʾһ������ ��ʾ��������FONT_CHINESE.h�еģ�
            ����Ҫ���ú���ȡģ������������� CHINESE_16X16�����
                ͬʱ����ͬ�ĺ���˳�򽫺��ַ��� FONT_CHINESE_TABLE������
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
             uint8_t x,char *str��Ҫ��ʾ�ĺ����ַ���
*  �������أ���
*  ʹ��ʾ����
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
        //��ȡ����������ݵ���ʼλ��
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
*  �������ƣ�OLED_PrintfAt(OLED_FONT font,uint8_t y,uint8_t x,const char * fmt,...)
*  ����˵����OLED��ָ��λ�ô�ӡ
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             uint8_t line����λ�ã�0-7��
             uint8_t x   ����λ�ã�0-127��
*  �������أ���
*  ʹ��ʾ����
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
        if((position-lastposition)>TEXT_SIZE)//���һ��Ҫ��ʾ���ַ�����������16������Ҫ����
        {
           OLED_DispStringLen(font,line,x,&OLED_String[lastposition],TEXT_SIZE);
           line += (Font[font].height/8);//����
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
           OLED_DispStringLen(font,line,x,&OLED_String[lastposition],(position-lastposition));
           for(x+=(position-lastposition)*Font[font].width;x < OLED_WIDTH;x++)   //������δ��ʾ�Ĳ�������
           {
             OLED_WriteByte(DATA,0);  //д������
           }
           break;
        }
      } 
      lastposition=position+1;
      //���к�X=0,y����һ��
      x = 0;
      line += (Font[font].height/8);//����
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
*  �������ƣ�OLEDPrintf(OLED_FONT font,uint8_t y,uint8_t x,const char * fmt,...)
*  ����˵����OLED��ӡ
*  ����˵����OLED_FONT font�����壨OLED_FONTö�٣�
             
*  �������أ���
*  ʹ��ʾ����
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
  for(int i=0;i<=length;i++)//���һ��з�
  {
    if(OLED_String[i] == '\n')
    {
      uint8_t position=i;
      while(position>lastposition)
      {
        if((position-lastposition)>TEXT_SIZE)//���һ��Ҫ��ʾ���ַ�����������TEXT_SIZE����Ҳ��Ҫ����
        {
           OLED_DispStringLen(font,y,x,&OLED_String[lastposition],TEXT_SIZE);
           y += (Font[font].height/8);//����
           if(y>(OLED_HEIGHT/Font[font].height - 1))
           {
             y=0;
             OLED_Clear();//ȫ���������                                                
           }
           x=0;
           lastposition+=TEXT_SIZE;
        }
        else
        {
           OLED_DispStringLen(font,y,x,&OLED_String[lastposition],(position-lastposition));  //��ʾ�����ַ���
           for(x+=(position-lastposition)*Font[font].width;x < OLED_WIDTH;x++)   //������δ��ʾ�Ĳ�������
           {
             OLED_WriteByte(DATA,0);  //д������
           }
           break;
        }
      } 
      lastposition=position+1;
      //���к�X=0,y����һ��
      x=0;
      y += (Font[font].height/8);//����
      if(y>(OLED_HEIGHT/Font[font].height - 1))
      {
         y=0;
         OLED_Clear();
      }
    }
  }
  if(lastposition<length)//û�л��з�����ʣ�ಿ��û����ʾʱ
  {
    OLED_DispStringLen(font,y,x,&OLED_String[lastposition],(length-lastposition));
    x+=(length-lastposition)*Font[font].width;
    while(x>=(TEXT_SIZE*Font[font].width))
    {
       x-=(TEXT_SIZE*Font[font].width);
       y += (Font[font].height/8);//����
       if(y>(OLED_HEIGHT/Font[font].height - 1))
       {
         y=0;
         x=0;
         OLED_Clear();
       }
    }
    for(int i=x;i < OLED_WIDTH;i++)   //������δ��ʾ�Ĳ�������
    {
      OLED_WriteByte(DATA,0);  //д������
    }
  }
  va_end(ap);
}