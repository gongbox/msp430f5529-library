
#include "nokia_5110.h"
#include "english_6x8_pixel.h"
#include "write_chinese_string_pixel.h"
#include "picture.h"
#include "math.h"
#include "string.h"
/*-----------------------------------------------------------------------
LCD_init          : 5110LCD初始化
-----------------------------------------------------------------------*/


void LCD_init(void)
  {
    
    //IO口初始化
    P2SEL&=~(BIT6+BIT7);
    LCD_5110_DIR|=(0x01 << LCD_RST)+(0x01 << LCD_CE)+(0x01 << LCD_DC)+(0x01 << LCD_DIN)+(0x01<< LCD_CLK);
   
   
    //产生复位信号
    LCD_5110_OUT &= ~(0x01 << LCD_RST);
    for(int i=0;i<200;i++);
    LCD_5110_OUT |= (0x01 << LCD_RST);
    
    //使用扩展命令设置LCD模式
    LCD_write_byte(0x21, 0);	
    LCD_write_byte(0xc0, 0);	// 设置偏置电压对比度 0xc8为标准5V
    LCD_write_byte(0x06, 0);	// 温度校正
    LCD_write_byte(0x13, 0);	// 1:48
    
    // 使用基本命令
    LCD_write_byte(0x20, 0);	
    LCD_clear();	        // 清屏
    LCD_write_byte(0x0c, 0);	// 设定显示模式，正常显示
    
      
  }

/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);			
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数

输入参数：X       ：0－83
          Y       ：0－5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// column
    LCD_write_byte(0x80 | X, 0);          	// row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符

输入参数：c       ：显示的字符；
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;

    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }

/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
  }
/*-----------------------------------------------------------------------
LCD_write_chinese_string: 在LCD上显示汉字

输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；  
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
测试：
	LCD_write_chi(0,0,12,7,0,0);
	LCD_write_chi(0,2,12,7,0,0);
	LCD_write_chi(0,4,12,7,0,0);	
-----------------------------------------------------------------------*/                        
void LCD_write_chinese_string(unsigned char X, unsigned char Y, 
                              unsigned char ch_with,unsigned char num,
                              unsigned char line,unsigned char row)
  {
    unsigned char i,n;
    
    LCD_set_XY(X,Y);                             //设置初始位置
    
    for (i=0;i<num;)
      {
      	for (n=0; n<ch_with*2; n++)              //写一个汉字
      	  { 
      	    if (n==ch_with)                      //写汉字的下半部分
      	      {
      	        if (i==0) LCD_set_XY(X,Y+1);
      	        else
      	           LCD_set_XY((X+(ch_with+row)*i),Y+1);
              }
      	    LCD_write_byte(write_chinese[line+i][n],1);
      	  }
      	i++;
      	LCD_set_XY((X+(ch_with+row)*i),Y);
      }
  }
/*-----------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD

输入参数：data    ：写入的数据；
          command ：写数据/命令选择；

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
	LCD_5110_OUT &= ~(0x01 << LCD_CE);	// msp430
    
    if (command == 0)
     // PORTB &= ~LCD_DC ;	        // 传送命令
	//     LCD_DC = 0;
	  LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else
     // PORTB |= LCD_DC ;		        // 传送数据
    // LCD_DC = 1;
	 LCD_5110_OUT |= (0x01 << LCD_DC);
		for(i=0;i<8;i++)
		{
			if(dat&0x80)
				//SDIN = 1;
			   LCD_5110_OUT |= (0x01 << LCD_DIN);
			else
				//SDIN = 0;
			   LCD_5110_OUT &= ~(0x01 << LCD_DIN);
			//SCLK = 0;
			LCD_5110_OUT &= ~(0x01 << LCD_CLK);
			dat = dat << 1;
			//SCLK = 1;
			LCD_5110_OUT |= (0x01 << LCD_CLK);
		}
   // SPDR = data;			// 传送数据到SPI寄存器

    //while ((SPSR & 0x80) == 0);         // 等待数据传送完毕
	
    //PORTB |= LCD_CE ;			// 关闭LCD
    // LCD_CE = 1;
	 LCD_5110_OUT |= (0x01 << LCD_CE);
  }

//显示浮点数
//x,y:起点坐标  
//number:要显示的数字
//decimal:要保留的小数位
//用16字体
static float _fround[6]={0.49,0.05,0.005,0.0005,0.00005,0.000005};

void ftoa(float n,unsigned char decimals,char *str)
{
	float scale;
	unsigned char i,d;
	if (n<0.0) {n=-n; *str++='-';};
	if (decimals>5) decimals=5;
	n=n+_fround[decimals];
	i=0;
	scale=1.0;
	while (n>=scale) {scale=scale*10.0; ++i;};
	if (i==0) *str++='0';
	else
	while (i--)
	{
    	scale=floor(0.5+scale/10.0);
      	d=(unsigned char) (n/scale);
      	*str++=d+'0';
      	n=n-scale*d;
    };
	if (decimals==0) {*str=0; return;};
	*str++='.';
	while (decimals--)
    {
    	n=n*10.0;
      	d=(unsigned char) n;
      	*str++=d+'0';
      	n=n-d;
	};
	*str=0;
}
void LCD_ShowFloat(unsigned char x,unsigned char y,float number,unsigned char decimal)
{
	char a[]="1234567890";
        ftoa(number,decimal,a);
	LCD_write_english_string(x,y,a);
        //LCD_ShowString(x,y,200,16,16,a);
}
/*-----------------------------------------------------------------------------
函数名称：LCD_draw_bmp_pixel
函数功能：位图绘制函数
入口参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）
出口参数：无 
备 注： 
------------------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y, const unsigned char *map, unsigned char Pix_x,unsigned char Pix_y)
{
    unsigned int i,n;
    unsigned char row;
    //计算位图所占行数						    
    if (Pix_y%8==0) 		//如果为位图所占行数为整数
        row=Pix_y/8;      
    else
        row=Pix_y/8+1;		//如果为位图所占行数不是整数
    
    LCD_set_XY(X,Y);
    for (n=0;n<row;n++)		//换行
    {
        for(i=0;i<Pix_x;i++)
        {
          LCD_set_XY(X+i,Y+n);
          LCD_write_byte(map[i+n*Pix_x], 1);
         }                         
      }      
}


