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
    DisableInterrupts();            //��ֹ���ж�
    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    CompaterB_Init(CB_CH1, CB_VREF_2_0V);   //P6.1
    GPIO_PinRemapConfig(P4, 2, GPIO_Remap_CBOUT0);   //P4.2�˿�ӳ��ΪCBOUT0����ʾ�ȽϽ��   
    GPIO_PinRemapConfig(P4, 7, GPIO_Remap_CBOUT1);   //P4.7�˿�ӳ��ΪCBOUT1����ʾ�ȽϽ��   

    while (1)
    {
        PIN_OUT(P1,0) = CompatorB_GetValue();  //P1.0����ȽϽ����P1.0��P4.2��P4.7��������ͬ
    }

}

