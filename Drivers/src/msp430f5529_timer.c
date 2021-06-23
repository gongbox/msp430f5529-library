#include "msp430f5529_timer.h"
#include "msp430f5529_clock.h"
#include "msp430f5529_gpio.h"
#include "msp430f5529_system.h"
#include "stdarg.h"

//定义4个指针数组保存 TIMERX 的地址
const TIMER_MemMapPtr TIMERX[TIMER_NUM] ={TIMER_A0_BASE_PTR,TIMER_A1_BASE_PTR,TIMER_A2_BASE_PTR,TIMER_B0_BASE_PTR};
//定义数组保存占空比最大值
const uint32_t TIMER_PRECISON[TIMER_NUM]={TIMER_PWM_A0_PRECISON,TIMER_PWM_A1_PRECISON,TIMER_PWM_A2_PRECISON,TIMER_PWM_B0_PRECISON};
//定义通道引脚
const GPIO_PIN TIMER_CHANEL_PIN[TIMER_NUM][TIMER_CH_NUM]=
{// CH0    CH1    CH2    CH3    CH4    CH5    CH6
  {{P1,1},{P1,2},{P1,3},{P1,4},{P1,5}},                  //TIMER_A0
  {{P1,7},{P2,0},{P2,1}},                                //TIMER_A1
  {{P2,3},{P2,4},{P2,5}},                                //TIMER_A2
  {{P5,6},{P5,7},{P7,4},{P7,5},{P7,6},{P3,5},{P3,6}}     //TIMER_B0
};//通道引脚
const GPIO_PIN TIMER_CLK_PIN[TIMER_NUM] =
{
  {P1,0},                                                //TIMER_A0
  {P1,6},                                                //TIMER_A1
  {P2,2},                                                //TIMER_A2
  {P7,7}                                                 //TIMER_B0
};//外部时钟输入引脚
/*******************************************************************************
*  函数名称：TIMER_PWM_MultiChannel_Init(TIMERn timer, uint32_t fre, uint8_t ChannelNum,...)
*  功能说明：定时器PWM输出初始化(可以多个通道同时初始化)
*  参数说明：TIMERn timer       ：定时器模块
             uint32_t fre         ：频率
             uint8_t ChannelNum   ：要初始化的通道数
             ...                ：初始化的通道
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：实例一：（使用函数初始化时需要输入初始化通道数，初始化通道数可以为0，如：TIMER_PWM_MultiChannel_Init(TIMER_A0, 1000, 0);）
             TIMER_PWM_MultiChannel_Init(TIMER_A0, 1000, 3, TIMER_CH1, TIMER_CH2, TIMER_CH4);
             //定时器TIMER_A0初始化输出PWM波，频率为1000Hz,初始化3个通道，分别是：TIMER_CH1,TIMER_CH2,TIMER_CH4
             实例二：(使用宏定义初始化时不需要输入初始化通道数)
             TIMER_PWM_Init(TIMER_B0,1000,TIMER_CH1,TIMER_CH2,TIMER_CH3,TIMER_CH4,TIMER_CH5,TIMER_CH6);
             //定时器TIMER_B0初始化输出PWM波，频率为1000Hz,初始化6个通道：TIMER_CH1,TIMER_CH2,TIMER_CH3,TIMER_CH4,TIMER_CH5,TIMER_CH6
********************************************************************************/
STATUS TIMER_PWM_MultiChannel_Init(TIMERn timer, uint32_t fre, uint8_t ChannelNum,...)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_PWM_MultiChannel_Init",
         "定时器已经设置为默认延时功能，不能再作为其他功能使用");       //不允许这种情况下还宏定义该定时器作为延时所用的定时器，请在system.h里修改DELAY_TIMER宏定义
