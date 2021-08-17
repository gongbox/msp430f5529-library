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
    GPIO_Init(P1, 1, GPI | PULL_UP); //��Ϊ���뷽��,����
    GPIO_Init(P1, 0, GPO);       //��Ϊ�������
    GPIO_Init(P4, 7, GPO);       //��Ϊ�������
    while (1)
    {
        DELAY_MS(500);                 //��ʱ500MS
        GPIO_TurnBit(P1, 0);             //��ת���״̬
        GPIO_WriteBit(P4, 7, GPIO_ReadBit(P1, 1));   //��ȡ��ƽ�������
    }
}

