#ifndef COMPATER_B_H_
#define COMPATER_B_H_

#include "common.h"

typedef enum
{
  CB_CH0                ,//P6.0
  CB_CH1                ,//P6.1
  CB_CH2                ,//P6.2
  CB_CH3                ,//P6.3
  CB_CH4                ,//P6.4
  CB_CH5                ,//P6.5
  CB_CH6                ,//P6.6
  CB_CH7                ,//P6.7
  CB_CH8                ,//P7.0
  CB_CH9                ,//P7.1
  CB_CH10               ,//P7.2
  CB_CH11               ,//P7.3
  CB_VREF_1_5V          ,//ʹ��1.5V��Ϊ�ο���ѹ
  CB_VREF_2_0V          ,//ʹ��2.0V��Ϊ�ο���ѹ
  CB_VREF_2_5V          ,//ʹ��2.5V��Ϊ�ο���ѹ
  CB_VREF_VCC           ,//ʹ��Vcc��Ϊ�ο���ѹ
}CompaterB_CHn;      //�Ƚ���ͨ��
typedef enum
{
  CB_RaisingIRQn         =0x01u,     //�����ش����ж�
  CB_FallingIRQn         =0x02u,     //�½��ش����ж�
}CompaterB_IRQn; //�Ƚ����жϴ�����

extern void    CompaterB_Init(CompaterB_CHn CB0, CompaterB_CHn CB1);            //�Ƚ�����ʼ����CB0Ϊ��������ͨ����CB1Ϊ��������ͨ��
extern STATUS  CompatorB_GetValue(void);                                        //��ȡ�Ƚ����ȽϽ��
extern void    CompaterB_ExchangeChanel(STATUS status);                         //�����Ƿ񽻻�����ͨ�����ԣ������жϺ���ܻᴥ��һ���ж�
extern void    CompaterB_ITConfig   (CompaterB_IRQn, STATUS ITState);           //�Ƚ����ж�����
extern STATUS  CompaterB_GetITStatus(CompaterB_IRQn);                           //��ȡ�жϱ�־״̬
extern void    CompaterB_ClearITPendingBit(CompaterB_IRQn);                     //����жϱ�־
#endif