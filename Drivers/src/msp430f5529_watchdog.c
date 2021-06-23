#include "msp430f5529_watchdog.h"
#include "msp430f5529_clock.h"

static const uint32_t WDT_Counter[]={2000000000u,128000000u,8192000u,512000u,32768u,8192u,512u,64u};
const WDT_MemMapPtr WDTX = WDT_BASE_PTR;
extern const SFR_MemMapPtr SFRX;
uint16_t WatchDog_Time_Cal(uint32_t ms);
/*******************************************************************************
*  函数名称：DisableWatchDog(void)
*  功能说明：关闭看门狗
*  参数说明：无
*  函数返回：无
********************************************************************************/
void DisableWatchDog(void)
{
  WDTCTL = (WDTCTL&0xFF)|(WDTPW+WDTHOLD);
} 
/*******************************************************************************
*  函数名称：WatchDog_Init(uint32_t ms) 
*  功能说明：设置看门狗时间，注意定时时间只是比较接近设置时间的一个时间，并不精准,返回及时间偏
*  参数说明：uint32_t ms          ：功能时间
*  函数返回：//误差时间（单位：ms）
*  使用示例： WatchDog_Init(500);   //设置看门狗时间为500ms
********************************************************************************/
void  WatchDog_Init(uint32_t ms)                                        //看门狗功能配置
{
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //关闭看门狗中断
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val ; //配置
}
/*******************************************************************************
*  函数名称：WatchDog_Timer_Init(uint32_t ms) 
*  功能说明：设置看门狗作为定时器并设置时间，注意定时时间只是比较接近设置时间的一个时间，并不精准,返回及时间偏
*  参数说明：uint32_t ms          ：功能时间
*  函数返回：//误差时间（单位：ms）
*  使用示例： WatchDog_Timer_Init(500);   //设置看门狗定时时间为500ms
********************************************************************************/
void  WatchDog_Timer_Init(uint32_t ms)                                  //看门狗定时器功能初始化
{
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //关闭看门狗中断
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val | WDTTMSEL; //配置
}
/*******************************************************************************
*  函数名称：WatchDog_Delay_Ms(uint32_t ms) 
*  功能说明：设置看门狗延时一段时间，注意定时时间只是比较接近设置时间的一个时间，并不精准,返回及时间偏
*  参数说明：uint32_t ms          ：功能时间
*  函数返回：//误差时间（单位：ms）
*  使用示例：WatchDog_Delay_Ms(500);   //设置在此处使用看门狗延时500ms
********************************************************************************/
void  WatchDog_Delay_Ms  (uint32_t ms)                                  //看门狗延时使用
{
  SFRX->IFG.WDT = RESET;
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //关闭看门狗中断
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val | WDTTMSEL; //配置
  while(SFRX->IFG.WDT == RESET);
}
/*************************************************************************
*  函数名称：WatchDog_Feed(void)
*  功能说明：喂狗
*  参数说明：无
*  函数返回：无
*  使用示例：WatchDog_Feed();  //喂狗，清除看门狗计数值
*************************************************************************/  
void  WatchDog_Feed(void)
{
  WDTCTL =(WDTCTL&0X00FF) | (WDTCNTCL + WDTPW);                //计数值清零
}
/*************************************************************************
*  函数名称：WatchDog_ITConfig(STATUS ITState)
*  功能说明：设置看门狗中断是否使能
*  参数说明：STATUS ITState ：是否使能中断
*  函数返回：无
*  使用示例：WatchDog_ITConfig(ENABLE);  //使能看门狗中断
*************************************************************************/
void  WatchDog_ITConfig(STATUS ITState)
{
  if(ITState != DISABLE)
  {
    SFRX->IFG.WDT = RESET;
    SFRX->IE.WDT = ENABLE;  //开启看门狗中断
  }
  else
  {
    SFRX->IE.WDT = DISABLE;
  }
}
/*************************************************************************
*  函数名称：SoftWare_RESET(void)
*  功能说明：软件触发一次复位
*  参数说明：无
*  函数返回：无
*  使用示例：SoftWare_RESET();  //软件复位一次单片机
*****************************************************{********************/  
void  SoftWare_RESET(void)
{
  WDTCTL = 0x0000;
}
//看门狗计算时间函数
uint16_t WatchDog_Time_Cal(uint32_t ms)
{
  float smclk_OffectTime,aclk_OffectTime,vlo_OffectTime; //误差时间
  int32_t smclk_OffectTimeTemp = 0,aclk_OffectTimeTemp = 0 ,vlo_OffectTimeTemp = 0;
  uint16_t smclk_DividerValue = 7 ,aclk_DividerValue = 7, vlo_DividerValue = 7;
  uint32_t SelectClock;
  uint32_t WDT_CounterValue;
  /**********************计算时钟源及计时时间**********************************/
  //SMCLK
  SelectClock = g_sClock.SMCLK.nHZ/1000;
  WDT_CounterValue = ms*SelectClock;
  while((WDT_CounterValue > WDT_Counter[smclk_DividerValue]) && (smclk_DividerValue > 0))
  {
    smclk_DividerValue --;
  }
  if(smclk_DividerValue == 7 || smclk_DividerValue == 0)
  {
    smclk_OffectTimeTemp = (WDT_Counter[smclk_DividerValue]-WDT_CounterValue);
  }
  else //在两个时间内选出误差时间最短的
  {
    if((WDT_Counter[smclk_DividerValue ]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[smclk_DividerValue +1]))
    {
      smclk_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++smclk_DividerValue ]);
    }
    else
    {
      smclk_OffectTimeTemp = (WDT_Counter[smclk_DividerValue ]-WDT_CounterValue);
    }
  }
  smclk_OffectTimeTemp = smclk_OffectTimeTemp>0 ? smclk_OffectTimeTemp : -smclk_OffectTimeTemp;
  smclk_OffectTime = (float)smclk_OffectTimeTemp/(float)g_sClock.SMCLK.nHZ;
  /*ACLK*/
  SelectClock = g_sClock.ACLK.nHZ;
  WDT_CounterValue = ms*SelectClock/1000;
  while((WDT_CounterValue > WDT_Counter[aclk_DividerValue]) && (aclk_DividerValue > 0))
  {
    aclk_DividerValue--;
  }
  if(aclk_DividerValue == 7 || aclk_DividerValue == 0)
  {
    aclk_OffectTimeTemp = (WDT_Counter[aclk_DividerValue]-WDT_CounterValue);
  }
  else //在两个时间内选出误差时间最短的
  {
    if((WDT_Counter[aclk_DividerValue ]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[aclk_DividerValue +1]))
    {
      aclk_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++aclk_DividerValue]);
    }
    else
    {
      aclk_OffectTimeTemp = (WDT_Counter[aclk_DividerValue]-WDT_CounterValue);
    }  
  }
  aclk_OffectTimeTemp = aclk_OffectTimeTemp>0 ? aclk_OffectTimeTemp : -aclk_OffectTimeTemp;
  aclk_OffectTime = (float)aclk_OffectTimeTemp/(float)g_sClock.ACLK.nHZ;
  /**VLOCLK_10K*/
  SelectClock = VLOCLK_FREQUENCY;
  WDT_CounterValue = ms*SelectClock/1000;
  while((WDT_CounterValue > WDT_Counter[vlo_DividerValue]) && (vlo_DividerValue > 0))
  {
    vlo_DividerValue--;
  }
  if(vlo_DividerValue == 7 || vlo_DividerValue == 0)
  {
    vlo_OffectTimeTemp = (WDT_Counter[vlo_DividerValue]-WDT_CounterValue);
  }
  else //在两个时间内选出误差时间最短的
  {
    if((WDT_Counter[vlo_DividerValue]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[vlo_DividerValue+1]))
    {
      vlo_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++vlo_DividerValue]);
    }
    else
    {
      vlo_OffectTimeTemp = (WDT_Counter[vlo_DividerValue]-WDT_CounterValue);
    }
  }
  vlo_OffectTimeTemp = vlo_OffectTimeTemp>0 ? vlo_OffectTimeTemp : -vlo_OffectTimeTemp;
  vlo_OffectTime = (float)vlo_OffectTimeTemp/(float)VLOCLK_FREQUENCY; 
  /*********比较各个时钟源的定时精度，选择最高精度的时钟源及定时时间**********/
  if((smclk_OffectTime <= aclk_OffectTime) && (smclk_OffectTime <= vlo_OffectTime))
  {
    return (WDTSSEL__SMCLK | smclk_DividerValue);
  }
  else if((aclk_OffectTime <= smclk_OffectTime) && (aclk_OffectTime <= vlo_OffectTime))
  {
    return (WDTSSEL__ACLK | aclk_DividerValue);
  }
  else
  {
    return (WDTSSEL__VLO | vlo_DividerValue);
  }
}