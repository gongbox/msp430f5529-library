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
    DisableInterrupts();          //禁止总中断

    LED_Init(LED1);              //LED灯初始化

    //定时器A0，频率1K
    TIMER_PWM_Init(TIMER_A0, 1000, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH1, 0);    //定时器TA0，通道1，P1.2,占空比为0
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH2, 200); //定时器TA0，通道2，P1.3,占空比为20%
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH3, 700); //定时器TA0，通道3，P1.4,占空比为70%
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH4, 1000); //定时器TA0，通道4，P1.5,占空比为100%
    //定时器B0，频率1HZ
    TIMER_PWM_Init(TIMER_B0, 1, TIMER_CH5, TIMER_CH6);
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 0);   //定时器TB0，通道5，P3.5,占空比为0%
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH6, 500); //定时器TB0，通道6，P3.6,占空比为50%
    GPIO_PinRemapConfig(P4, 0, GPIO_Remap_TB0CCR5A); //GPIO端口重映射,并不对应TIMER_CH5的占空比 ，输出占空比为50%
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_TB0CCR6A);        //GPIO端口重映射

    while (1)
    {
        DELAY_MS(500);
        LED_Turn(LED1);              //指示灯闪烁
    }
}