#endif
  ASSERT((timer==TIMER_A0&&ChannelNum<=TIMER_CH4)
       ||(timer==TIMER_A1&&ChannelNum<=TIMER_CH2)
       ||(timer==TIMER_A2&&ChannelNum<=TIMER_CH2)
       ||(timer==TIMER_B0&&ChannelNum<=TIMER_CH6),
       "TIMER_PWM_MultiChannel_Init",
       "定时器通道参数超出范围！"); //断言检测输入参数*/
  
  va_list ap;
  va_start(ap,ChanelNum);
  for(int i=0;i < ChannelNum;i++)           //根据初始化的通道数，逐一初始化通道
  {
    TIMER_CHn ch = va_arg(ap, TIMER_CHn);  //读取要初始化的通道
    if((ch == TIMER_CH0) || (ch >= TIMER_CH_NUM))   //判断通道值是否合理
    {
      return ERROR;                                 //不合理返回ERROR
    }
    //初始化端口
    GPIO_Init(TIMER_CHANEL_PIN[timer][ch].Port,TIMER_CHANEL_PIN[timer][ch].Pin,GPO|SEL);
    //初始化占空比及输出波形
    TIMERX[timer]->CCR[ch] = 0u;
    TIMERX[timer]->CCTL[ch].IE = RESET;               //关闭通道中断
    TIMERX[timer]->CCTL[ch].OUTMOD = DEFAULT_PWM_OUTMOD;      //输出波形设置
    TIMERX[timer]->CCTL[ch].CAPMODE = RESET;          //比较模式
  }
  va_end(ap);
  
  return TIMER_PWM_SetFrequency(timer,fre);
}
/*******************************************************************************
*  函数名称：TIMER_PWM_SetChannelOutmod(TIMERn timer,TIMER_CHn ch,TIMER_OUTMODn outmod)
*  功能说明：PWM输出波形设置
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
             TIMER_PWM_OUTMODn outmod ：输出波形模式
*  函数返回：无
*  使用例程：TIMER_PWM_SetChannelOutmod(TIMER_A0,TIMER_CH1,TIMER_PWM_OUTMOD3);  //修改定时器A0的TIMER_CH1通道输出波形
********************************************************************************/
void TIMER_PWM_SetChannelOutmod(TIMERn timer,TIMER_CHn ch,TIMER_PWM_OUTMODn outmod)
{
  TIMERX[timer]->CCTL[ch].OUTMOD = outmod;      //输出波形设置
}
/*******************************************************************************
*  函数名称：TIMER_PWM_SetFrequency(TIMERn timer,uint32_t fre)
*  功能说明：定时器PWM频率设置(注意这里没有更改各个通道的占空比，所以修改频率后占空比变了，需要从新设置占空比)
*  参数说明：TIMERn timer       ：定时器模块
             uint32_t fre         ：频率
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_PWM_SetFrequency(TIMER_B0,1000);       //将定时器B输出频率修改为1000Hz
********************************************************************************/
STATUS  TIMER_PWM_SetFrequency(TIMERn timer,uint32_t fre)
{
  if(fre == 0u)
  {
    TIMERX[timer]->MC = TIMER_MC_STOP;              //暂停输出PWM波
    return SUCCESS;
  }
  
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;           //先选择时钟源为SMCLK
  TIMERX[timer]->ID = 0u;                           //分频系数清零
  //时钟大小
  uint32_t Counter_Value = g_sClock.SMCLK.nHZ/fre;                 //计算计数值
  while(Counter_Value > 65536u)        //这里不是0xffff，因为后面会-1，所以是(0xffff+1)=65536
  {  //分频一次
     TIMERX[timer]->ID ++;          
     Counter_Value >>= 1;          
     
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //这个时候意味得选择更低频率的时钟了
     {
       if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK)   //如果时钟已经是最低的ACLK了，那么意味着设置的频率太低了，失败返回ERROR
       {
         return ERROR;
       }
       //更换为更低的ACLK，从新配置
       TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
       TIMERX[timer]->ID = 0u;  
       Counter_Value = g_sClock.ACLK.nHZ/fre; 
       continue;  
     } 
  }
  if(Counter_Value <= 1)//出现这种情况,说明频率不合适
  {
    return ERROR; 
  }

  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//为什么要减一？？因为TAR/TBR计数器从CCR0变化到0还有一个计数周期
  TIMERX[timer]->IE = RESET;                      //关闭中断
  TIMERX[timer]->MC = TIMER_MC_UP;                //增计数模式
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_PWM_SetChannelDuty(TIMERn timer,TIMER_CHn ch,uint32_t duty)
*  功能说明：PWM输出占空比设置
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
             uint32_t duty      ：占空比
