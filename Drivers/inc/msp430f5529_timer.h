#ifndef TIMER_H_
#define TIMER_H_

#include"common.h"

typedef enum 
{
  TIMER_A0              ,
  TIMER_A1              ,
  TIMER_A2              ,
  TIMER_B0              ,
  TIMER_NUM             ,
}TIMERn;             //定时器模块
typedef enum 
{
  TIMER_CH0             ,  
  TIMER_CH1             ,  
  TIMER_CH2             ,
  TIMER_CH3             , 
  TIMER_CH4             , 
  TIMER_CH5             ,
  TIMER_CH6             , 
  TIMER_CH_NUM          ,
}TIMER_CHn;           //定时器通道
/*********************************PWM功能**************************************/
typedef enum
{
  TIMER_PWM_OUTMOD0     ,
  TIMER_PWM_OUTMOD1     ,
  TIMER_PWM_OUTMOD2     ,
  TIMER_PWM_OUTMOD3     ,
  TIMER_PWM_OUTMOD4     ,
  TIMER_PWM_OUTMOD5     ,
  TIMER_PWM_OUTMOD6     ,
  TIMER_PWM_OUTMOD7     ,
}TIMER_PWM_OUTMODn;       //PWM波输出波形
//输出PWM波功能
#define DEFAULT_PWM_OUTMOD      TIMER_PWM_OUTMOD7  //宏定义PWM波输出默认波形
//定义占空比最大值
#define TIMER_PWM_A0_PRECISON       1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TIMER_A0_PRECISON
#define TIMER_PWM_A1_PRECISON       1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TIMER_A1_PRECISON
#define TIMER_PWM_A2_PRECISON       1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TIMER_A2_PRECISON
#define TIMER_PWM_B0_PRECISON       20000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TIMER_B0_PRECISON   
extern STATUS   TIMER_PWM_MultiChannel_Init(TIMERn, uint32_t fre, uint8_t ChannelNum,...);//定时器PWM功能初始化，使用可变参数，可以根据需要初始化多个通道
//使用下面的宏定义可以不用输入初始化通道数量，如：TIMER_PWM_Init(TIMER_A0,1000,TIMER_CH1,TIMER_CH3); //初始化TIMER_CH1,TIMER_CH3两个通道
#define         TIMER_PWM_Init(timer,fre,...)       TIMER_PWM_MultiChannel_Init(timer,fre,ARG_NUM(__VA_ARGS__),##__VA_ARGS__)
extern void     TIMER_PWM_SetChannelOutmod(TIMERn, TIMER_CHn, TIMER_PWM_OUTMODn);//设置PWM输出波形
extern STATUS   TIMER_PWM_SetFrequency    (TIMERn, uint32_t fre);               //设置某一个定时器PWM的频率,注意：占空比会改变
extern void     TIMER_PWM_SetChannelDuty  (TIMERn, TIMER_CHn, uint32_t duty);   //设置某一个定时器某一个通道的占空比
/*******************************定时中断功能************************************/
extern STATUS   TIMER_Interval_Us   (TIMERn, uint32_t us);                      //初始化一个us级的定时中断
#define         TIMER_Interval_Ms(timer,ms)  TIMER_Interval_Us(timer,(uint32_t)ms*1000)
/*******************************延时中断功能************************************/
extern STATUS   TIMER_Delay_Us(TIMERn, uint32_t us);                            //us级延时
#define         TIMER_Delay_Ms(timer,ms)     TIMER_Delay_Us(timer,(uint32_t)ms*1000)
/*******************************捕获中断功能************************************/
typedef enum
{
  CAP_None              ,  //禁止捕获
  CAP_Rising            ,  //上升沿捕获
  CAP_Falling           ,  //下降沿捕获
  CAP_RisingAndFalling  ,  //上升沿下降沿都捕获
}TIMER_CAPTUREn;      //捕获模式
extern STATUS   TIMER_Capture_Init      (TIMERn, TIMER_CHn, TIMER_CAPTUREn);    //初始化一个通道为捕获模式， 
extern void     TIMER_Capture_SetMode   (TIMERn, TIMER_CHn, TIMER_CAPTUREn);    //设置捕获边沿
extern uint16_t TIMER_Capture_GetValue  (TIMERn, TIMER_CHn);                    //读取发生该捕获条件时的计数值
extern uint32_t TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime);  //计算两次捕获的时间差，注意还要考虑发生两次捕获之间的定时器溢出次数
extern void     TIMER_Capture_Clear     (TIMERn, TIMER_CHn);                    //某一通道读取数据清零
/*******************************脉冲计数功能************************************/
extern STATUS   TIMER_Pluse_Init    (TIMERn);                                   //初始化一个定时器用作脉冲计数
extern uint32_t TIMER_Pluse_GetValue(TIMERn, uint16_t overflowTime);            //读取某一个定时器脉冲计数值
extern void     TIMER_Pluse_Clear   (TIMERn);                                   //清零一个定时器的脉冲计数值
/*******************************计时功能************************************/
extern STATUS   TIMER_Timer_Init    (TIMERn);                                   //计时功能初始化
extern void     TIMER_Timer_Start   (TIMERn);                                   //计时功能启动
extern uint16_t TIMER_Timer_Stop    (TIMERn);                                   //计时功能停止，返回计数值
extern uint32_t TIMER_Timer_GetTime_Us(TIMERn, uint16_t overflowTime);          //读取计时时间(单位：US)
/*******************************中断设置************************************/
typedef enum
{
  TIMER_CCR0_IRQn          ,    /* CCR0_Interrupt *///注意这个中断标志进入中断后会自动清除，进入中断后不要再判断这个标志是否置位
  TIMER_CCR1_IRQn          ,    /* CCR1_Interrupt */
  TIMER_CCR2_IRQn          ,    /* CCR2_Interrupt */
  TIMER_CCR3_IRQn          ,    /* CCR3_Interrupt */
  TIMER_CCR4_IRQn          ,    /* CCR4_Interrupt */
  TIMER_CCR5_IRQn          ,    /* CCR5_Interrupt */
  TIMER_CCR6_IRQn          ,    /* CCR6_Interrupt */
  TIMER_OverFlow_IRQn           /* IFG */ //计数器溢出，捕获，计时，脉冲计数时使能，其他情况下不使能
}TIMER_IRQn;
extern void     TIMER_ITConfig   (TIMERn, TIMER_IRQn, STATUS ITState);          //设置是否使能TIMER的某一个中断
extern STATUS   TIMER_GetITStatus(TIMERn, TIMER_IRQn);                          //获取定时器的某一位中断标志
extern void     TIMER_ClearITPendingBit(TIMERn, TIMER_IRQn);                    //清除TIMER的某一个中断标志
/*******************************其他***********************************/
//定时器运行，停止
extern void     TIMER_Run        (TIMERn);                                      //启动定时器工作
extern void     TIMER_Stop       (TIMERn);                                      //停止定时器
//读取，设置定时器当前计数值
extern uint16_t TIMER_GetCounterValue(TIMERn);                                  //读取定时器计数器值
extern void     TIMER_SetCounterValue(TIMERn, uint16_t value);                  //设置定时器计数器值
//读取，设置定时器某一通道当前计数值
extern uint16_t TIMER_GetChannelCounterValue(TIMERn,TIMER_CHn);                 //读取定时器通道计数器值
extern void     TIMER_SetChannelCounterValue(TIMERn,TIMER_CHn,uint16_t value);  //设置定时器通道计数器值
//读取通道引脚电平状态
extern STATUS   TIMER_GetChannelPinOutValue(TIMERn, TIMER_CHn);                    //获取通道输出值
extern STATUS   TIMER_GetChannelPinInValue (TIMERn, TIMER_CHn);                    //获取通道输入值,可用于捕获时判断是上升沿还是下降沿
#endif