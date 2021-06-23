#include "12864ser.h"
//#include "delay.h"
#include "gpio.h"
void delay_us(int us)
{
  us*=25;
  while(us--);
}
void delay_ms(int ms)
{
  ms*=25;
  while(ms--)
  {
      for(int i=200;i>0;i--);
  }
}
 void LCD_GPIO_Config(void)
{
	/*GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);	*/
  
        gpio_init(P8,1,GPO);
        gpio_init(P8,2,GPO);
}

/**************************LCD初始化***************************/
void init_lcd(void)
{
   //	设置 SID->B4   SCLK->B5
	LCD_GPIO_Config();
    
    clrram();         //清除显示 全屏清除
    delay_ms(1);
    wr_lcd(comm,0x06);//光标右移，AC自动加一，整体显示不移动
    wr_lcd(comm,0x0c);//整体显示ON,光标显示OFF,光标位置不反白闪烁
    wr_lcd(comm,0x01);//清除显示
}

/*************************clrram()******************************/
void clrram()
{
    wr_lcd(comm,0x30);//8-BIT控制接口，基本指令集动作
    wr_lcd(comm,0x01);//清除显示
    delay_us(180);
}

/*-----------清除lcd任意位置字符------------------*/
/*--------position为任一位置的起始地址------------*/
/*--length为长度，取值为1-64---/若只清一行最大为16*/
void clr_lcd(uchar x,uchar y0,uchar length)
{
    uint i;
    lcd_xy(x,y0);          //定位光标
    for(i=0;i<length;i++)
    {
        wr_lcd (dat,0x20);//填充空格
    }
}

/*********************写命令或数据*********************
   dat_comm 为 comm 即 0 时写命令
   dat_comm 为 dat  即 1 时写数据
******************************************************/
void wr_lcd(uchar dat_comm,uchar content)
{			
    uchar a,i,j;
    delay_us(50);
    a=content;             //要写的数据
    LCD_SCLK0;             //en=0;
    LCD_SID1;              //wr=1
    for(i=0;i<5;i++)       //数据时序
    {
       LCD_SCLK1;
       LCD_SCLK0;
    }
    LCD_SID0;              //wr=0
    LCD_SCLK1;             //en=1
    LCD_SCLK0;             //en=0
    if(dat_comm) LCD_SID1;
    else         LCD_SID0;

    LCD_SCLK1;
    LCD_SCLK0;
    LCD_SID0;
    LCD_SCLK1;
    LCD_SCLK0;

    for(j=0;j<2;j++)       //8次循环写写一字节数据
    {
       for(i=0;i<4;i++)
       {
          if(a&0x80)
              LCD_SID1;
          else
              LCD_SID0;
          a=a<<1;
          LCD_SCLK1;
          LCD_SCLK0;
       }
       LCD_SID0;
       for(i=0;i<4;i++)    //时钟
       {
          LCD_SCLK1;
          LCD_SCLK0;
       }
    }
}

/********************用于中间调用，写地址***********************/
void lcd_xy(uchar x,uchar y0)
{
    uchar pos=0;
    switch(y0)             //y0为行号
    {
        case 0: pos=0x80+x;break;
        case 1: pos=0x90+x;break;
        case 2: pos=0x88+x;break;
        case 3: pos=0x98+x;break;
        default:break;
    }
    wr_lcd(comm,pos);
}

/***********************显示单个字符***************************/
void wr_data(uchar x,uchar y0,uchar data )
{
    lcd_xy(x,y0);
    wr_lcd(dat,data);      //写入这个数据  
}

/************************写字符串******************************/
//X取值0-7，Y取值0-3
void wr_string(uchar x,uchar y0,uchar *p) 
{
     uchar cnt=0;          //传递过来的是指针 ，cnt为偏移量，然后一个字符一个字符的写入。
     lcd_xy(x,y0);
     while(*(p+cnt)!='\0')
     {
          wr_lcd(dat,*(p+cnt));//写数据
          cnt++;
     }
}
/***********************浮点型数据显示************************
num为浮点型小数，n为显示的小数点位数
不是四舍五入算法
当n=0时，不显示小数点
加小数点最多可显示10位
****************************************************************/
void wr_float(uchar x,uchar y0,float num,int n)
{
    uint i=1,j=0,k=0;
    uchar array[10]={'0'};
    float m=1;
    unsigned long int t;
    for(j=0;j<n;j++)                     
       {m*=10.0;}
    t=(unsigned long int)(num*m);              //将小数num转化为整数         
    while(num>=10.0)                  //计整数位数   
    {   
        i++;   
        num/=10.0;   
    }
    for(j=i+n+1;j>0;j--)              //倒序存数值进数组
    { 
      if(j!=i+1)
      {
          array[j-1]=t%10+48;
          t/=10;
      }
      else if(n==0)                   //当n=0时，不显示小数点
             array[i]='\0';
           else   array[i]=46;        //当n!=0时，array[i]存放小数点  小数点的ASCII码为46
    }
    array[i+n+1]='\0';                //最后一位存入结束标志
    wr_string(x,y0,array);
    for(k=0;k<10;k++)                 //清除缓存数据
        array[k]=48;
}

/************************整型数据显示*************************/
void wr_int(uchar x,uchar y0,uint num) //num范围：0-65535
{
    uint i=1,j=0,k=0;
    float temp;
    uchar array[5]={'0'};
    temp=(float)num;
    while(temp>=10.0)          //判断整数部分位数        
    {   
        i++;                   //i为整型数据位数
        temp/=10.0;   
    } 
    array[i]='\0';
    for(j=i;j>0;j--)           //反向取值存入数组 
    {   
       array[j-1]=num%10+48;   //取最低位数值       
       num/=10;                 
    }
    wr_string(x,y0,array);
    for(k=0;k<5;k++)           //清除缓存数据
        array[k]='0';
}




