#include "LCD.h"
#include "delay.h" 
void LCD_PORT_Init(void)
{
#if(LCDHardWareSPI)
  LCDHardWareSPI_Init();
#else
  GPIO_Init(LCD_SCL_PORT,LCD_SCL_PIN,GPO);
  GPIO_Init(LCD_SDI_PORT,LCD_SDI_PIN,GPO);
#endif
  GPIO_Init(LCD_RST_PORT,LCD_RST_PIN,GPO);
  GPIO_Init(LCD_DC_PORT,LCD_DC_PIN,GPO);
  GPIO_Init(LCD_CS_PORT, LCD_CS_PIN,GPO);
}
void LCD_Writ_Bus(char data)   //串行数据写入
{  
#if(LCDHardWareSPI)
  LCD_CS = RESET;
  LCDHardWareSPI_SendByte(data);
  LCD_CS = BIT_SET;
#else
  LCD_CS = RESET;	
  LCD_SCL = RESET;
  for(uint8_t i=0;i<8;i++)
  {
    LCD_SDI = (data&0x80) ? BIT_SET : RESET;//上升沿发送数据
    LCD_SCL = BIT_SET;
    LCD_SCL = RESET;		
    data <<= 1;    
  }
  LCD_CS = BIT_SET;
#endif 
} 
void LCD_Write_DATA8(char da) //发送数据-8位参数
{
    LCD_DC = BIT_SET;
	LCD_Writ_Bus(da);
}  
 void LCD_Write_DATA16(uint16_t da)
{
    LCD_DC = BIT_SET;
	LCD_Writ_Bus(da>>8);
	LCD_Writ_Bus(da);
}	  
void LCD_Write_REG(char da)	 
{
    LCD_DC = RESET;
        LCD_Writ_Bus(da);
}
 void LCD_Write_REG_DATA(int reg,int da)
{
    LCD_Write_REG(reg);
	LCD_Write_DATA16(da);
}
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_Write_REG(0x2a);
   LCD_Write_DATA8(x1>>8);
   LCD_Write_DATA8(x1);
   LCD_Write_DATA8(x2>>8);
   LCD_Write_DATA8(x2);
  
   LCD_Write_REG(0x2b);
   LCD_Write_DATA8(y1>>8);
   LCD_Write_DATA8(y1);
   LCD_Write_DATA8(y2>>8);
   LCD_Write_DATA8(y2);

   LCD_Write_REG(0x2C);
}
void LCD_Init(void)
{
        LCD_PORT_Init();
//调用一次这些函数，免得编译的时候提示警告
   	//LCD_CS=1;   
	LCD_RST = RESET;
	delay_ms(10);
	LCD_RST = BIT_SET;
	delay_ms(10);
	//LCD_CS =0;  //打开片选使能

	LCD_Write_REG(0xCB);  
        LCD_Write_DATA8(0x39); 
        LCD_Write_DATA8(0x2C); 
        LCD_Write_DATA8(0x00); 
        LCD_Write_DATA8(0x34); 
        LCD_Write_DATA8(0x02); 

        LCD_Write_REG(0xCF);  
        LCD_Write_DATA8(0x00); 
        LCD_Write_DATA8(0XC1); 
        LCD_Write_DATA8(0X30); 
 
        LCD_Write_REG(0xE8);  
        LCD_Write_DATA8(0x85); 
        LCD_Write_DATA8(0x00); 
        LCD_Write_DATA8(0x78); 
 
        LCD_Write_REG(0xEA);  
        LCD_Write_DATA8(0x00); 
        LCD_Write_DATA8(0x00); 
 
        LCD_Write_REG(0xED);  
        LCD_Write_DATA8(0x64); 
        LCD_Write_DATA8(0x03); 
        LCD_Write_DATA8(0X12); 
        LCD_Write_DATA8(0X81); 

        LCD_Write_REG(0xF7);  
        LCD_Write_DATA8(0x20); 
  
        LCD_Write_REG(0xC0);    //Power control 
        LCD_Write_DATA8(0x23);   //VRH[5:0] 
 
        LCD_Write_REG(0xC1);    //Power control 
        LCD_Write_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_Write_REG(0xC5);    //VCM control 
        LCD_Write_DATA8(0x3e); //对比度调节
        LCD_Write_DATA8(0x28); 
 
        LCD_Write_REG(0xC7);    //VCM control2 
        LCD_Write_DATA8(0x86);  //--
 
        LCD_Write_REG(0x36);    // Memory Access Control 
        LCD_Write_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_Write_REG(0x3A);    
        LCD_Write_DATA8(0x55); 

        LCD_Write_REG(0xB1);    
        LCD_Write_DATA8(0x00);  
        LCD_Write_DATA8(0x18); 
 
        LCD_Write_REG(0xB6);    // Display Function Control 
        LCD_Write_DATA8(0x08); 
        LCD_Write_DATA8(0x82);
        LCD_Write_DATA8(0x27);  
 
        LCD_Write_REG(0xF2);    // 3Gamma Function Disable 
        LCD_Write_DATA8(0x00); 
 
        LCD_Write_REG(0x26);    //Gamma curve selected 
        LCD_Write_DATA8(0x01); 
 
        LCD_Write_REG(0xE0);    //Set Gamma 
        LCD_Write_DATA8(0x0F); 
        LCD_Write_DATA8(0x31); 
        LCD_Write_DATA8(0x2B); 
        LCD_Write_DATA8(0x0C); 
        LCD_Write_DATA8(0x0E); 
        LCD_Write_DATA8(0x08); 
        LCD_Write_DATA8(0x4E); 
        LCD_Write_DATA8(0xF1); 
        LCD_Write_DATA8(0x37); 
        LCD_Write_DATA8(0x07); 
        LCD_Write_DATA8(0x10); 
        LCD_Write_DATA8(0x03); 
        LCD_Write_DATA8(0x0E); 
        LCD_Write_DATA8(0x09); 
        LCD_Write_DATA8(0x00); 

        LCD_Write_REG(0XE1);    //Set Gamma 
        LCD_Write_DATA8(0x00); 
        LCD_Write_DATA8(0x0E); 
        LCD_Write_DATA8(0x14); 
        LCD_Write_DATA8(0x03); 
        LCD_Write_DATA8(0x11); 
        LCD_Write_DATA8(0x07); 
        LCD_Write_DATA8(0x31); 
        LCD_Write_DATA8(0xC1); 
        LCD_Write_DATA8(0x48); 
        LCD_Write_DATA8(0x08); 
        LCD_Write_DATA8(0x0F); 
        LCD_Write_DATA8(0x0C); 
        LCD_Write_DATA8(0x31); 
        LCD_Write_DATA8(0x36); 
        LCD_Write_DATA8(0x0F); 
 
        LCD_Write_REG(0x11);    //Exit Sleep 
        delay_ms(10); 
				
        LCD_Write_REG(0x29);    //Display on 
        LCD_Write_REG(0x2c); 
}







