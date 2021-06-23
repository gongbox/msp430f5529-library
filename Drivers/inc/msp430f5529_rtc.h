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
  Every_Hours,    //ÿ��Сʱ��ĳһ����
  Every_Days,     //ÿ���ĳһ����
  Every_Weeks,    //ÿ�����ڵ�ĳһ����
  Every_Months,   //ÿ���µ�ĳһ����
}Alarm_Type;  //����ģʽ
typedef enum
{
  RTC_Ready_IRQn          =(0x01u),     /* RTC Ready Interrupt   */
  RTC_Alarm_IRQn          =(0x02u),     /* RTC Alarm Interrupt   */
  RTC_TimeEvent_IRQn      =(0x04u),     /* RTC Time Event Interrupt   *///ʱ���¼�,�ο�����ĺ궨�壺RTC_TimeEvent
}RTC_IRQn;

extern void    RTC_Init    (Calendar nowDate);                                  //��ʼ��RTC�������õ�ǰʱ��
extern void    RTC_SetTime (Calendar nowDate);                                  //����RTCʱ��
extern Calendar RTC_GetTime(void);                                              //��ȡRTCʱ��
extern void    RTC_SetAlarm(Calendar alarm_Time, Alarm_Type);                   //��������ʱ��

extern void    RTC_ITConfig(RTC_IRQn, STATUS ITState);                          //����RTCĳһ���ж��Ƿ�ʹ��
extern STATUS  RTC_GetITStatus(RTC_IRQn);                                       //��ȡĳλ�жϱ�־״̬
extern void    RTC_ClearITPendingBit(RTC_IRQn);                                 //���ĳһλ�жϱ�־

#endif