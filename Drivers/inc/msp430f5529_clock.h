#ifndef CLOCK_H_  
#define CLOCK_H_

#include "common.h"
#include "msp430f5529_system.h"

//�궨�����ʱ�ӣ�����ʱ�Ӻ���Ҫ
#define EXTAL_IN_XT1_HZ   (  32768)
#define EXTAL_IN_XT2_HZ   (4000000)
//�궨���ڲ�REFO��VLOʱ��Ƶ��
#define VLOCLK_FREQUENCY  (  10000)
#define REFOCLK_FREQUENCY (  32768)

typedef struct 
{
  struct
  {
    uint32_t nHZ;
    uint32_t nKHZ;
    float    fMHZ;
  }MCLK,SMCLK,ACLK;
  uint32_t DCO_FLL_Frequency;
}CLOCK;
extern CLOCK g_sClock; //ȫ�ֱ�����ʱ��Ƶ��
typedef enum
{
  FLLREF_XT1    =0u,
  FLLREF_REFO   =2u,
  FLLREF_XT2    =5u
}FLLREF_Source;  //FLL�ο�ʱ��Դ
typedef enum
{
  FLLREF_DIV_1     ,   //����Ƶ
  FLLREF_DIV_2     ,   //����Ƶ
  FLLREF_DIV_4     ,   //�ķ�Ƶ
  FLLREF_DIV_8     ,   //�˷�Ƶ
  FLLREF_DIV_12    ,   //ʮ����Ƶ
  FLLREF_DIV_16    ,   //ʮ����Ƶ
}FLLREF_DIVx;    //FLL�ο�ʱ�ӷ�Ƶ
typedef enum
{
  CLOCK_XT1        ,   //XT1      
  CLOCK_VLO        ,   //�ڲ�10K
  CLOCK_REFO       ,   //�ڲ�32768
  CLOCK_DCO        ,   //�ڲ�DCO,���ʹ���ڲ�DCO��Ϊʱ�ӵĻ�����Ҫ�ȵ��ã�DCO_PLLConfig����DCOƵ��
  CLOCK_DCO_DIV    ,   //DCO��Ƶ(��ƵΪ1�Ҳ������޸ģ���DCOͬƵ)
  CLOCK_XT2        ,   //XT2
}CLOCK_Source;  //ʱ��Դ
typedef enum
{
  CLOCK_DIV_1      ,   //����Ƶ
  CLOCK_DIV_2      ,   //����Ƶ
  CLOCK_DIV_4      ,   //�ķ�Ƶ
  CLOCK_DIV_8      ,   //�˷�Ƶ
  CLOCK_DIV_16     ,   //ʮ����Ƶ
  CLOCK_DIV_32     ,   //��ʮ����Ƶ
}CLOCK_DIVx;    //ʱ�ӷ�Ƶ
//����DCOʱ��Ƶ��,ע�⣺ʹ��DCO��Ϊʱ��Դ��Ƶ��ֵ��ı�
extern void CLOCK_DCO_PLLConfig(FLLREF_Source, FLLREF_DIVx, uint32_t DCO_FLL_Fre);   //DCO_FLL��Ƶ������
//����ʱ��Դ����Ƶ
extern void CLOCK_MCLK_Config (CLOCK_Source , CLOCK_DIVx);                         //��ʱ�����ã�
extern void CLOCK_SMCLK_Config(CLOCK_Source , CLOCK_DIVx);                         //��ϵͳʱ������
extern void CLOCK_ACLK_Config (CLOCK_Source , CLOCK_DIVx);                         //����ʱ������
//ʱ�����
#define MCLK_OUT()                              GPIO_Init(P7,7,GPO | SEL)       //��ʱ�����,   P7.7
#define SMCLK_OUT()                             GPIO_Init(P2,2,GPO | SEL)       //��ϵͳʱ�����P2.2
#define ACLK_OUT()                              GPIO_Init(P1,0,GPO | SEL)       //����ʱ�����  P1.0   
#define CLOCK_OUT()                             do{MCLK_OUT();SMCLK_OUT();ACLK_OUT();}while(0)  //����ʱ�����
#endif