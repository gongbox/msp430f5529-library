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

__interrupt void COMPATER_IRQ_Handler()
{
    if (CompaterB_GetITStatus(CB_RaisingIRQn))       //判断是否是上升沿
    {
        LED_On(LED1);               //指示灯闪烁
        LED_Off(LED2);
        CompaterB_ClearITPendingBit(CB_RaisingIRQn);  //清除上升沿中断标志
    }
    else if (CompaterB_GetITStatus(CB_FallingIRQn))  //判断是否是下降沿
    {
        LED_On(LED2);               //指示灯闪烁
        LED_Off(LED1);
        CompaterB_ClearITPendingBit(CB_FallingIRQn);  //清除下降沿中断标志
    }
}
void main()
{
    DisableInterrupts();            //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化

    CompaterB_Init(CB_CH2, CB_VREF_2_0V);   //P6.2
    Set_Vector_Handler(VECTOR_COMP_B, COMPATER_IRQ_Handler);      //设置中断服务函数
    CompaterB_ITConfig(CB_RaisingIRQn, ENABLE);                    //使能上升沿中断
    CompaterB_ITConfig(CB_FallingIRQn, ENABLE);                    //使能下降沿中断

    EnableInterrupts();              //使能中断
    while (1)
    {
    }
}

