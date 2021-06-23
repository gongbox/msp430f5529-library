#ifndef __LCD_H
#define __LCD_H	

#include "gpio.h"
#include "spi.h"

#define LCDHardWareSPI     (1) //�궨���Ƿ�ʹ��Ӳ��SPI��ʹ����궨��Ϊ1����ʹ����궨��Ϊ0
#if(LCDHardWareSPI)
//���ʹ��Ӳ��SPI����궨�����º���
//Ӳ��SPI��ʼ��������ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ؽ������ݣ����������ط�������
//�����ʲ��Կ��Դﵽ10M,������̫��Ҳ����ʾ���룬��֪��Ϊʲô
#define LCDHardWareSPI_Init()        SPI_Master_Init(SPI2,10000000,MSB_First,STE_None,SPI_Mode_4)
#define LCDHardWareSPI_Send(data)    SPI_SendReadByte(SPI2,data)
#else
//���ʹ��IO��ģ��SPI������궨������IO��

#define LCD_SCL_Port   P3
#define LCD_SDI_Port   P3

#define LCD_SCL_Pin    2
#define LCD_SDI_Pin    0

#define LCD_SCL    PIN_OUT(LCD_SCL_Port,LCD_SCL_Pin)
#define LCD_SDI    PIN_OUT(LCD_SDI_Port,LCD_SDI_Pin)
#endif
//���¼�������  ���Ƿ�ʹ��SPI�޹أ����Ա���궨��ʹ��
#define LCD_CS_Port    P2
#define LCD_RST_Port   P2
#define LCD_DC_Port    P2

#define LCD_CS_Pin     7
#define LCD_RST_Pin    4
#define LCD_DC_Pin     0

#define LCD_CS     PIN_OUT(LCD_CS_Port,LCD_CS_Pin)
#define LCD_RST    PIN_OUT(LCD_RST_Port,LCD_RST_Pin)
#define LCD_DC     PIN_OUT(LCD_DC_Port,LCD_DC_Pin)

extern  uint16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void LCD_Init(void); 
void LCD_Clear(uint16 Color);
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Write_DATA8(char da);
void LCD_Write_DATA16(uint16 da);
void LCD_Write_REG(char da);
					  		 
#endif  
	 
	 



