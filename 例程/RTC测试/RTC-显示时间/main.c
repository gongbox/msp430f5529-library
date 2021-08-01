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
Calendar nowDate;
void main()
{
    DisableInterrupts();            //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化
    OLED_Init();                       //OLED初始化

    nowDate.Seconds = 56;
    nowDate.Hours = 1;
    nowDate.DayOfMonth = 1;
    nowDate.DayOfWeek = 3;
    nowDate.Year = 2015;
    nowDate.Month = 10;
    nowDate.Minutes = 40;
    RTC_Init(nowDate);

    EnableInterrupts();              //是能中断
    while (1)
    {
        DELAY_MS(500);
        nowDate = RTC_GetTime();
#if 0
     DEBUG_PRINTF("%d:%d:%d\n",nowDate.Year,nowDate.Month,nowDate.DayOfMonth);
     DEBUG_PRINTF("WeekDay:%d\n",nowDate.DayOfWeek);
     DEBUG_PRINTF("%d:%d:%d\n",nowDate.Hours,nowDate.Minutes,nowDate.Seconds);
#else
     OLED_PrintfAt(FONT_ASCII_6X8,0,0,"%d:%d:%d     \n",nowDate.Year,nowDate.Month,nowDate.DayOfMonth);
     OLED_PrintfAt(FONT_ASCII_6X8,1,0,"WeekDay:%d     ",nowDate.DayOfWeek);
     OLED_PrintfAt(FONT_ASCII_6X8,2,0,"%d:%d:%d     \n",(uint16_t)nowDate.Hours,(uint16_t)nowDate.Minutes,(uint16_t)nowDate.Seconds);
#endif
    }
}
