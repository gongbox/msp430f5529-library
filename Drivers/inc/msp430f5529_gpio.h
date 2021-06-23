#ifndef _GPIO_H_
#define _GPIO_H_

#include "common.h"

//�Ƿ�ʹ��16λIO�˿ڶ���,ʹ�������PORTA-PORTD��ע����ʱ����IO�ڲ�������������Ҫ��ȡport,pinֵ��ִ��Ч�ʻ���
//#define GPIO_PORT_SIZE_WORD

//���Ŷ�Ӧ����궨��
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

typedef enum
{
  //����д��
  P1            ,PORT1 = P1,
  P2            ,PORT2 = P2,
  P3            ,PORT3 = P3,
  P4            ,PORT4 = P4,
  P5            ,PORT5 = P5,
  P6            ,PORT6 = P6,
  P7            ,PORT7 = P7,
  P8            ,PORT8 = P8,
#ifdef GPIO_PORT_SIZE_WORD
//ע��ʵ����PORTA��P1.0-P2.7������16��IO�ڣ�PORTB��P3.0-P4.7������16��IO��,PORTC,PORTD�Դ�����
  PA            ,PORTA = PA,  //P1.0-P2.7
  PB            ,PORTB = PB,  //P3.0-P4.7
  PC            ,PORTC = PC,  //P5.0-P6.7
  PD            ,PORTD = PD,  //P7.0-P8.7
#endif
}PORTn;  //�˿�
typedef enum
{
  GPI                   =0x0001u,// ���뷽��Ĭ�ϣ�
  GPO                   =0x0002u,// �������
  
  LDS                   =0x0004u,// ������������Ĭ�ϣ�
  HDS                   =0x0008u,// ����������
  
  PULL_DOWN             =0x0010u,// ����
  PULL_UP               =0x0020u,// ����
  
  IRQ_RISING            =0x0040u,// �����ش����ж�
  IRQ_FALLING           =0x0080u,// �½��ش����ж�
  
  SEL                   =0x0100u,// �ڶ����ù���
}GPIO_Config;     //GPIOģʽ
/**************************IO�ڲ�������****************************************/
extern void      GPIO_MultiBits_Init(PORTn, uint16_t BITs, uint16_t config);    //һλ���λIO�ڹ��ܳ�ʼ��,����BITsΪBIT0,BIT1��...BIT7�����ǵĺ͵�
#define          GPIO_Init(port,pin,config)         GPIO_MultiBits_Init(port,(1u<<pin),config)

extern void      GPIO_SetBits  (PORTn, uint16_t BITs);                          //һλ���λIO������ߵ�ƽ
extern void      GPIO_ResetBits(PORTn, uint16_t BITs);                          //һλ���λIO������͵�ƽ

extern void      GPIO_WriteBit (PORTn, uint8_t pin, STATUS level);              //ĳ1λIO�������ƽ����
extern STATUS    GPIO_ReadBit  (PORTn, uint8_t pin);                            //ĳ1λIO�������ƽ��ȡ
extern void      GPIO_TurnBit  (PORTn, uint8_t pin);                            //ĳ1λIO�������ƽ��ת

extern void      GPIO_WriteByte(PORTn, uint8_t value);                          //ĳ8λIO�������ƽ����
extern uint8_t   GPIO_ReadByte (PORTn);                                         //ĳ8λIO�������ƽ��ȡ
#ifdef GPIO_PORT_SIZE_WORD
extern void      GPIO_WriteWord(PORTn, uint16_t value);                         //ĳ16λIO�������ƽ����
extern uint16_t  GPIO_ReadWord (PORTn);                                         //ĳ16λIO�������ƽ��ȡ
#endif
extern void      GPIO_ITConfig (PORTn, uint8_t pin, STATUS ITState);            //ʹ�ܻ��ֹĳ1λ����˿ڵ��ж�
extern STATUS    GPIO_GetITStatus(PORTn, uint8_t pin);                          //��ȡĳһλ�˿��жϱ�־
extern void      GPIO_ClearITPendingBit(PORTn, uint8_t pin);                    //���ĳһλ�жϱ�־

//λ�����궨�壬ʹ��PIN��Ϊ����
#include "msp430f5529_pin.h"
#define          GPIO_PinInit(pin,config)       GPIO_Init((PORTn)((pin>>4)+PORT1),(pin&0x0f),config)
#define          GPIO_PinWriteBit(pin,level)    GPIO_WriteBit((PORTn)((pin>>4)+PORT1),(pin&0x0f),level) 
#define          GPIO_PinReadBit(pin)           GPIO_ReadBit((PORTn)((pin>>4)+PORT1),(pin&0x0f)) 
#define          GPIO_PinTurnBit(pin)           GPIO_TurnBit((PORTn)((pin>>4)+PORT1),(pin&0x0f)) 
/********************************************************************************************
ʵ����51��Ƭ��һ����λ����
��PIN_OUT(P1,1)=1;���ǽ�P1.1λ����ߵ�ƽ����Ӱ��P1�˿ڵ�����λ��int value=PIN_IN(P1,0);���Ƕ�ȡP1.0���ŵ������ƽֵ��0��1��
ע��:port,pin����������P1-P8,�����Ǻ궨��,������Ϊ����,
*********************************************************************************************/
#define          PIN_DIR(port,pin)               GPIO_PIN_DIR(port,pin)         //���뷽��Ĵ���
#define          PIN_OUT(port,pin)               GPIO_PIN_OUT(port,pin)         //���״̬�Ĵ���
#define          PIN_IN(port,pin)                GPIO_PIN_IN(port,pin)          //����״̬�Ĵ���
#define          PIN_SEL(port,pin)               GPIO_PIN_SEL(port,pin)         //�ڶ����ܼĴ���
#define          PIN_REN(port,pin)               GPIO_PIN_REN(port,pin)         //����������Ĵ���
#define          PIN_IE(port,pin)                GPIO_PIN_IE(port,pin)          //�����жϼĴ���
#define          PIN_IES(port,pin)               GPIO_PIN_IES(port,pin)         //�жϴ����ؼĴ���
#define          PIN_IFG(port,pin)               GPIO_PIN_IFG(port,pin)         //�жϱ�־�Ĵ���

#endif