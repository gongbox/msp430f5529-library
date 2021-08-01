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

//NMI中断服务函数
__interrupt void NMI_IRQ_Handler()
{
    if (NMI_GetITStatus() == TRUE)
    {
        P1OUT ^= BIT0;
        NMI_ClearITPendingBit(); //一定要清除中断标志 
    }
}
void main()
{
    DisableInterrupts();            //禁止总中断
    LED_Init(LED_ALL);              //LED灯初始化

    NMI_Init(NMI_Falling);                                          // 配置NMI
    Set_Vector_Handler(VECTOR_UNMI, NMI_IRQ_Handler);
    NMI_ITConfig(ENABLE);
    //EnableInterrupts();              //使能中断
    while (1)
    {
        DELAY_MS(500);
    }
}

