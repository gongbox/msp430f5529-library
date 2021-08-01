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

__interrupt void TIMER1_A0_IRQ_Handler()
{
    KEY_20msTimingProcess(KEY1);
    KEY_20msTimingProcess(KEY2);
}
void KEY1_EventHandler(KEY_EVENT event)
{
    switch (event)
    {
    case KEY_CLICK:
        LED_SetStatus(LED1, TRUE);                                     //设置LED状态
        LED_SetStatus(LED2, FALSE);                                    //设置LED状态
        break;
    case KEY_DOUBLE_CLICK:
        LED_SetStatus(LED1, FALSE);                                    //设置LED状态
        LED_SetStatus(LED2, TRUE);                                     //设置LED状态
        break;
    case KEY_LONG_CLICK:
        LED_SetStatus(LED1, TRUE);                                     //设置LED状态
        LED_SetStatus(LED2, TRUE);                                     //设置LED状态
        break;
    default:
        break;
    }
}
void main()
{
    DisableInterrupts();            //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化
    KEY_Init(KEY1, KEY1_EventHandler); 	                //按键初始化函数
    KEY_Init(KEY2, NULL); 	                //按键初始化函数

    Set_Vector_Handler(VECTOR_TIMER_A1_CCR0, TIMER1_A0_IRQ_Handler); //设置定时器TA1的中断服务程序为：TIMER1_A0_IRQ_Handler
    TIMER_Interval_Ms(TIMER_A1, 20);    //设置TA1为定时中断，中断时间为500Ms
    TIMER_ITConfig(TIMER_A1, TIMER_CCR0_IRQn, ENABLE);           //使能TIMER的某一个中断

    EnableInterrupts();
    while (1)
    {
        if (KEY_isPressed(KEY2))
        {
            LED_SetStatus(LED1, FALSE);                                //设置LED状态
            LED_SetStatus(LED2, FALSE);                                //设置LED状态
        }
    }
}
