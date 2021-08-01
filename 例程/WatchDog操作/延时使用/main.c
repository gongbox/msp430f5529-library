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

void main()
{
    DisableInterrupts();            //禁止总中断
    LED_Init(LED_ALL);              //LED灯初始化
    EnableInterrupts();              //是能中断
    while (1)
    {
        WatchDog_Delay_Ms(500);  //500MS，看门狗延时
        LED_Turn(LED1);               //指示灯闪烁
    }
}
