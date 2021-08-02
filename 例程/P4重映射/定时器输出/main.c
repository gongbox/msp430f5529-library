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

void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED1);              //LED�Ƴ�ʼ��

    //��ʱ��A0��Ƶ��1K
    TIMER_PWM_Init(TIMER_A0, 1000, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH1, 0);    //��ʱ��TA0��ͨ��1��P1.2,ռ�ձ�Ϊ0
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH2, 200); //��ʱ��TA0��ͨ��2��P1.3,ռ�ձ�Ϊ20%
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH3, 700); //��ʱ��TA0��ͨ��3��P1.4,ռ�ձ�Ϊ70%
    TIMER_PWM_SetChannelDuty(TIMER_A0, TIMER_CH4, 1000); //��ʱ��TA0��ͨ��4��P1.5,ռ�ձ�Ϊ100%
    //��ʱ��B0��Ƶ��1HZ
    TIMER_PWM_Init(TIMER_B0, 1, TIMER_CH5, TIMER_CH6);
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 0);   //��ʱ��TB0��ͨ��5��P3.5,ռ�ձ�Ϊ0%
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH6, 500); //��ʱ��TB0��ͨ��6��P3.6,ռ�ձ�Ϊ50%
    GPIO_PinRemapConfig(P4, 0, GPIO_Remap_TB0CCR5A); //GPIO�˿���ӳ��,������ӦTIMER_CH5��ռ�ձ� �����ռ�ձ�Ϊ50%
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_TB0CCR6A);        //GPIO�˿���ӳ��

    while (1)
    {
        DELAY_MS(500);
        LED_Turn(LED1);              //ָʾ����˸
    }
}

