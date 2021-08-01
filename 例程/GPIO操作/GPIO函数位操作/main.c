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
 * @date       2016-01-30
 */
#include "include.h"

void main()
{
    DisableInterrupts();          //禁止总中断
    GPIO_Init(P1, 1, GPI | PULL_UP); //设为输入方向,上拉
    GPIO_Init(P1, 0, GPO);       //设为输出方向
    GPIO_Init(P4, 7, GPO);       //设为输出方向
    while (1)
    {
        DELAY_MS(500);                 //延时500MS
        GPIO_TurnBit(P1, 0);             //翻转输出状态
        GPIO_WriteBit(P4, 7, GPIO_ReadBit(P1, 1));   //读取电平，并输出
    }
}

