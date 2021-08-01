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

uint32_t PulseWidth = 0;
int16_t overflowTime = 0;
float distance = 0;
//定时器A1中断服务程序，读取超声波检测到的距离 
__interrupt void TIMER_A1_IRQ_Handler()
{
    static uint16_t start_value = 0, stop_value = 0;
    LED_Turn(LED1);               //指示灯闪烁
    if (TIMER_GetITStatus(TIMER_A1, TIMER_CCR1_IRQn))  //获取某一通道中断标志
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
            distance = PulseWidth / 2 * 340.0 / 1000000;          //读取捕获时间并转化为距离

            LowPowerMode_Exit();                           //退出低功耗模式
            TIMER_Capture_Clear(TIMER_A1, TIMER_CH1);                //清零
            overflowTime = 0;
        }
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_CCR1_IRQn);    //清除TIMER的某一个中断标志
    }
    if (TIMER_GetITStatus(TIMER_A1, TIMER_OverFlow_IRQn))     //如果溢出
    {
        overflowTime++;          //溢出值加1
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_OverFlow_IRQn); //清除TIMER的某一个中断标志
    }
}
//看门狗定时中断
__interrupt void WatchDog_IRQ_Handler()
{
    PIN_OUT(P8,1) = BIT_SET;
    DELAY_US(10);
    PIN_OUT(P8,1) = RESET;
    //LowPowerMode_Exit();                           //退出低功耗模式
}
void main()
{
    DisableInterrupts();          //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化

    //定时中断模块
    GPIO_Init(P8, 1, GPO);                             //初始

    Set_Vector_Handler(VECTOR_WDT, WatchDog_IRQ_Handler);
    WatchDog_Timer_Init(500);  //500MS中断
    WatchDog_ITConfig(ENABLE);                                   //设置看门狗中断是否使能

    Set_Vector_Handler(VECTOR_TIMER_A1, TIMER_A1_IRQ_Handler);    //设置接收中断向量
    TIMER_Capture_Init(TIMER_A1, TIMER_CH1, CAP_RisingAndFalling); //初始化一个通道为捕获模式， 
    TIMER_ITConfig(TIMER_A1, TIMER_CCR1_IRQn, ENABLE);       //设置是否使能TIMER的某一个中断
    TIMER_ITConfig(TIMER_A1, TIMER_OverFlow_IRQn, ENABLE);   //设置是否使能TIMER的某一个中断

    EnableInterrupts();
    while (1)
    {
        LowPowerMode_Enter(LPM_1);              //进入低功耗模式
        printf("distance:%f\n", distance);
    }
}

