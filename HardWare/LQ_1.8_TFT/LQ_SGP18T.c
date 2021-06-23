/********************************************************   
��ƽ    ̨������K10/60DN/DX�๦�ܿ�����
����    д������
��Designed��by Chiu Sir
��E-mail  ��chiusir@yahoo.cn
������汾��V1.0
�������¡�2013��1��16��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��CodeWarrior IAR10,6.30
��Target  ��K60DX/DN
��Crystal ��50.000Mhz
��busclock��???.000MHz
��pllclock��100.000MHz    
***************************
------------------------------------ 
����TFT1.8ģ��ʹ��˵���� 
��Դʹ��3.3V��   
----------------
G    ��Դ��
3.3V ��3.3V��Դ
CS   PORTc12  �ѽӵأ����ý�
SCK  PORTc13
SDA  PORTc14 
A0   PORTc15
RST  PORTc16  
��5V�ĵ�Ƭ����Ҫ���ź����ϴ���1K���裬��Դ���뱣֤3.3V��
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
void LCD_Writ_Bus(char data)   //��������д��
{	
  LCD_CS=0;
#if(LCDHardWareSPI)
  LCDHardWareSPI_Send(data);
#else
  OLED_SCL=0;
  for(uint8 i=0;i<8;i++)
  {
    OLED_SDA=(data&0x80)==0x80?1:0;//�����ط�������
    OLED_SCL=1;
    OLED_SCL=0;		
    data<<=1;    
  }
#endif 
  LCD_CS=1;
} 
void LCD_Write_DATA8(char da) //��������-8λ����
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
	LCD_Write_DATA8(0x00);			//����ʼ�����8λ��ʼ��Ϊ��
	LCD_Write_DATA8(xs);			//����ʼ�����8λ
	LCD_Write_DATA8(0x00);			//����ֹ�����8λ��ʼ��Ϊ��
	LCD_Write_DATA8(xe);			//����ֹ�����8λ

	LCD_Write_Command(0x2B);      	//Colulm addRSTs set
	LCD_Write_DATA8(0x00);			//����ʼ�����8λ��ʼ��Ϊ��
	LCD_Write_DATA8(ys);			//����ʼ�����8λ
	LCD_Write_DATA8(0x00);			//����ֹ�����8λ��ʼ��Ϊ��
	LCD_Write_DATA8(ye);			//����ֹ�����8λ

	LCD_Write_Command(0x2C);      	//GRAM����MCU���ݻ�����
}
void LCD_Init(void)
{
    LCD_Port_Init();
    LCD_RST=0;
    DELAY_MS(5);
    LCD_RST=1;
    DELAY_MS(5);
   
    LCD_Write_Command(0x11);       		  	//�ر�˯�ߣ���������
    DELAY_MS(2);
    
    LCD_Write_Command(0x3a);       		  	//ÿ�δ���16λ����(VIPF3-0=0101)��ÿ������16λ(IFPF2-0=101)
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
    
    LCD_Write_Command(0xb1);              	//������Ļˢ��Ƶ��
    LCD_Write_DATA8(0x08);				   	//DIVA=8
    LCD_Write_DATA8(0x08);				   	//VPA =8��Լ90Hz
    				   
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
    
    LCD_Write_Command(0x2a);              	//����MCU�ɲ�����LCD�ڲ�RAM��������ʼ����������
    LCD_Write_DATA8(0x00);				   	//��������ʼ��ַ0x0000
    LCD_Write_DATA8(0x00);					
    LCD_Write_DATA8(0x00);				   	//�����������ַ0x007f(127)
    LCD_Write_DATA8(0x7f);
   
    LCD_Write_Command(0x2b);              	//����MCU�ɲ�����LCD�ڲ�RAM��������ʼ��������
    LCD_Write_DATA8(0x00);				   	//��������ʼ��ַ0x0000
    LCD_Write_DATA8(0x00);
    LCD_Write_DATA8(0x00);				  	//�����������ַ0x009f(159)
    LCD_Write_DATA8(0x9f);

    LCD_Write_Command(0x36);              	//����MPU��DDRAM��Ӧ��ϵ
    LCD_Write_DATA8(0xc0);					//MX=1,MY=1

    LCD_Write_Command(0xb7);              	//LCD Driveing control
    LCD_Write_DATA8(0x00);				   	//CRL=0
	   
    LCD_Write_Command(0x29);   		  	//������Ļ��ʾ
    LCD_Write_Command(0x2c);   			//����ΪLCD��������/����ģʽ
  
}