*  函数返回：无
*  使用例程：TIMER_PWM_SetChannelDuty(TIMER_A0,TIMER_CH1,800);  //修改定时器A0的TIMER_CH1通道占空比为(800/TIMAR_A0_PRECISON)*100%
********************************************************************************/
void  TIMER_PWM_SetChannelDuty(TIMERn timer,TIMER_CHn ch,uint32_t duty)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_PWM_SetChannelDuty",
       "定时器通道参数超出范围！"); //断言检测输入参数
  
  ASSERT(duty <= TIMER_PRECISON[timer],
         "TIMER_PWM_SetChannelDuty",
         "定时器占空比参数超出范围！");      //用断言检测 占空比是否合理
  
  TIMERX[timer]->CCR[ch] = (uint16_t)((duty * (TIMERX[timer]->CCR[0]+1))/TIMER_PRECISON[timer]);
}
/*******************************************************************************
*  函数名称：TIMER_Interval_Us(TIMERn timer,uint32_t us)
*  功能说明：定时器定时初始化(单位：us)
*  参数说明：TIMERn timer       ：定时器模块
             uint32_t us          : 定时时间
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_Interval_Us(TIMER_B0,2500);  //TB定时间隔2.5ms中断
********************************************************************************/
STATUS  TIMER_Interval_Us(TIMERn timer,uint32_t us)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Interval_Us",
         "定时器已经设置为默认延时功能，不能再作为其他功能使用");       //不允许这种情况下还宏定义该定时器作为延时所用的定时器，请在system.h里修改DELAY_TIMER宏定义
#endif
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u;
  //计数值
  uint32_t Counter_Value = (uint32_t)(us * g_sClock.SMCLK.fMHZ);
  while(Counter_Value > 65536)
  {
     TIMERX[timer]->ID ++; 
     Counter_Value >>= 1;  
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //这个时候意味得选择更低频率的时钟了
     {
      if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK) //如果时钟已经是最低的ACLK了，那么意味着设置的频率太低了，失败返回ERROR
      {
        return ERROR;
      }
      //更换为更低的ACLK，从新配置
      TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
      TIMERX[timer]->ID = 0u;  
      Counter_Value = (uint32_t)(us * g_sClock.ACLK.fMHZ);
      continue;  
     } 
  }
  if(Counter_Value <= 1)//出现这种情况,说明频率不合适
  {
    return ERROR; 
  }
  
  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//为什么要减一？？因为TAR/TBR计数器从CCR0变化到0还有一个计数周期
  TIMERX[timer]->CCTL[0].CAPMODE = RESET;          //比较模式
  TIMERX[timer]->IE = RESET;             //关闭溢出中断
  TIMERX[timer]->CCTL[0].IE = RESET;     //关闭通道中断
  TIMERX[timer]->MC = TIMER_MC_UP;      //增计数模式
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_Delay_Us(TIMERn timer,uint32_t us)
*  功能说明：定时器延时（单位：ms）
*  参数说明：TIMERn timer       ：定时器模块
             uint32_t us          : 定时时间
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_Delay_Us(TIMER_B0 ,5000);      //在此处延时5ms
********************************************************************************/
STATUS TIMER_Delay_Us(TIMERn timer,uint32_t us)
{
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u;
  //计数值
  uint32_t Counter_Value = (uint32_t)(us * g_sClock.SMCLK.fMHZ);
  while(Counter_Value > 65536)
  {
     TIMERX[timer]->ID ++; 
     Counter_Value >>= 1;  
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //这个时候意味得选择更低频率的时钟了
     {
      if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK) //如果时钟已经是最低的ACLK了，那么意味着设置的频率太低了，失败返回ERROR
      {
        return ERROR;
      }
      //更换为更低的ACLK，从新配置
      TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
      TIMERX[timer]->ID = 0u;  
      Counter_Value = (uint32_t)(us * g_sClock.ACLK.fMHZ);
      continue;  
     } 
  }
  if(Counter_Value <= 1)//出现这种情况,说明频率不合适
  {
    return ERROR; 
  }
  
  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//为什么要减一？？因为TAR/TBR计数器从CCR0变化到0还有一个计数周期
  TIMERX[timer]->CCTL[0].CAPMODE = RESET;          //比较模式
  TIMERX[timer]->IE = RESET;             //关闭溢出中断
  TIMERX[timer]->CCTL[0].IE = RESET;     //关闭通道中断
  TIMERX[timer]->MC = TIMER_MC_UP;
  
  TIMERX[timer]->CCTL[0].CTL = 0;
  TIMERX[timer]->IFG = RESET;
  while(TIMERX[timer]->IFG == RESET);
  TIMERX[timer]->CTL = 0;
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_Pluse_Init    (TIMERn timer)
*  功能说明：定时器脉冲计数初始化
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_Pluse_Init    (TIMER_A1);    //定时器TA1初始化为脉冲计数功能
********************************************************************************/
STATUS  TIMER_Pluse_Init    (TIMERn timer)
{ 
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Pluse_Init",
         "定时器已经设置为默认延时功能，不能再作为其他功能使用");       //不允许这种情况下还宏定义该定时器作为延时所用的定时器，请在system.h里修改DELAY_TIMER宏定义
