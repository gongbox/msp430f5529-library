#ifndef ADC_H_
#define ADC_H_

#include"common.h"  

//�궨��ADC12MEM8 to ADC12MEM15�����뱣��ʱ��  0-15
#define M8_M15_SAMPLE_HOLD_TIME  3
//�궨��ADC12MEM0 to ADC12MEM7 �����뱣��ʱ��  0-15
#define M0_M7_SAMPLE_HOLD_TIME   3

typedef enum
{
  ADC_8bit              ,  //8 λ����,���ֵ256-1
  ADC_10bit             ,  //10λ����,���ֵ1024-1
  ADC_12bit             ,  //12λ����,���ֵ4096-1
}ADC_Resolution;   //����
typedef enum
{
  ADC_VREF_1_5V         ,  //�ڲ�1.5V
  ADC_VREF_2_5V         ,  //�ڲ�2.5V
  ADC_VREF_3_3V         ,  //�����ѹ��Ϊ�ο���ѹ
  ADC_VREF_External     ,  //ʹ���ⲿ�ṩ�ο���ѹ
}ADC_VREF;         //�ο���ѹ
typedef enum
{
  ADC_CH0        =BIT0,  //P6.0
  ADC_CH1        =BIT1,  //P6.1
  ADC_CH2        =BIT2,  //P6.2
  ADC_CH3        =BIT3,  //P6.3
  ADC_CH4        =BIT4,  //P6.4
  ADC_CH5        =BIT5,  //P6.5
  ADC_CH6        =BIT6,  //P6.6
  ADC_CH7        =BIT7,  //P6.7
  ADC_CH8        =BIT8,  //VeREF+    //�ⲿ����׼
  ADC_CH9        =BIT9,  //VeREF-    //�ⲿ����׼
  ADC_CH10       =BITA,  //Ƭ���¶ȴ�����
  ADC_CH11       =BITB,  //(AVCC-AVSS)/2
  ADC_CH12       =BITC,  //P7.0
  ADC_CH13       =BITD,  //P7.1
  ADC_CH14       =BITE,  //P7.2
  ADC_CH15       =BITF,  //P7.3 
  ADC_CH_NUM     =16u,   //ͨ������
}ADC_CHn;       //ͨ��

extern void     ADC_Init(uint16_t ch, ADC_VREF, ADC_Resolution);                //��ʼ��һ������ͨ��
extern void     ADC_DeInit(ADC_CHn);                                            //��λADCͨ����ȡ����ͨ��ADC����
extern void     ADC_StartConv(void);                                            //����һ��ת��
extern void     ADC_WaitBusy(void);                                             //�ȴ�ADC��æ
extern uint16_t ADC_ReadChanelValue (ADC_CHn);                                  //��ȡADC���
extern uint16_t ADC_ReadChanelOnce  (ADC_CHn);                                  //תתһ�β���ȡADC���
extern void     ADC_ITConfig   (ADC_CHn, STATUS ITState);                       //ADC�ж�����
extern STATUS   ADC_GetITStatus(ADC_CHn);                                       //��ȡĳһͨ���жϱ�־״̬
extern void     ADC_ClearITPendingBit(ADC_CHn);                                 //���ĳһͨ���жϱ�־λ
extern void *   ADC_GetChanelMemoryAddr(ADC_CHn);                               //��ȡĳһͨ������Ĵ�����ַ

#endif