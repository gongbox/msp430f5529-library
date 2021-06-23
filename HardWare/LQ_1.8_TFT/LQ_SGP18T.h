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
��dev.env.��CodeWarrior IAR6.10,6.30
��Target  ��K60DX/DN
��Crystal ��50.000Mhz
��busclock��???.000MHz
��pllclock��100.000MHz    
***************************
------------------------------------
  ʹ��˵���� 
OLED��Դʹ��3.3V��   
----------------
G    ��Դ��
3.3V ��3.3V��Դ
CS   PORTc12  
SCK  PORTc13
SDA  PORTc14 
A0   PORTc15
RST  PORTc16  �ѽӵأ����ý�
��5V�ĵ�Ƭ����Ҫ���ź����ϴ���1K���裬��Դ���뱣֤3.3V��
********************************************************/

#ifndef __SGP18T_LCD_BASE_H__
#define __SGP18T_LCD_BASE_H__

#include "gpio.h"
#include "spi.h"

#define LCDHardWareSPI     (1) //�궨���Ƿ�ʹ��Ӳ��SPI��ʹ����궨��Ϊ1����ʹ����궨��Ϊ0
#if(LCDHardWareSPI)
//���ʹ��Ӳ��SPI����궨�����º���
//Ӳ��SPI��ʼ��������ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ؽ������ݣ����������ط�������
//�����ʲ��Կ��Դﵽ10M,������̫��Ҳ����ʾ���룬��֪��Ϊʲô
#define LCDHardWareSPI_Init()        SPI_Master_Init(SPI0,10000000,MSB_First,STE_None,SPI_Mode_4)
#define LCDHardWareSPI_Send(data)    SPI_SendReadByte(SPI0,data)
#else
//���ʹ��IO��ģ��SPI������궨������IO��

#define LCD_SCL_Port   P2
#define LCD_SDI_Port   P3

#define LCD_SCL_Pin    7
#define LCD_SDI_Pin    3

#define LCD_SCL    PIN_OUT(LCD_SCL_Port,LCD_SCL_Pin)
#define LCD_SDI    PIN_OUT(LCD_SDI_Port,LCD_SDI_Pin)
#endif
//���¼�������  ���Ƿ�ʹ��SPI�޹أ����Ա���궨��ʹ��
#define LCD_CS_Port    P3
#define LCD_RST_Port   P2
#define LCD_DC_Port    P2

#define LCD_CS_Pin     2
#define LCD_RST_Pin    4
#define LCD_DC_Pin     0

#define LCD_CS     PIN_OUT(LCD_CS_Port,LCD_CS_Pin)
#define LCD_RST    PIN_OUT(LCD_RST_Port,LCD_RST_Pin)
#define LCD_DC     PIN_OUT(LCD_DC_Port,LCD_DC_Pin)

extern  uint16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void LCD_Init(void); 
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA16(uint16 da);
void LCD_WR_REG(char da);



#endif /*SGP18T_ILI9163B.h*/