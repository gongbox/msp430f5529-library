#include "msp430f5529_clock.h"

#ifndef EXTAL_IN_XT1_HZ
#error û�к궨��"XT1_nHZ",����clock.h��궨�壬��#define EXTAL_IN_XT1_HZ  32768HZ
#endif  
#ifndef EXTAL_IN_XT2_HZ
#error û�к궨��"XT2_nHZ",����clock.h��궨�壬��#define EXTAL_IN_XT2_HZ  4000000HZ
#endif

const UCS_MemMapPtr UCS = UCS_BASE_PTR;
__no_init CLOCK g_sClock;                                        //ȫ�ֱ�����ʱ��Ƶ��
static void SetVcoreUp (uint8_t level);                          //�����ں˵�ѹ
/*******************************************************************************
*  �������ƣ�XT1_Config(STATUS status)
*  ����˵���������Ƿ�ʹ��XT1
*  ����˵����STATUS status���Ƿ�ʹ��XT1
*  �������أ���
*  ʹ��ʾ����XT1_Config(ENABLE);   //ʹ��XT1
********************************************************************************/
static inline void XT1_Config(STATUS status)
{
  if(status != DISABLE)
  {
    if(UCS->XT1_OFF == BIT_SET)
    {
      GPIO_MultiBits_Init(P5,(GPIO_Pin_4|GPIO_Pin_5),GPI|SEL);   //ѡ�� XT1 �ڶ�����
      UCS->XT1_OFF = RESET ;                          // ���� XT1                       
      do
      {
        UCS->XT1_LFOFFG = RESET;                     // ���XT1,CLOCK_DCO ʧЧ��־                     
        SFRIFG1 &= ~OFIFG;                           // 
      }while (UCS->XT1_LFOFFG == BIT_SET);               //
    }
  }
  else
  {
    UCS->XT1_OFF = BIT_SET ;                          // �ر� XT1                       
  }
}
/*******************************************************************************
*  �������ƣ�XT2_Config(STATUS status)
*  ����˵���������Ƿ�ʹ��XT2
*  ����˵����STATUS status���Ƿ�ʹ��XT2
*  �������أ���
*  ʹ��ʾ����XT2_Config(TRUE);   //ʹ��XT2
********************************************************************************/
static inline void XT2_Config(STATUS status)
{
  if(status != DISABLE)
  {
    if(UCS->XT2_OFF == BIT_SET)
    {
      GPIO_MultiBits_Init(P5,(GPIO_Pin_2|GPIO_Pin_3),GPI|SEL);      //ѡ�� CLOCK_XT2 �ڶ�����
      UCS->XT2_OFF = RESET;                         //���� CLOCK_XT2                      
      do
      {
        UCS->XT2_OFFG = RESET;
        SFRIFG1 &= ~OFIFG;                       
      }while (UCS->XT2_OFFG == BIT_SET);
    }
  }
  else
  {
    UCS->XT2_OFF = BIT_SET ;                          // �ر�XT2                       
  }
}
/*******************************************************************************
*  �������ƣ�CLOCK_DCO_PLLConfig     (FLLREF_Source refsource, FLLREF_DIVx refdiv, uint32_t DCO_FLL_Fre)
*  ����˵��������DCOƵ�ʣ���λ��HZ��
*  ����˵����FLLREF_Source refsource :�ο�ʱ��Դ
             FLLREF_DIVx refdiv      :�ο�ʱ��Դ��Ƶϵ��
             uint32_t DCO_FLL_Fre      :DCO����Ƶ��
*  �������أ���
*  ʹ��ʾ����CLOCK_DCO_PLLConfig     (FLLREF_REFO, FLLREF_DIV_1, 16MHZ);  //����DCO��Ƶ����REFO��һ��Ƶ��Ϊ�ο�ʱ��Դ����Ƶ��16MHZ
********************************************************************************/
void CLOCK_DCO_PLLConfig     (FLLREF_Source refsource, FLLREF_DIVx refdiv, uint32_t DCO_FLL_Fre)
{
  static const uint16_t ref_div_value[6]={1,2,4,8,12,16};
  /*����Ƶ������ں˵�ѹ*/
  //SetVcoreUp ( (DCO_FLL_Fre < 12MHz) ? 0 : ((DCO_FLL_Fre < 16MHz) ? 1 : ((DCO_FLL_Fre < 20MHz) ? 2 :3))); //�����ں˵�ѹ
  if(DCO_FLL_Fre < 12MHz)
    SetVcoreUp (0x00);                      //�����ں˵�ѹ
  else if(DCO_FLL_Fre < 16MHz)
    SetVcoreUp (0x01);       
  else if(DCO_FLL_Fre < 20MHz)
    SetVcoreUp (0x02);       
  else
    SetVcoreUp (0x03);       
  
  __bis_SR_register(SCG0);                                              // ��ֹ��Ƶ��FLL,
  UCS->CTL0 = 0x0000;                                                   // ���� DCOx, MODx
  
  if (DCO_FLL_Fre < 0.63MHz)         //           fsystem < 0.63MHz
	UCS->DCORSEL = 0;
  else if (DCO_FLL_Fre < 1.25MHz)    // 0.63MHz < fsystem < 1.25MHz
	UCS->DCORSEL = 1;
  else if (DCO_FLL_Fre < 2.5MHz)     // 1.25MHz < fsystem <  2.5MHz
	UCS->DCORSEL = 2;
  else if (DCO_FLL_Fre <   5MHz)     // 2.5MHz  < fsystem <    5MHz
	UCS->DCORSEL = 3;
  else if (DCO_FLL_Fre <  10MHz)     // 5MHz    < fsystem <   10MHz
	UCS->DCORSEL = 4;
  else if (DCO_FLL_Fre <  20MHz)     // 10MHz   < fsystem <   20MHz
	UCS->DCORSEL = 5;
  else if (DCO_FLL_Fre <  40MHz)     // 20MHz   < fsystem <   40MHz
	UCS->DCORSEL = 6;
  else
	UCS->DCORSEL = 7;
  
  UCS->FLLREFDIV = refdiv;
  UCS->SELREF = refsource;
  
  float Fref_value;
  if(refsource == FLLREF_XT2)
  {
    XT2_Config(TRUE);
    Fref_value = (float)((uint32_t)EXTAL_IN_XT2_HZ/ref_div_value[refdiv]);  
  }
  else if(refsource == FLLREF_XT1)
  {
    XT1_Config(TRUE);
    Fref_value = (float)(EXTAL_IN_XT1_HZ/ref_div_value[refdiv]);
  }
  else if(refsource == FLLREF_REFO)
  {
    Fref_value = (float)(REFOCLK_FREQUENCY/ref_div_value[refdiv]);
  }
  uint16_t FLLN_VALUE = (uint16_t)((DCO_FLL_Fre/Fref_value+0.5f)-1u);
  ASSERT(FLLN_VALUE < 1024,"CLOCK_DCO_PLLConfig","FLLN_VALUE��������1023���뽫DCOƵ����ͻ��߸���Ϊ����Ƶ�ʵĲο�ʱ��Դ��");         //��������1023���뽫DCOƵ����ͻ��߸���Ϊ����Ƶ�ʵĲο�ʱ��Դ
  
  g_sClock.DCO_FLL_Frequency = DCO_FLL_Fre;
  
  UCS->FLLN = FLLN_VALUE;        //
  UCS->FLLD = 0;     //����DCO��Ƶ  
  
  __bic_SR_register(SCG0);                  // ʹ��FLL
  do
  {
    UCS->DCO_FFG = RESET; // ���,CLOCK_DCO ʧЧ��־                                        
    SFRIFG1 &= ~OFIFG;                                             // ���ʱ��ʧЧ��־
  }while (UCS->DCO_FFG == BIT_SET);                                           // ���DCOʧЧ��־
 
  //��ʹ��DCO��Ϊʱ��Դ��ʱ��Ƶ��ֵ�޸�
  if(UCS->SELM == CLOCK_DCO || UCS->SELM == CLOCK_DCO_DIV) 
  {
    CLOCK_DIVx div = (CLOCK_DIVx)UCS->DIVM;
    CLOCK_MCLK_Config ((CLOCK_Source)UCS->SELM, div);
  }
  
  if(UCS->SELS == CLOCK_DCO || UCS->SELS == CLOCK_DCO_DIV)
  {
    CLOCK_DIVx div = (CLOCK_DIVx)UCS->DIVS;
    CLOCK_SMCLK_Config((CLOCK_Source)UCS->SELS, div);
  }
  
  if(UCS->SELA == CLOCK_DCO || UCS->SELA == CLOCK_DCO_DIV)
  {
    CLOCK_DIVx div = (CLOCK_DIVx)UCS->DIVA;
    CLOCK_ACLK_Config ((CLOCK_Source)UCS->SELS, div);
  }
}
/*************************************************************************
*  �������ƣ�CLOCK_MCLK_Config  (CLOCK_Source mclk , CLOCK_DIVx mclk_div)
*  ����˵����������ʱ��Դ����Ƶ
*  ����˵����CLOCK_Source mclk   :��ʱ��ʱ��Դ
             CLOCK_DIVx mclk_div :��ʱ�ӷ�Ƶϵ��
*  �������أ���
*  ʹ��ʾ����CLOCK_MCLK_Config  (CLOCK_DCO , DIV_1);   //��ʱ��ʹ��DCO��Ϊʱ��Դ����Ƶϵ��Ϊ1������Ƶ��
*************************************************************************/  
void CLOCK_MCLK_Config  (CLOCK_Source mclk , CLOCK_DIVx mclk_div)
{
  if(mclk == CLOCK_XT2)
  {
    XT2_Config(TRUE);
  }
  else if(mclk == CLOCK_XT1)
  {
    XT1_Config(TRUE);
  }
  
  UCS->SELM = mclk;                         //ѡ��DCO��Ϊʱ��Դ
  UCS->DIVM = mclk_div;
  
  switch(mclk)
  {
  case CLOCK_XT1    :g_sClock.MCLK.nHZ = EXTAL_IN_XT1_HZ;break;
  case CLOCK_VLO    :g_sClock.MCLK.nHZ = VLOCLK_FREQUENCY;break;
  case CLOCK_REFO   :g_sClock.MCLK.nHZ = REFOCLK_FREQUENCY;break;       
  case CLOCK_DCO    :g_sClock.MCLK.nHZ = g_sClock.DCO_FLL_Frequency;break;    
  case CLOCK_DCO_DIV:g_sClock.MCLK.nHZ = g_sClock.DCO_FLL_Frequency;break;
  case CLOCK_XT2    :g_sClock.MCLK.nHZ = EXTAL_IN_XT2_HZ;break;
  default :return;
  }
  g_sClock.MCLK.nHZ  >>= mclk_div;
  g_sClock.MCLK.nKHZ = g_sClock.MCLK.nHZ/1000u;
  g_sClock.MCLK.fMHZ = g_sClock.MCLK.nHZ/1000000.0;
}
/*************************************************************************
*  �������ƣ�CLOCK_SMCLK_Config (CLOCK_Source smclk, CLOCK_DIVx smclk_div)
*  ����˵��������ϵͳʱ��Դ����Ƶ
*  ����˵����CLOCK_Source smclk   :ϵͳʱ��ʱ��Դ
             CLOCK_DIVx smclk_div :ϵͳʱ�ӷ�Ƶϵ��
*  �������أ���
*  ʹ��ʾ����CLOCK_SMCLK_Config  (CLOCK_DCO , DIV_2);   //ϵͳʱ��ʹ��DCO��Ϊʱ��Դ����Ƶϵ��Ϊ2������Ƶ��
*************************************************************************/  
void CLOCK_SMCLK_Config (CLOCK_Source smclk, CLOCK_DIVx smclk_div)
{
  if(smclk == CLOCK_XT2)
  {
    XT2_Config(TRUE);
  }
  else if(smclk == CLOCK_XT1)
  {
    XT1_Config(TRUE);
  }
  
  UCS->SELS = smclk;//ѡ��smclkʱ��Դ
  UCS->DIVS = smclk_div;
  
  switch(smclk)
  {
  case CLOCK_XT1    :g_sClock.SMCLK.nHZ = EXTAL_IN_XT1_HZ;break;
  case CLOCK_VLO    :g_sClock.SMCLK.nHZ = VLOCLK_FREQUENCY;break;
  case CLOCK_REFO   :g_sClock.SMCLK.nHZ = REFOCLK_FREQUENCY;break;       
  case CLOCK_DCO    :g_sClock.SMCLK.nHZ = g_sClock.DCO_FLL_Frequency;break; 
  case CLOCK_DCO_DIV:g_sClock.SMCLK.nHZ = g_sClock.DCO_FLL_Frequency;break;
  case CLOCK_XT2    :g_sClock.SMCLK.nHZ = EXTAL_IN_XT2_HZ;break;
  default :return;
  }
  g_sClock.SMCLK.nHZ >>= smclk_div;
  g_sClock.SMCLK.nKHZ = g_sClock.SMCLK.nHZ/1000u;;
  g_sClock.SMCLK.fMHZ = g_sClock.SMCLK.nHZ/1000000.0;
}
/*************************************************************************
*  �������ƣ�CLOCK_ACLK_Config  (CLOCK_Source aclk , CLOCK_DIVx aclk_div)
*  ����˵�������ø���ʱ��Դ����Ƶ
*  ����˵����CLOCK_Source aclk   :����ʱ��ʱ��Դ
             CLOCK_DIVx aclk_div :����ʱ�ӷ�Ƶϵ��
*  �������أ���
*  ʹ��ʾ����CLOCK_ACLK_Config  (XT! , DIV_4);   //����ʱ��ʹ��XT1��Ϊʱ��Դ����Ƶϵ��Ϊ4���ķ�Ƶ��
*************************************************************************/  
void CLOCK_ACLK_Config  (CLOCK_Source aclk , CLOCK_DIVx aclk_div)
{
   if(aclk == CLOCK_XT2)
  {
    XT2_Config(TRUE);
  }
  else if(aclk == CLOCK_XT1)
  {
    XT1_Config(TRUE);
  }
  
  UCS->SELA = aclk;//ѡ��ʱ��Դ                   
  UCS->DIVA = aclk_div; //���÷�Ƶϵ��Ϊ0
  
  switch(aclk)
  {
  case CLOCK_XT1    :g_sClock.ACLK.nHZ = EXTAL_IN_XT1_HZ;break;
  case CLOCK_VLO    :g_sClock.ACLK.nHZ = VLOCLK_FREQUENCY;break;
  case CLOCK_REFO   :g_sClock.ACLK.nHZ = REFOCLK_FREQUENCY;break;       
  case CLOCK_DCO    :g_sClock.ACLK.nHZ = g_sClock.DCO_FLL_Frequency;break;   
  case CLOCK_DCO_DIV:g_sClock.ACLK.nHZ = g_sClock.DCO_FLL_Frequency;break;
  case CLOCK_XT2    :g_sClock.ACLK.nHZ = EXTAL_IN_XT2_HZ;break;
  default :return;
  }
  g_sClock.ACLK.nHZ >>= aclk_div;
  g_sClock.ACLK.nKHZ = g_sClock.ACLK.nHZ/1000u;;
  g_sClock.ACLK.fMHZ = g_sClock.ACLK.nHZ/1000000.0;
}
/*******************************************************************************
�������ܣ������ں˵�ѹֵ����Ƶ�������йأ�
����������u8 level ����ѹ���� С��3
********************************************************************************/
void SetVcoreUp (uint8_t level)
{
#if 1  //����ʱ�޸�Ϊ0������Ῠ����ѭ����
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;              
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  uint16_t i=50000; 
  while (((PMMIFG & SVSMLDLYIFG) == 0)&&((i--)>0));
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  i =50000;
  if ((PMMIFG & SVMLIFG))
    while(((PMMIFG & SVMLVLRIFG) == 0)&&((i--)>0));
  /*
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  */
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
#endif
}
