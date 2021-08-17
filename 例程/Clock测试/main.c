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
 * @date       2016-11-26
 */
#include "include.h"

void main()
{
    //��ı�msp430f5529_system.h����ĺ궨���޸�ʱ��Դ��ʱ�ӷ�Ƶ
    DisableInterrupts();            //��ֹ���ж�
    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    MCLK_OUT();
    SMCLK_OUT();
    ACLK_OUT();                     //ʱ�����
    GPIO_PinRemapConfig(P4, 0, GPIO_Remap_MCLK);             //P4.0�˿���ӳ��ΪMCLK���
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_MCLK);             //P4.7�˿���ӳ��ΪMCLK���
    EnableInterrupts();                //ʹ���ж�
    while (1)
    {

    }
}

