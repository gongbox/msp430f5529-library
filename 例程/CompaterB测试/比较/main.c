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

    CompaterB_Init(CB_CH1, CB_VREF_2_0V);   //P6.1
    GPIO_PinRemapConfig(P4, 2, GPIO_Remap_CBOUT0);   //P4.2端口映射为CBOUT0，显示比较结果   
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_CBOUT1);   //P4.7端口映射为CBOUT1，显示比较结果   

    while (1)
    {
        PIN_OUT(P1,0) = CompatorB_GetValue();  //P1.0输出比较结果，P1.0，P4.2，P4.7输出结果相同
    }

}

