#ifndef SPI_H_
#define SPI_H_

#include"common.h"

typedef enum
{                      //         STE    CLK    SOMI   SIMO
  SPI0            ,    //UCA0     P3.2   P2.7   P3.4   P3.3
  SPI1            ,    //UCA1     P4.3   P4.0   P4.5   P4.4
  SPI2            ,    //UCB0     P2.7   P3.2   P3.1   P3.0 
  SPI3            ,    //UCB1     P4.0   P4.3   P4.2   P4.1 
}SPIn;     //SPIģ��
typedef enum
{
  SPI_LSB_First       ,  //�ȷ��͵�λ����
  SPI_MSB_First       ,  //�ȷ��͸�λ����
}SPI_FirstBit;       //������λѡ��
typedef enum
{
  SPI_STE_None        ,  //3��SPI
  SPI_STE_High_Enable ,  //4��SPI���ӻ����ߵ�ƽʹ������
  SPI_STE_Low_Enable  ,  //4��SPI���ӻ����͵�ƽʹ������
}SPI_STE_Mode;    //Ƭѡ�ź�ģʽ
typedef enum
{
  SPI_CLK_Mode_1      ,  //SPIģʽ1��ʱ������û�����ݴ���ʱΪ�͵�ƽ�������������ط������ݣ������½��ؽ�������
  SPI_CLK_Mode_2      ,  //SPIģʽ2��ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ط������ݣ����������ؽ�������
  SPI_CLK_Mode_3      ,  //SPIģʽ3��ʱ������û�����ݴ���ʱΪ�͵�ƽ�������������ؽ������ݣ������½��ط�������
  SPI_CLK_Mode_4      ,  //SPIģʽ4��ʱ������û�����ݴ���ʱΪ�ߵ�ƽ���������½��ؽ������ݣ����������ط�������
}SPI_CLK_Mode;   //ʱ����ģʽ
typedef enum
{
  SPI_RX_IRQn         =0x0001u, //�����ж�
  SPI_TX_IRQn         =0x0002u, //�����ж�
}SPI_IRQn;   //�ж�

extern void    SPI_Master_Init (SPIn, uint32_t BaudRate, SPI_FirstBit, SPI_STE_Mode, SPI_CLK_Mode);//SPInģ���ʼ��
extern void    SPI_SetBaudRate (SPIn, uint32_t BaudRate);                       //����SPI������
extern uint8_t SPI_SendReadByte(SPIn, uint8_t data);                            //���Ͳ���ȡһ�ֽ�����
extern void    SPI_SendReadString(SPIn, char * SendData, char * ReceiveData, uint16_t Length); //���Ͳ������ַ���

extern void    SPI_ITConfig   (SPIn, SPI_IRQn, STATUS ITState);                 //����SPI�ж��Ƿ�ʹ�� 
extern STATUS  SPI_GetITStatus(SPIn, SPI_IRQn);                                 //��ȡSPIĳһ���жϱ�־
extern void    SPI_ClearITPendingBit(SPIn, SPI_IRQn);                           //���SPIĳһ���жϱ�־
#endif