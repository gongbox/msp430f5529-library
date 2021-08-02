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
 * @date       2016-01-30
 */
#include "include.h"

void main()
{
    DisableInterrupts();          //��ֹ���ж�
    GPIO_MultiBits_Init(P1, BIT0, GPO);       //P1.0��Ϊ�������
    GPIO_MultiBits_Init(P1, BIT1 + BIT3, GPI | PULL_UP);  //P1.1,P1.3��Ϊ���뷽��,������
    GPIO_MultiBits_Init(P4, 0XFF, GPO);       //P4����IO����Ϊ�������
    while (1)
    {
        DELAY_MS(500);                 //��ʱ500MS
        if (GPIO_ReadByte(P1) & (BIT1))                 //���P1.1λΪ�ߵ�ƽ
        {
            GPIO_TurnBit(P1, 0);             //��ת���״̬
        }
        else
        {
            GPIO_WriteByte(P4, BIT0 + BIT7);    //P4.0��P4.7���Ϊ�ߵ�ƽ,����λ����͵�ƽ
        }
    }
}

