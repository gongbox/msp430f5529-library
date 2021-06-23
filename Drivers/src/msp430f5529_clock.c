#include "msp430f5529_clock.h"

#ifndef EXTAL_IN_XT1_HZ
#error 没有宏定义"XT1_nHZ",请在clock.h里宏定义，如#define EXTAL_IN_XT1_HZ  32768HZ
#endif  
#ifndef EXTAL_IN_XT2_HZ
#error 没有宏定义"XT2_nHZ",请在clock.h里宏定义，如#define EXTAL_IN_XT2_HZ  4000000HZ
#endif

const UCS_MemMapPtr UCS = UCS_BASE_PTR;
__no_init CLOCK g_sClock;                                        //全局变量，时钟频率
static void SetVcoreUp (uint8_t level);                          //设置内核电压
/*******************************************************************************
*  函数名称：XT1_Config(STATUS status)
*  功能说明：设置是否使能XT1
*  参数说明：STATUS status：是否使能XT1
*  函数返回：无
*  使用示例：XT1_Config(ENABLE);   //使能XT1
********************************************************************************/
static inline void XT1_Config(STATUS status)
{
  if(status != DISABLE)
  {
    if(UCS->XT1_OFF == BIT_SET)
    {
      GPIO_MultiBits_Init(P5,(GPIO_Pin_4|GPIO_Pin_5),GPI|SEL);   //选用 XT1 第二功能
      UCS->XT1_OFF = RESET ;                          // 开启 XT1                       
      do
      {
        UCS->XT1_LFOFFG = RESET;                     // 清除XT1,CLOCK_DCO 失效标志                     
        SFRIFG1 &= ~OFIFG;                           // 
      }while (UCS->XT1_LFOFFG == BIT_SET);               //
    }
  }
  else
  {
    UCS->XT1_OFF = BIT_SET ;                          // 关闭 XT1                       
  }
}
/*******************************************************************************
*  函数名称：XT2_Config(STATUS status)
*  功能说明：设置是否使能XT2
*  参数说明：STATUS status：是否使能XT2
*  函数返回：无
*  使用示例：XT2_Config(TRUE);   //使能XT2
********************************************************************************/
static inline void XT2_Config(STATUS status)
{
  if(status != DISABLE)
  {
    if(UCS->XT2_OFF == BIT_SET)
    {
      GPIO_MultiBits_Init(P5,(GPIO_Pin_2|GPIO_Pin_3),GPI|SEL);      //选用 CLOCK_XT2 第二功能
      UCS->XT2_OFF = RESET;                         //开启 CLOCK_XT2                      
      do
      {
        UCS->XT2_OFFG = RESET;
        SFRIFG1 &= ~OFIFG;                       
      }while (UCS->XT2_OFFG == BIT_SET);
    }
  }
  else
  {
    UCS->XT2_OFF = BIT_SET ;                          // 关闭XT2                       
  }
}
/*******************************************************************************
*  函数名称：CLOCK_DCO_PLLConfig     (FLLREF_Source refsource, FLLREF_DIVx refdiv, uint32_t DCO_FLL_Fre)
*  功能说明：设置DCO频率，单位（HZ）
*  参数说明：FLLREF_Source refsource :参考时钟源
             FLLREF_DIVx refdiv      :参考时钟源分频系数
             uint32_t DCO_FLL_Fre      :DCO设置频率
*  函数返回：无
*  使用示例：CLOCK_DCO_PLLConfig     (FLLREF_REFO, FLLREF_DIV_1, 16MHZ);  //设置DCO倍频环以REFO的一分频作为参考时钟源，倍频到16MHZ
********************************************************************************/
void CLOCK_DCO_PLLConfig     (FLLREF_Source refsource, FLLREF_DIVx refdiv, uint32_t DCO_FLL_Fre)
{
  static const uint16_t ref_div_value[6]={1,2,4,8,12,16};
  /*根据频率提高内核电压*/
  //SetVcoreUp ( (DCO_FLL_Fre < 12MHz) ? 0 : ((DCO_FLL_Fre < 16MHz) ? 1 : ((DCO_FLL_Fre < 20MHz) ? 2 :3))); //设置内核电压
  if(DCO_FLL_Fre < 12MHz)
    SetVcoreUp (0x00);                      //设置内核电压
  else if(DCO_FLL_Fre < 16MHz)
    SetVcoreUp (0x01);       
  else if(DCO_FLL_Fre < 20MHz)
    SetVcoreUp (0x02);       
  else
    SetVcoreUp (0x03);       
  
  __bis_SR_register(SCG0);                                              // 禁止倍频环FLL,
  UCS->CTL0 = 0x0000;                                                   // 清零 DCOx, MODx
  
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
  ASSERT(FLLN_VALUE < 1024,"CLOCK_DCO_PLLConfig","FLLN_VALUE不允许超过1023，请将DCO频率设低或者更换为更高频率的参考时钟源！");         //不允许超过1023，请将DCO频率设低或者更换为更高频率的参考时钟源
  
  g_sClock.DCO_FLL_Frequency = DCO_FLL_Fre;
  
  UCS->FLLN = FLLN_VALUE;        //
  UCS->FLLD = 0;     //设置DCO分频  
  
  __bic_SR_register(SCG0);                  // 使能FLL
  do
  {
    UCS->DCO_FFG = RESET; // 清除,CLOCK_DCO 失效标志                                        
    SFRIFG1 &= ~OFIFG;                                             // 清除时钟失效标志
  }while (UCS->DCO_FFG == BIT_SET);                                           // 检查DCO失效标志
 
  //将使用DCO作为时钟源的时钟频率值修改
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
*  函数名称：CLOCK_MCLK_Config  (CLOCK_Source mclk , CLOCK_DIVx mclk_div)
*  功能说明：设置主时钟源及分频
*  参数说明：CLOCK_Source mclk   :主时钟时钟源
             CLOCK_DIVx mclk_div :主时钟分频系数
*  函数返回：无
*  使用示例：CLOCK_MCLK_Config  (CLOCK_DCO , DIV_1);   //主时钟使用DCO作为时钟源，分频系数为1（不分频）
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
  
  UCS->SELM = mclk;                         //选择DCO作为时钟源
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
*  函数名称：CLOCK_SMCLK_Config (CLOCK_Source smclk, CLOCK_DIVx smclk_div)
*  功能说明：设置系统时钟源及分频
*  参数说明：CLOCK_Source smclk   :系统时钟时钟源
             CLOCK_DIVx smclk_div :系统时钟分频系数
*  函数返回：无
*  使用示例：CLOCK_SMCLK_Config  (CLOCK_DCO , DIV_2);   //系统时钟使用DCO作为时钟源，分频系数为2（二分频）
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
  
  UCS->SELS = smclk;//选择smclk时钟源
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
*  函数名称：CLOCK_ACLK_Config  (CLOCK_Source aclk , CLOCK_DIVx aclk_div)
*  功能说明：设置辅助时钟源及分频
*  参数说明：CLOCK_Source aclk   :辅助时钟时钟源
             CLOCK_DIVx aclk_div :辅助时钟分频系数
*  函数返回：无
*  使用示例：CLOCK_ACLK_Config  (XT! , DIV_4);   //辅助时钟使用XT1作为时钟源，分频系数为4（四分频）
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
  
  UCS->SELA = aclk;//选择时钟源                   
  UCS->DIVA = aclk_div; //设置分频系数为0
  
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
函数功能：设置内核电压值（与频率设置有关）
函数参数：u8 level ：电压阶梯 小于3
********************************************************************************/
void SetVcoreUp (uint8_t level)
{
#if 1  //仿真时修改为0，否则会卡在死循环里
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
