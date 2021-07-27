#ifndef __LCD_H
#define __LCD_H	

#include "msp430f5529_gpio.h"

#define LCDHardWareSPI     (1) //�궨���Ƿ�ʹ��Ӳ��SPI��ʹ����궨��Ϊ1����ʹ����궨��Ϊ0
#if(LCDHardWareSPI)
#include "msp430f5529_spi.h"    //��Ҫ����spi.h���ļ�
//���ʹ��Ӳ��SPI����궨�����º���
//Ӳ��SPI��ʼ��������ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ؽ������ݣ����������ط�������
//������̫��Ҳ����ʾ���룬��֪��Ϊʲô
#define LCDHardWareSPI_Init()            SPI_Master_Init(SPI2,10MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_4)
#define LCDHardWareSPI_SendByte(data)    SPI_SendReadByte(SPI2,data)
#else
//���ʹ��IO��ģ��SPI������궨������IO��
#define LCD_SCL_PORT   P3
#define LCD_SCL_PIN    2

#define LCD_SDI_PORT   P3
#define LCD_SDI_PIN    0

#define LCD_SCL    PIN_OUT(LCD_SCL_PORT,LCD_SCL_PIN)
#define LCD_SDI    PIN_OUT(LCD_SDI_PORT,LCD_SDI_PIN)
#endif
//���¼�������  ���Ƿ�ʹ��SPI�޹أ����Ա���궨��ʹ��
#define LCD_CS_PORT    P2
#define LCD_CS_PIN     7

#define LCD_RST_PORT   P2
#define LCD_RST_PIN    4

#define LCD_DC_PORT    P2
#define LCD_DC_PIN     0

#define LCD_CS     PIN_OUT(LCD_CS_PORT,LCD_CS_PIN)
#define LCD_RST    PIN_OUT(LCD_RST_PORT,LCD_RST_PIN)
#define LCD_DC     PIN_OUT(LCD_DC_PORT,LCD_DC_PIN)

void LCD_Init(void); 
void LCD_SetPos(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Write_DATA8(char da);
void LCD_Write_DATA16(uint16_t da);
void LCD_Write_REG(char da);
					  		 
#endif  
	 
	 



