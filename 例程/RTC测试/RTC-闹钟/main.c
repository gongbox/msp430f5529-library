/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
 * @brief      MSP430F5529 平台主程序

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

__interrupt void RTC_IRQ_Handler()
{
    if (RTC_GetITStatus(RTC_Alarm_IRQn) == TRUE)
    {
        LED_On(LED1);               //指示灯亮
        RTC_ClearITPendingBit(RTC_Alarm_IRQn);                       //
    }
    if (RTC_GetITStatus(RTC_TimeEvent_IRQn) == TRUE)
    {
        LED_Turn(LED2);               //指示灯亮
        RTC_ClearITPendingBit(RTC_TimeEvent_IRQn);                       //
    }
}
void main()
{
    DisableInterrupts();            //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化
    OLED_Init();                       //OLED初始化

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
    RTC_SetAlarm(nowDate, Every_Hours);              //设置闹钟时间
    RTC_ITConfig(RTC_Alarm_IRQn, ENABLE);              //使能闹钟
    RTC_ITConfig(RTC_TimeEvent_IRQn, ENABLE);          //设置RTC某一个中断使能
    EnableInterrupts();              //是能中断
    while (1)
    {
        DELAY_MS(200);
        nowDate = RTC_GetTime();
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "%d:%d:%d   \n", nowDate.Year, nowDate.Month, nowDate.DayOfMonth);
        OLED_PrintfAt(FONT_ASCII_6X8, 1, 0, "WeekDay:%d   ", nowDate.DayOfWeek);
        OLED_PrintfAt(FONT_ASCII_6X8, 2, 0, "%d:%d:%d   \n", nowDate.Hours, nowDate.Minutes, nowDate.Seconds);
    }
}