#endif 
  //端口设置为输入方向//选择第二功能
  GPIO_Init(TIMER_CLK_PIN[timer].Port,TIMER_CLK_PIN[timer].Pin,GPI|SEL);
  
  TIMERX[timer]->CLR = BIT_SET;                       
  TIMERX[timer]->SSEL = TIMER_SSEL_INCLK;             //时钟源选择为外部输入
  TIMERX[timer]->ID = 0u;                             //不分频
  TIMERX[timer]->MC = TIMER_MC_CONTINOUS;             //连续计数模式
  TIMERX[timer]->IE = RESET;                          //禁止溢出中断
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_Pluse_GetValue(TIMERn timer,uint16_t overflowTime)
*  功能说明：定时器脉冲计数值获取
*  参数说明：TIMERn timer       ：定时器模块
             uint16_t overflowTime ：计数器溢出次数
*  函数返回：定时器脉冲计数值
*  使用例程：
             int32 pulse = TIMER_Pluse_GetValue(TIMER_A1,0);    //在没有发生溢出的情况下读取脉冲个数
             TIMER_Pluse_Clear   (TIMER_A1);                    //读取后应该要清零
********************************************************************************/
uint32_t TIMER_Pluse_GetValue(TIMERn timer,uint16_t overflowTime)
{
  return (TIMERX[timer]->R + (overflowTime*65536u));
}
/*******************************************************************************
*  函数名称：TIMER_Pluse_Clear   (TIMERn timer)
*  功能说明：定时器脉冲计数值清零
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：无
*  使用例程：
             int32 pulse = TIMER_Pluse_Read    (TIMER_A1,0);    //在没有发生溢出的情况下读取脉冲个数
             TIMER_Pluse_Clear   (TIMER_A1);                    //读取后应该要清零
********************************************************************************/
void   TIMER_Pluse_Clear   (TIMERn timer)
{
  TIMERX[timer]->R = 0;
}
/*******************************************************************************
*  函数名称：TIMER_Capture_Init(TIMERn timer,TIMER_CHn ch,TIMER_CAPTUREn capmode)
*  功能说明：定时器捕获初始化
*  参数说明：TIMERn timer        ：定时器模块
             TIMER_CHn ch        ：定时器通道
             TIMER_CAPTUREn capmode：捕获模式
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_Capture_Init(TIMER_A0,TIMER_CH1,CAP_Rising);         //定时器TA0 CCR1通道初始化为边缘捕获，上升沿捕获
********************************************************************************/
STATUS TIMER_Capture_Init(TIMERn timer,TIMER_CHn ch,TIMER_CAPTUREn capmode)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Capture_Init",
         "定时器已经设置为默认延时功能，不能再作为其他功能使用");       //不允许这种情况下还宏定义该定时器作为延时所用的定时器，请在system.h里修改DELAY_TIMER宏定义
#endif
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_Init",
       "定时器通道参数超出范围！"); //断言检测输入参数
 
  TIMERX[timer]->CCTL[ch].CAPMODE = BIT_SET;                //设为捕获模式
  TIMERX[timer]->CCTL[ch].CM = capmode;                 //选择捕获沿
  
  TIMERX[timer]->CCTL[ch].CCIS = (timer == TIMER_B0) ? TIMER_CCIS_1 : TIMER_CCIS_0;
  
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u; //这里选择子系统时钟，除非频率特别低时才使用辅助时钟
  TIMERX[timer]->IE = RESET;              //关闭溢出中断
  TIMERX[timer]->CCTL[ch].IE = RESET;     //关闭通道中断
  TIMERX[timer]->MC = TIMER_MC_CONTINOUS;             //连续计数模式
  
  //端口设置为输入方,选择第二功能
  GPIO_Init(TIMER_CHANEL_PIN[timer][ch].Port,TIMER_CHANEL_PIN[timer][ch].Pin,GPI|SEL);
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_Capture_SetMode(TIMERn timer, TIMER_CHn ch, TIMER_CAPTUREn cap_mode)
*  功能说明：定时器捕获边沿设置
*  参数说明：TIMERn timer        ：定时器模块
             TIMER_CHn ch        ：定时器通道
             TIMER_CAPTUREn capmode：捕获模式
