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
    //请改变msp430f5529_system.h里面的宏定义修改时钟源及时钟分频
    DisableInterrupts();            //禁止总中断
    LED_Init(LED_ALL);              //LED灯初始化
    MCLK_OUT();
    SMCLK_OUT();
    ACLK_OUT();                     //时钟输出
    GPIO_PinRemapConfig(P4, 0, GPIO_Remap_MCLK);             //P4.0端口重映射为MCLK输出
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_MCLK);             //P4.7端口重映射为MCLK输出
    EnableInterrupts();                //使能中断
    while (1)
    {

    }
}

