/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
 * @brief      MSP430F5529 ƽ̨������

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

__interrupt void RTC_IRQ_Handler()
{
    if (RTC_GetITStatus(RTC_Alarm_IRQn) == TRUE)
    {
        LED_On(LED1);               //ָʾ����
        RTC_ClearITPendingBit(RTC_Alarm_IRQn);                       //
    }
    if (RTC_GetITStatus(RTC_TimeEvent_IRQn) == TRUE)
    {
        LED_Turn(LED2);               //ָʾ����
        RTC_ClearITPendingBit(RTC_TimeEvent_IRQn);                       //
    }
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    OLED_Init();                       //OLED��ʼ��

    Calendar nowDate;
    nowDate.Seconds = 50;
    nowDate.Hours = 1;
    nowDate.DayOfMonth = 1;
    nowDate.DayOfWeek = 3;
    nowDate.Year = 2015;
    nowDate.Month = 10;
    nowDate.Minutes = 40;
    RTC_Init(nowDate);

    nowDate.Minutes++;
    nowDate.Seconds = 5;
    Set_Vector_Handler(VECTOR_RTC, RTC_IRQ_Handler);
    RTC_SetAlarm(nowDate, Every_Hours);              //��������ʱ��
    RTC_ITConfig(RTC_Alarm_IRQn, ENABLE);              //ʹ������
    RTC_ITConfig(RTC_TimeEvent_IRQn, ENABLE);          //����RTCĳһ���ж�ʹ��
    EnableInterrupts();              //�����ж�
    while (1)
    {
        DELAY_MS(200);
        nowDate = RTC_GetTime();
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "%d:%d:%d   \n", nowDate.Year, nowDate.Month, nowDate.DayOfMonth);
        OLED_PrintfAt(FONT_ASCII_6X8, 1, 0, "WeekDay:%d   ", nowDate.DayOfWeek);
        OLED_PrintfAt(FONT_ASCII_6X8, 2, 0, "%d:%d:%d   \n", nowDate.Hours, nowDate.Minutes, nowDate.Seconds);
    }
}
