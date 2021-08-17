#ifndef _OLED_H_
#define _OLED_H_

#include "msp430f5529_gpio.h"

#define OLEDHardWareSPI     (0) //�궨���Ƿ�ʹ��Ӳ��SPI��ʹ����궨��Ϊ1����ʹ����궨��Ϊ0
#if(OLEDHardWareSPI)
#include "msp430f5529_spi.h"    //��Ҫ����spi.h���ļ�
//���ʹ��Ӳ��SPI����궨�����º���
//Ӳ��SPI��ʼ��������ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ؽ������ݣ����������ط�������
//������̫��Ҳ����ʾ���룬��֪��Ϊʲô
#define OLEDHardWareSPI_Init()            SPI_Master_Init(SPI3,30MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_4)
#define OLEDHardWareSPI_SendByte(data)    SPI_SendReadByte(SPI3,data)
#else
//���ʹ��IO��ģ��SPI������궨������IO��
#define OLED_SCL_PORT   P4
#define OLED_SCL_PIN    3

#define OLED_SDA_PORT   P4
#define OLED_SDA_PIN    0

#define OLED_SCL    PIN_OUT(OLED_SCL_PORT,OLED_SCL_PIN)
#define OLED_SDA    PIN_OUT(OLED_SDA_PORT,OLED_SDA_PIN)
#endif
//���¼�������  ���Ƿ�ʹ��SPI�޹أ����Ա���궨��ʹ��
#define OLED_RST_PORT   P3
#define OLED_RST_PIN    7

#define OLED_DC_PORT    P8
#define OLED_DC_PIN     2

#define OLED_RST    PIN_OUT(OLED_RST_PORT,OLED_RST_PIN)
#define OLED_DC     PIN_OUT(OLED_DC_PORT,OLED_DC_PIN)

#define OLED_HEIGHT          64                 //��
#define OLED_WIDTH          128                 //��

#define OLED_RAM                //�궨���Ƿ��������Դ棬������1K��RAM������ֻ�п��������ʹ�û��㺯��

typedef enum
{
    FONT_ASCII_6X8, FONT_ASCII_8X16, FONT_CHINESE_16X16,
} OLED_FONT; //OLED����

void OLED_Init(void);                                                      //��ʼ��
void OLED_Clear(void);                                                      //����
#ifdef OLED_RAM //�����Դ�
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t value);                   //����
void OLED_FillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t data);   //����������
#endif
void OLED_FillPart(uint8_t x0, uint8_t line0, uint8_t x1, uint8_t line1, uint8_t data);     //����������
void OLED_DispChar(OLED_FONT font, uint8_t line, uint8_t x, char ascii); //��ʾһ���ַ�        
void OLED_DispStringAt(OLED_FONT font, uint8_t line, uint8_t x, char *str); //��ָ��λ����ʾһ��Ӣ���ַ���        
void OLED_DispStringAtCEOL(OLED_FONT font, uint8_t line, uint8_t x, char *str); //��ָ��λ����ʾһ��Ӣ���ַ����������ĩβ 
void OLED_DispStringLenAt(OLED_FONT font, uint8_t line, uint8_t x, char *str, uint8_t len); //��ָ��λ����ʾһ��ָ���ַ����ȵ�Ӣ���ַ���
void OLED_DispDecAt(OLED_FONT font, uint8_t line, uint8_t x, int32_t number, uint8_t len);  //��ָ��λ����ʾһ����������
void OLED_DispFolatAt(OLED_FONT font, uint8_t line, uint8_t x, float number, uint8_t len);  //��ָ��λ����ʾһ��������
void OLED_PrintfAt(OLED_FONT font, uint8_t line, uint8_t x, const char *fmt, ...);         //��ָ��λ�ô�ӡ
void OLED_Printf(OLED_FONT font, const char *fmt, ...);                    //��ӡ 
void OLED_DispChineseAt(OLED_FONT font, uint8_t line, uint8_t x, char *str); //��ָ��λ����ʾһ�������ַ��� 

#endif
