/********************************************************   
【平    台】龙丘K10/60DN/DX多功能开发板
【编    写】龙丘
【Designed】by Chiu Sir
【E-mail  】chiusir@yahoo.cn
【软件版本】V1.0
【最后更新】2013年1月16日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】CodeWarrior IAR10,6.30
【Target  】K60DX/DN
【Crystal 】50.000Mhz
【busclock】???.000MHz
【pllclock】100.000MHz    
***************************
------------------------------------ 
龙邱TFT1.8模块使用说明： 
电源使用3.3V。   
----------------
G    电源地
3.3V 接3.3V电源
CS   PORTc12  已接地，不用接
SCK  PORTc13
SDA  PORTc14 
A0   PORTc15
RST  PORTc16  
（5V的单片机需要在信号线上串接1K电阻，电源必须保证3.3V）
********************************************************/
#include "LQ_SGP18T.h"
#include "gpio.h"
#include "System.h"
#include "LCD_FONT.h"	 
void LCD_Port_Init()
{
#if(LCDHardWareSPI)
  LCDHardWareSPI_Init();
#else
  GPIO_Init(LCD_SCL_Port,LCD_SCL_Pin,GPO);
  GPIO_Init(LCD_SDI_Port,LCD_SDI_Pin,GPO);
#endif
  GPIO_Init(LCD_RST_Port,LCD_RST_Pin,GPO);
  GPIO_Init(LCD_DC_Port,LCD_DC_Pin,GPO);
  GPIO_Init(LCD_CS_Port, LCD_CS_Pin,GPO);
}
void LCD_Writ_Bus(char data)   //串行数据写入
{	
  LCD_CS=0;
#if(LCDHardWareSPI)
  LCDHardWareSPI_Send(data);
#else
  OLED_SCL=0;
  for(uint8 i=0;i<8;i++)
  {
    OLED_SDA=(data&0x80)==0x80?1:0;//上升沿发送数据
    OLED_SCL=1;
    OLED_SCL=0;		
    data<<=1;    
  }
#endif 
  LCD_CS=1;
} 
void LCD_Write_DATA8(char da) //发送数据-8位参数
{
    LCD_DC=1;
	LCD_Writ_Bus(da);
}  
 void LCD_Write_DATA16(uint16 da)
{
    LCD_DC=1;
	LCD_Writ_Bus(da>>8);
	LCD_Writ_Bus(da);
}	  
void LCD_Write_Command(char da)	 
{
    LCD_DC=0;
        LCD_Writ_Bus(da);
}
 void LCD_Write_Command_DATA(int reg,int da)
{
    LCD_Write_Command(reg);
	LCD_WR_DATA16(da);
}
void LCD_SetPos(unsigned int xs,unsigned int ys,unsigned int xe,unsigned int ye)
{  
        LCD_Write_Command(0x2A);      	//Colulm addRSTs set
	LCD_Write_DATA8(0x00);			//行起始坐标高8位，始终为零
	LCD_Write_DATA8(xs);			//行起始坐标低8位
	LCD_Write_DATA8(0x00);			//行终止坐标高8位，始终为零
	LCD_Write_DATA8(xe);			//行终止坐标低8位

	LCD_Write_Command(0x2B);      	//Colulm addRSTs set
	LCD_Write_DATA8(0x00);			//列起始坐标高8位，始终为零
	LCD_Write_DATA8(ys);			//列起始坐标低8位
	LCD_Write_DATA8(0x00);			//列终止坐标高8位，始终为零
	LCD_Write_DATA8(ye);			//列终止坐标低8位

	LCD_Write_Command(0x2C);      	//GRAM接收MCU数据或命令
}
void LCD_Init(void)
{
    LCD_Port_Init();
    LCD_RST=0;
    DELAY_MS(5);
    LCD_RST=1;
    DELAY_MS(5);
   
    LCD_Write_Command(0x11);       		  	//关闭睡眠，振荡器工作
    DELAY_MS(2);
    
    LCD_Write_Command(0x3a);       		  	//每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
    LCD_Write_DATA8(0x55);						
   
    LCD_Write_Command(0x26);       		  	
    LCD_Write_DATA8(0x04);
	
    LCD_Write_Command(0xf2);              		//Driver Output Control(1)
    LCD_Write_DATA8(0x01);
    
    LCD_Write_Command(0xe0);              		//Driver Output Control(1)
    LCD_Write_DATA8(0x3f);
    LCD_Write_DATA8(0x25);
    LCD_Write_DATA8(0x1c);
    LCD_Write_DATA8(0x1e);
    LCD_Write_DATA8(0x20);
    LCD_Write_DATA8(0x12);
    LCD_Write_DATA8(0x2a);
    LCD_Write_DATA8(0x90);
    LCD_Write_DATA8(0x24);
    LCD_Write_DATA8(0x11);
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);
     
    LCD_Write_Command(0xe1);              //Driver Output Control(1)
    LCD_Write_DATA8(0x20);
    LCD_Write_DATA8(0x20);
    LCD_Write_DATA8(0x20);
    LCD_Write_DATA8(0x20);
    LCD_Write_DATA8(0x05);
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x15);
    LCD_Write_DATA8(0xa7);
    LCD_Write_DATA8(0x3d);
    LCD_Write_DATA8(0x18);
    LCD_Write_DATA8(0x25);
    LCD_Write_DATA8(0x2a);
    LCD_Write_DATA8(0x2b);
    LCD_Write_DATA8(0x2b);  
    LCD_Write_DATA8(0x3a);  
    
    LCD_Write_Command(0xb1);              	//设置屏幕刷新频率
    LCD_Write_DATA8(0x08);				   	//DIVA=8
    LCD_Write_DATA8(0x08);				   	//VPA =8，约90Hz
    				   
    LCD_Write_Command(0xb4);              	//LCD Driveing control
    LCD_Write_DATA8(0x07);				  	//NLA=1,NLB=1,NLC=1
   
   
    LCD_Write_Command(0xc0);              //LCD Driveing control
    LCD_Write_DATA8(0x0a);
    LCD_Write_DATA8(0x02);
      
    LCD_Write_Command(0xc1);              //LCD Driveing control
    LCD_Write_DATA8(0x02);

    LCD_Write_Command(0xc5);              //LCD Driveing control
    LCD_Write_DATA8(0x4f);
    LCD_Write_DATA8(0x5a);

    LCD_Write_Command(0xc7);              //LCD Driveing control
    LCD_Write_DATA8(0x40);
    
    LCD_Write_Command(0x2a);              	//配置MCU可操作的LCD内部RAM横坐标起始、结束参数
    LCD_Write_DATA8(0x00);				   	//横坐标起始地址0x0000
    LCD_Write_DATA8(0x00);					
    LCD_Write_DATA8(0x00);				   	//横坐标结束地址0x007f(127)
    LCD_Write_DATA8(0x7f);
   
    LCD_Write_Command(0x2b);              	//配置MCU可操作的LCD内部RAM纵坐标起始结束参数
    LCD_Write_DATA8(0x00);				   	//纵坐标起始地址0x0000
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);				  	//纵坐标结束地址0x009f(159)
    LCD_Write_DATA8(0x9f);

    LCD_Write_Command(0x36);              	//配置MPU和DDRAM对应关系
    LCD_Write_DATA8(0xc0);					//MX=1,MY=1

    LCD_Write_Command(0xb7);              	//LCD Driveing control
    LCD_Write_DATA8(0x00);				   	//CRL=0
	   
    LCD_Write_Command(0x29);   		  	//开启屏幕显示
    LCD_Write_Command(0x2c);   			//设置为LCD接收数据/命令模式
  
}