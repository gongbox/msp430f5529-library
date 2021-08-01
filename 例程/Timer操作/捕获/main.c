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

/*
 *使用时短接P3.5和P2.0即可,输出脉冲值应接近PWM波频率
 */
volatile uint32_t PulseWidth = 0;
volatile int16_t overflowTime = 0;
//定时器A1中断服务程序，读取超声波检测到的距离 
__interrupt void TIMER_A1_IRQ_Handler()
{
    static uint16_t start_value = 0, stop_value = 0;
    LED_Turn(LED1);               //指示灯闪烁
    if (TIMER_GetITStatus(TIMER_A1, TIMER_CCR1_IRQn) == TRUE)  //获取某一通道中断标志
    {
        //如果捕获到上升沿
        if (TIMER_GetChannelPinInValue(TIMER_A1, TIMER_CH1) == BIT_SET)
        {
            start_value = TIMER_Capture_GetValue(TIMER_A1, TIMER_CH1); //读取发生该捕获条件时的计数值
            overflowTime = 0;
        }
        //如果捕获到下降沿
        else
        {
            stop_value = TIMER_Capture_GetValue(TIMER_A1, TIMER_CH1); //读取发生该捕获条件时的计数值
            PulseWidth = TIMER_Capture_CalTime_Us(start_value, stop_value,
                                                  overflowTime);  //读取捕获时间并转化为距离
            if (PulseWidth > 1500)
            {
                TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 1000); //设置某一个定时器某一个通道的占空比
            }
            else
            {
                TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 2000); //设置某一个定时器某一个通道的占空比
            }
            LowPowerMode_Exit();                           //退出低功耗模式
        }
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_CCR1_IRQn);    //清除TIMER的某一个中断标志
    }
    if (TIMER_GetITStatus(TIMER_A1, TIMER_OverFlow_IRQn) == TRUE)     //如果溢出
    {
        overflowTime++;          //溢出值加1
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_OverFlow_IRQn); //清除TIMER的某一个中断标志
    }
}
void main()
{
    DisableInterrupts();          //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化

    //定时器B0，频率10K
    //TIMER_PWM_MultiChannel_Init      (TIMER_B0,10000, 1 ,TIMER_CH5);   //定时器TB0，通道5，P3.5
    TIMER_PWM_Init(TIMER_B0, 50, TIMER_CH5);   //定时器TB0，通道5，P3.5
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 1000);     //设置某一个定时器某一个通道的占空比

    Set_Vector_Handler(VECTOR_TIMER_A1, TIMER_A1_IRQ_Handler);    //设置接收中断向量
    TIMER_Capture_Init(TIMER_A1, TIMER_CH1, CAP_RisingAndFalling); //初始化一个通道为捕获模式， 
    TIMER_ITConfig(TIMER_A1, TIMER_CCR1_IRQn, ENABLE);       //设置是否使能TIMER的某一个中断
    TIMER_ITConfig(TIMER_A1, TIMER_OverFlow_IRQn, ENABLE);   //设置是否使能TIMER的某一个中断

    EnableInterrupts();
    while (1)
    {
        LowPowerMode_Enter(LPM_1);              //进入低功耗模式
        printf("PulseWidth:%ld US\n", PulseWidth);
    }
}