*  函数返回：无
*  使用例程：TIMER_Capture_SetMode(TIMER_A0,TIMER_CH1,CAP_Rising);         //定时器TA0 CCR1通道初始化为边缘捕获，上升沿捕获
********************************************************************************/
void   TIMER_Capture_SetMode   (TIMERn timer, TIMER_CHn ch, TIMER_CAPTUREn cap_mode)
{
   TIMERX[timer]->CCTL[ch].CM = cap_mode;                 //设置捕获沿
}
/*******************************************************************************
*  函数名称：TIMER_Capture_Clear     (TIMERn timer,TIMER_CHn ch)
*  功能说明：某一通道读取数据清零
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
*  函数返回：
*  使用例程：int16 overflowTime;  //定义变量，在定时器溢出中断里对该值增一
             ......
             data=TIMER_Capture_GetTime_Us(TIMER_A0,TIMER_CH1,overflowTime)
             TIMER_Capture_Clear(TIMER_A0,TIMER_CH1);  //清零
             overflowTime=0;       //清零
********************************************************************************/
void   TIMER_Capture_Clear     (TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_Clear",
       "定时器通道参数超出范围！"); //断言检测输入参数
  
  TIMERX[timer]->CCR[ch] = 0u;
  TIMERX[timer]->R = 0u;
}
/*******************************************************************************
*  函数名称：TIMER_Capture_GetValue(TIMERn timer,TIMER_CHn ch)
*  功能说明：读取发生捕获时定时器该通道计数值
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
*  函数返回：
*  使用例程：
             uint16_t value = TIMER_Capture_GetValue(TIMER_A0,TIMER_CH1);  //读取发生捕获时的时间
********************************************************************************/
uint16_t TIMER_Capture_GetValue(TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_GetValue",
       "定时器通道参数超出范围！"); //断言检测输入参数
  
  return TIMERX[timer]->CCR[ch];
}
/*******************************************************************************
*  函数名称：TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime)
*  功能说明：读取发生捕获时定时器该通道计数值
*  参数说明：uint16_t start_value  ：捕获起始值
             uint16_t stop_value   ：捕获结束值
             uint16_t overflowTime ：计数器溢出次数
*  函数返回：
*  使用例程：uint16 overflowTime;  //定义变量，在定时器溢出中断里对该值增一
             ......
             TIMER_Capture_CalTime_Us(0,TIMER_Capture_GetValue  (TIMER_A0,TIMER_CH1),overflowTime);  //读取发生捕获时的时间
             overflowTime=0;       //清零
********************************************************************************/
uint32_t TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime)
{
  return (uint32_t)((((int32_t)stop_value - (int32_t)start_value) + (uint32_t)overflowTime*65536)/g_sClock.SMCLK.fMHZ);
}
/*******************************************************************************
*  函数名称：TIMER_Timer_Init      (TIMERn timer)
*  功能说明：计时功能初始化
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：不成功则返回ERROR，成功则返回SUCCESS
*  使用例程：TIMER_Timer_Init      (TIMER_A1);     //定时器TA1初始化为计时器
********************************************************************************/
STATUS   TIMER_Timer_Init      (TIMERn timer)
{
#ifdef DELAY_TIMER
 ASSERT((timer != DELAY_TIMER),
         "TIMER_Timer_Init",
         "定时器已经设置为默认延时功能，不能再作为其他功能使用");       //不允许这种情况下还宏定义该定时器作为延时所用的定时器，请在system.h里修改DELAY_TIMER宏定义
#endif
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;             //这里选择子系统时钟
  TIMERX[timer]->ID = 0u; 
  TIMERX[timer]->MC = TIMER_MC_STOP;                  //先停止计数计数
  TIMERX[timer]->IE = RESET;                          //禁止溢出中断
  
  return SUCCESS;
}
/*******************************************************************************
*  函数名称：TIMER_Timer_Start(TIMERn timer)
*  功能说明：计时功能开始计时
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：无
*  使用例程：TIMER_Timer_Start(TIMER_A1);      //计时器开始计时
********************************************************************************/
void   TIMER_Timer_Start(TIMERn timer)
{
  TIMERX[timer]->R = 0u;
  TIMERX[timer]->MC = TIMER_MC_CONTINUOUS;
}
/*******************************************************************************
*  函数名称：TIMER_Timer_Stop (TIMERn timer)
*  功能说明：计时功能停止计时
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：返回当前计数值
*  使用例程：TIMER_Timer_Stop (TIMER_A1);   //计时器停止计时
********************************************************************************/
uint16_t   TIMER_Timer_Stop (TIMERn timer)
{
   TIMERX[timer]->MC = TIMER_MC_STOP;
   return TIMERX[timer]->R;
}
/*******************************************************************************
*  函数名称：TIMER_Timer_GetTime_Us(TIMERn timer,uint16_t overflowTime)
*  功能说明：计时时间读取
*  参数说明：TIMERn timer       ：定时器模块
             uint16_t overflowTime ：计数器溢出次数
*  函数返回：返回计时时间 （单位：us）
*  使用例程：int16 overflowTime;  //定义变量，在定时器溢出中断里对该值增一
             ......
             TIMER_Timer_GetTime_Us(TIMAER_A1,overflowTime);  //读取计时时间
             overflowTime=0;       //清零
********************************************************************************/
uint32_t TIMER_Timer_GetTime_Us(TIMERn timer,uint16_t overflowTime)
{
  return (uint32_t)((TIMERX[timer]->R + overflowTime*65536u)/g_sClock.SMCLK.fMHZ);
}
/*******************************************************************************
*  函数名称：TIMER_Run (TIMERn timer)
*  功能说明：定时器工作
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：无
*  使用例程：TIMER_Clear_Counter (TIMAER_A1)；  //启动定时器A1
********************************************************************************/
void   TIMER_Run (TIMERn timer)
{
  TIMERX[timer]->MC = TIMER_MC_CONTINUOUS;
}
/*******************************************************************************
*  函数名称：TIMER_Stop (TIMERn timer)
*  功能说明：定时器工作
*  参数说明：TIMERn timer       ：定时器模块
*  函数返回：无
*  使用例程：TIMER_Stop(TIMAER_A1)；  //停止定时器A1
********************************************************************************/
void   TIMER_Stop (TIMERn timer)
{
  TIMERX[timer]->MC = TIMER_MC_STOP;
}
/*******************************************************************************
*  函数名称：TIMER_GetCounterValue (TIMERn timer)
*  功能说明：读取定时器计数值
*  参数说明：TIMERn timer  ：定时器模块
*  函数返回：uint16_t      ：计数值
*  使用例程：uint16_t value = TIMER_GetCounterValue(TIMAER_A1)；  //读取定时器A1计数值
********************************************************************************/
uint16_t   TIMER_GetCounterValue (TIMERn timer)
{
  return TIMERX[timer]->R;
}
/*******************************************************************************
*  函数名称：TIMER_SetCounterValue(TIMERn timer,uint16_t value)
*  功能说明：设置定时器计数值
*  参数说明：TIMERn timer       ：定时器模块
             uint16_t value     ：计数值
*  函数返回：无
*  使用例程：TIMER_SetCounterValue(TIMAER_A1,0)；  //设置定时器A1计数值为0
********************************************************************************/
void   TIMER_SetCounterValue (TIMERn timer,uint16_t value)
{
  TIMERX[timer]->R = value;
}
/*******************************************************************************
*  函数名称：TIMER_GetChannelCounterValue (TIMERn timer,TIMER_CHn ch)
*  功能说明：读取定时器通道计数值
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道             
*  函数返回：uint16_t value     ：该通道的计数值
*  使用例程：uint16_t value = TIMER_GetChannelCounterValue(TIMAER_A1,TIMER_CH4)；  //读取定时器A1通道4计数值
********************************************************************************/
uint16_t   TIMER_GetChannelCounterValue (TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_GetChannelCounterValue",
       "定时器通道参数超出范围！"); //断言检测输入参数
  
  return TIMERX[timer]->CCR[ch];
}
/*******************************************************************************
*  函数名称：TIMER_SetChannelCounterValue(TIMERn timer,uint16_t value)
*  功能说明：设置定时器计数值
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
             uint16_t value     ：计数值
*  函数返回：无
*  使用例程：TIMER_SetChannelCounterValue(TIMAER_A1,TIMER_CH4,0)；  //设置定时器A1通道4计数值为0
********************************************************************************/
void   TIMER_SetChannelCounterValue (TIMERn timer,TIMER_CHn ch,uint16_t value)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_SetChannelCounterValue",
       "定时器通道参数超出范围！"); //断言检测输入参数
  
  TIMERX[timer]->CCR[ch] = value;
}
/*******************************************************************************
*  函数名称：TIMER_GetChannelPinInValue(TIMERn timer, TIMER_CHn ch)
*  功能说明：获取通道输入值
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
*  函数返回：STATUS : BIT_SET-输入高电平 RESET-输入低电平
*  使用例程：STATUS status = TIMER_GetChannelInValue(TIMAER_A1, TIMER_CH1);  //读取定时器A1通道1输入电平
********************************************************************************/
STATUS TIMER_GetChannelPinInValue (TIMERn timer, TIMER_CHn ch)
{
  return (TIMERX[timer]->CCTL[ch].CH_CCI) ? BIT_SET : RESET;
}
/*******************************************************************************
*  函数名称：TIMER_GetChannelPinOutValue(TIMERn timer, TIMER_CHn ch)
*  功能说明：获取通道输出值
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_CHn ch       ：定时器通道
*  函数返回：STATUS : BIT_SET-输出高电平 RESET-输出低电平
*  使用例程：STATUS status = TIMER_GetChannelOutValue(TIMAER_A1, TIMER_CH1);  //读取定时器A1通道1输出电平
********************************************************************************/
STATUS TIMER_GetChannelPinOutValue(TIMERn timer, TIMER_CHn ch)
{
  return (TIMERX[timer]->CCTL[ch].CH_OUT) ? BIT_SET : RESET;
}
/*******************************************************************************
*  函数名称：TIMER_ITConfig (TIMERn timer,TIMER_IRQn irqn,STATUS ITState)
*  功能说明：设置是否使能TIMER的某一个中断
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_IRQn irqn    ：中断事件
             STATUS ITState     ：是否使能中断
*  函数返回：无
*  使用例程：TIMER_ITConfig (TIMAER_A1,TIMER_CCR0_IRQn,ENABLE); //使能定时器1通道0中断，即定时中断
********************************************************************************/
void  TIMER_ITConfig (TIMERn timer,TIMER_IRQn irqn,STATUS ITState)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    TIMERX[timer]->IE = ITState;
  }
  else
  {
    TIMERX[timer]->CCTL[irqn].IE = ITState;
  }
}
/*******************************************************************************
*  函数名称：TIMER_GetITStatus(TIMERn timer,TIMER_IRQn irqn)
*  功能说明：获取TIMER的某一个中断标志
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_IRQn irqn    ：中断事件
*  函数返回：STATUS : TRUE-中断事件发生 FALSE-中断事件未发生
*  使用例程：if(TRUE == TIMER_GetITStatus(TIMAER_A1,TIMER_CCR1_IRQn)){...} //判断定时器A1通道1中断事件是否发生
********************************************************************************/
STATUS TIMER_GetITStatus(TIMERn timer,TIMER_IRQn irqn)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    return ((TIMERX[timer]->IFG == BIT_SET) ? TRUE : FALSE);
  }
  else
  {
    return ((TIMERX[timer]->CCTL[irqn].IFG == BIT_SET) ? TRUE : FALSE);
  }
}
/*******************************************************************************
*  函数名称：TIMER_ClearITPendingBit(TIMERn timer,TIMER_IRQn irqn)
*  功能说明：清除TIMER的某一个中断标志
*  参数说明：TIMERn timer       ：定时器模块
             TIMER_IRQn irqn    ：中断事件
*  函数返回：无
*  使用例程：TIMER_ClearITPendingBit(TIMAER_A1,TIMER_CCR1_IRQn);   //清除定时器通道1中断标志位
********************************************************************************/
void  TIMER_ClearITPendingBit(TIMERn timer,TIMER_IRQn irqn)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    TIMERX[timer]->IFG = RESET;
  }
  else
  {
    TIMERX[timer]->CCTL[irqn].IFG = RESET;
  }
}