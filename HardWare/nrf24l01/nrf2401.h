#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "common.h"
#include "msp430f5529_gpio.h"

#define NRF24L01HardWareSPI     (1) //�궨���Ƿ�ʹ��Ӳ��SPI��ʹ����궨��Ϊ1����ʹ����궨��Ϊ0
#if(NRF24L01HardWareSPI)
#include "msp430f5529_spi.h"    //��Ҫ����spi.h���ļ�
//���ʹ��Ӳ��SPI����궨�����º���
//Ӳ��SPI��ʼ��������SPIģʽ3��ʱ������û�����ݴ���ʱΪ�͵�ƽ�������������ؽ������ݣ������½��ط�������
#define NRF2401HardWareSPI_Init()            SPI_Master_Init(SPI3,5MHz,SPI_MSB_First,SPI_STE_None,SPI_CLK_Mode_3)
#define NRF2401HardWareSPI_SendByte(data)    SPI_SendReadByte(SPI3,data)
#else
//���ʹ��IO��ģ��SPI������궨������IO��
#define NRF2401_SCL_PORT    P4
#define NRF2401_SCL_PIN     3

#define NRF2401_SIMO_PORT   P4
#define NRF2401_SIMO_PIN    1

#define NRF2401_SOMI_PORT   P4
#define NRF2401_SOMI_PIN    2

#define NRF2401_SCL    PIN_OUT(NRF2401_SCL_PORT,NRF2401_SCL_PIN)
#define NRF2401_SIMO   PIN_OUT(NRF2401_SIMO_PORT,NRF2401_SIMO_PIN)
#define NRF2401_SOMI   PIN_IN(NRF2401_SOMI_PORT,NRF2401_SOMI_PIN)
#endif
//���¼�������  ���Ƿ�ʹ��SPI�޹أ����Ա���궨��ʹ��
#define NRF2401_CE_PORT     P3
#define NRF2401_CE_PIN      7

#define NRF2401_CSN_PORT    P8
#define NRF2401_CSN_PIN     2
    
#define NRF2401_IRQ_PORT    P2
#define NRF2401_IRQ_PIN     2

#define NRF2401_CE     PIN_OUT(NRF2401_CE_PORT,NRF2401_CE_PIN)
#define NRF2401_CSN    PIN_OUT(NRF2401_CSN_PORT,NRF2401_CSN_PIN)
#define NRF2401_IRQ    PIN_IN(NRF2401_IRQ_PORT,NRF2401_IRQ_PIN)
    
//���س���
#define NRF_OK      (0x00)
#define NRF_ERR     (0x01)
#define MAX_TX      (0x10)     //�ﵽ����ʹ����ж�
#define TX_OK       (0x20)     //TX��������ж�
#define RX_OK       (0x40)     //���յ������ж�

//24L01���ͽ������ݿ�ȶ���
#define ADR_WIDTH       5       //�����ַ���ȣ�3~5��
#define TX_ADR_WIDTH    ADR_WIDTH   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    ADR_WIDTH   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
    
//��������ʵ�ֵĽӿں���    
typedef struct s_NRF24L01
{
  volatile uint8_t * pReceiveData;    //��������ָ��
  void (*CallBack_IRQ_Init)   (void); //�ж����ų�ʼ���ص����������ڸú���������Ϊ�������ж�,���ڸú����е���NRF2401_IRQ_Handler����
  void (*CallBack_RecevieData)(void); //���յ����ݻص�����
  void (*CallBack_SendDataOk) (void); //�������ݳɹ��ص�����
  void (*CallBack_SendOutTime)(void); //���ͳ�ʱ�ص�����
  void (*CallBack_TxFull)     (void); //���ͻ��������ص�����
}s_NRF24L01;

extern uint8_t NRF2401_Init(s_NRF24L01);                             //��ʼ��NRF24L01ģ��

extern void  NRF2401_SetRXMode(void);                              //����Ϊ����ģʽ
extern void  NRF2401_SetTXMode(void);                              //����Ϊ����ģʽ

extern void  NRF2401_SendData(uint8_t *txbuf);                     //��������
extern uint8_t NRF2401_RecData(uint8_t *rxbuf);                      //��������

extern void  NRF2401_IRQ_Handler(void);                            //NRF24L01�жϴ�����
#endif



