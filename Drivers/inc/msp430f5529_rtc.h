#ifndef RTC_H_
#define RTC_H_

#include"common.h"

#define RTC_TimeEvent  RTCTEV__MIN   //RTCTEV__MIN     /* RTC Time Event: 0 (Min. changed) */
                                     //RTCTEV__HOUR    /* RTC Time Event: 1 (Hour changed) */
                                     //RTCTEV__0000    /* RTC Time Event: 2 (12:00 changed) */
                                     //RTCTEV__1200    /* RTC Time Event: 3 (00:00 changed) */
typedef struct
{
  uint8_t Seconds;    //! Seconds of minute between 0-59
  uint8_t Minutes;    //! Minutes of hour between 0-59
  uint8_t Hours;      //! Hour of day between 0-23
  uint8_t DayOfWeek;  //! Day of week between 0-6
  uint8_t DayOfMonth; //! Day of month between 1-31
  uint8_t Month;      //! Month between 0-11
  uint16_t Year;      //! Year between 0-4095
}Calendar;   
typedef enum
{
  Every_Hours,    //每个小时的某一分钟
  Every_Days,     //每天的某一分钟
  Every_Weeks,    //每个星期的某一分钟
  Every_Months,   //每个月的某一分钟
}Alarm_Type;  //闹钟模式
typedef enum
{
  RTC_Ready_IRQn          =(0x01u),     /* RTC Ready Interrupt   */
  RTC_Alarm_IRQn          =(0x02u),     /* RTC Alarm Interrupt   */
  RTC_TimeEvent_IRQn      =(0x04u),     /* RTC Time Event Interrupt   *///时钟事件,参考上面的宏定义：RTC_TimeEvent
}RTC_IRQn;

extern void    RTC_Init    (Calendar nowDate);                                  //初始化RTC，并设置当前时间
extern void    RTC_SetTime (Calendar nowDate);                                  //设置RTC时间
extern Calendar RTC_GetTime(void);                                              //获取RTC时间
extern void    RTC_SetAlarm(Calendar alarm_Time, Alarm_Type);                   //设置闹钟时间

extern void    RTC_ITConfig(RTC_IRQn, STATUS ITState);                          //设置RTC某一个中断是否使能
extern STATUS  RTC_GetITStatus(RTC_IRQn);                                       //获取某位中断标志状态
extern void    RTC_ClearITPendingBit(RTC_IRQn);                                 //清除某一位中断标志

#endif