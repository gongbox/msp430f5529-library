#include "msp430f5529_rtc.h"
#include "msp430f5529_clock.h"

const RTC_MemMapPtr  RTC = RTC_BASE_PTR; 
/*******************************************************************************
*  函数名称：RTC_Init(Calendar nowDate)
*  功能说明：RTC 初始化并设置时间
*  参数说明：Calendar nowDate : 设置当前时间
*  函数返回：无
*  使用示例：Calendar  nowDate;RTC_Init(nowDate);//初始化RTC并设置RTC时间
*  注意：日历模式下要求ACLK为32768HZ
********************************************************************************/
void RTC_Init(Calendar nowDate)
{
  ASSERT(g_sClock.ACLK.nHZ == 32768,"RTC_Init","时钟配置不合理，请将ACLK频率配置为32768HZ！");                //日历模式下要求ACLK为32768HZ
  /*uint32_t clock_div_result = g_sClock.ACLK.nHZ;          
  uint16_t clock_div_value = 0;//二分频次数
  while(clock_div_result>1)  //不断分频，直到分频后的值为1或小于1（要求为1）
  {
    clock_div_result = clock_div_result>>1; //二分频一次
    clock_div_value++;                      //分频系数加一
  };
  ASSERT((clock_div_result==1)&&(clock_div_value>=2)&&(clock_div_value<=16));        //要求时钟二分频最多16次，且最少2次，且分频后的频率值为1HZ
  */
  RTCCTL01 = RTCHOLD + RTCMODE + RTCSSEL__RT1PS + RTC_TimeEvent;           //使能日历模式
  
  RTCPS0CTL =   RT0PSHOLD ;             //使用ACLK作为时钟
  RTCPS1CTL  =  RT1PSHOLD + RT1SSEL_2; //使用 RTCPS0输出作为时钟源     
  
  //RTC->RT1_PSDIV = (clock_div_value <= 9) ? (clock_div_value-2u) : 7u;
  //RTC->RT0_PSDIV = (clock_div_value >  9) ? (clock_div_value-9u) : 0u;
  
  RTC->HOLD = RESET;                 //打开RTC模块 
  RTC->RT0_PSHOLD = RESET;           //打开RTCPS0CTL
  RTC->RT1_PSHOLD = RESET;           //打开RTCPS1CTL
  
  RTC_SetTime(nowDate);    
}
/*******************************************************************************
*  函数名称：RTC_SetTime(Calendar nowDate)
*  功能说明：设置当前时间
*  参数说明：Calendar nowDate : 当前时间
*  函数返回：无
*  使用示例：RTC_SetTime(nowDate);  //设置RTC时间
********************************************************************************/
void RTC_SetTime(Calendar nowDate)
{
  (*((Calendar *)(&RTCSEC))) = nowDate;  
}
/*******************************************************************************
*  函数名称：RTC_GetTime(void)
*  功能说明：获取当前时间
*  参数说明：无
*  函数返回：Calendar nowDate : 当前时间
*  使用示例：Calendar nowDate = RTC_GetTime();  //获取当前时间
********************************************************************************/
Calendar RTC_GetTime(void)
{
  while(RTC->RDY == RESET);                  //等待时钟准备好读
  return (*((Calendar *)(&RTCSEC)));         //因为该寄存器结构与Calendar结构体相同，直接将寄存器值返回
}
/*******************************************************************************
*  函数名称：RTC_SetAlarm    (Calendar alarm_Time,Alarm_Type alarm_type)
*  功能说明：设置闹钟时间及类型
*  参数说明：Calendar alarm_Time     :闹钟时间
             Alarm_Type alarm_type   :闹钟类型
*  函数返回：无
*  使用示例：RTC_SetAlarm (alarm_Time,Every_Days); //闹钟时间设置，每天触发一次闹钟事件
********************************************************************************/
void RTC_SetAlarm    (Calendar alarm_Time,Alarm_Type alarm_type)
{
  //写入闹钟时间
  RTCAMIN  = alarm_Time.Minutes    ;
  RTCAHOUR = alarm_Time.Hours      ;
  RTCADAY  = alarm_Time.DayOfMonth ; 
  
  BIT_VALUE_IN_BYTE(RTCAMIN,7) = BIT_SET;
  BIT_VALUE_IN_BYTE(RTCAHOUR,7) = (alarm_type == Every_Days  ||
                              alarm_type == Every_Weeks ||
                              alarm_type == Every_Months 
                              ) ? BIT_SET : RESET;
  BIT_VALUE_IN_BYTE(RTCADOW,7) = alarm_type == Every_Weeks  ? BIT_SET : RESET;
  BIT_VALUE_IN_BYTE(RTCADAY,7) = alarm_type == Every_Months ? BIT_SET : RESET;
}
/*******************************************************************************
*  函数名称：RTC_ITConfig   (RTC_IRQn irqn, STATUS ITState)
*  功能说明: 设置RTC某一个中断是否使能
*  参数说明：RTC_IRQn irqn   : 中断事件选择
             STATUS ITState  : 是否使能中断
*  函数返回：无
*  使用示例：RTC_ITConfig   (RTC_Alarm_IRQn, ENABLE);  //使能闹钟中断
********************************************************************************/
void RTC_ITConfig   (RTC_IRQn irqn, STATUS ITState)
{
  if(ITState != DISABLE)
  {
    RTC->IE |= irqn;
  }
  else
  {
    RTC->IE &=~ irqn;
  }
}
/*******************************************************************************
*  函数名称：RTC_GetITStatus(RTC_IRQn irqn)
*  功能说明：获取RTC某一位中断标志
*  参数说明：RTC_IRQn irqn : 中断事件选择
*  函数返回：STATUS : TRUE 中断事件发生，FALSE 中断事件未发生
*  使用示例：if(TRUE == RTC_GetITStatus(RTC_Alarm_IRQn)){...}  //判断闹钟时间是否发生
********************************************************************************/
STATUS RTC_GetITStatus(RTC_IRQn irqn)
{
  return ((RTC->IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  函数名称：RTC_ClearITPendingBit(RTC_IRQn irqn)
*  功能说明：清除RTC某一位中断标志
*  参数说明：RTC_IRQn irqn : 中断事件选择
*  函数返回：无
*  使用示例：RTC_ClearITPendingBit(RTC_Alarm_IRQn); //清除RTC闹钟中断标志位
********************************************************************************/
void RTC_ClearITPendingBit(RTC_IRQn irqn)
{
  RTC->IFG &=~ irqn;
}
