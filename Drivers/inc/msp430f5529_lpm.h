#ifndef LPM_H_
#define LPM_H_

#include "common.h"
#include "msp430f5529.h"
#include "msp430f5529_MemMap.h"

//�͹���ģʽ
typedef enum
{
  LPM_AM  =(0x0u),                                 //�ģʽ   ��CPU���MCLK���SMCLK���ACLK�
  LPM_0   =(CPUOFF),                               //�͹���ģʽ0��CPU��ֹ��MCLK��ֹ��SMCLK���ACLK�
  LPM_1   =(SCG0+CPUOFF),                          //�͹���ģʽ1��CPU��ֹ��MCLK��ֹ��SMCLK���ACLK�
  LPM_2   =(SCG1+CPUOFF),                          //�͹���ģʽ2��CPU��ֹ��MCLK��ֹ��SMCLK��ֹ��ACLK�
  LPM_3   =(SCG1+SCG0+CPUOFF),                     //�͹���ģʽ3��CPU��ֹ��MCLK��ֹ��SMCLK��ֹ��ACLK�
  LPM_4   =(SCG1+SCG0+OSCOFF+CPUOFF),              //�͹���ģʽ4��CPU��ֹ��MCLK��ֹ��SMCLK��ֹ��ACLK��ֹ
}LPMx;

extern void LowPowerMode_Enter(LPMx);                  //�͹���ģʽ���ã�ע�ⲻ�����ж���ʹ��
#define     LowPowerMode_Exit()    LPM4_EXIT           //�жϺ��˳��͹���ģʽ,ע��һ��Ҫ���ж���ʹ��

#endif