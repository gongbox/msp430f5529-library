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
    GPIO_MultiBits_Init(P1, BIT0, GPO);       //P1.0设为输出方向
    GPIO_MultiBits_Init(P1, BIT1 + BIT3, GPI | PULL_UP);  //P1.1,P1.3设为输入方向,且上拉
    GPIO_MultiBits_Init(P4, 0XFF, GPO);       //P4所有IO口设为输出方向
    while (1)
    {
        DELAY_MS(500);                 //延时500MS
        if (GPIO_ReadByte(P1) & (BIT1))                 //如果P1.1位为高电平
        {
            GPIO_TurnBit(P1, 0);             //翻转输出状态
        }
        else
        {
            GPIO_WriteByte(P4, BIT0 + BIT7);    //P4.0，P4.7输出为高电平,其他位输出低电平
        }
    }
}

