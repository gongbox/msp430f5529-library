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

//NMI�жϷ�����
__interrupt void NMI_IRQ_Handler()
{
    if (NMI_GetITStatus() == TRUE)
    {
        P1OUT ^= BIT0;
        NMI_ClearITPendingBit(); //һ��Ҫ����жϱ�־ 
    }
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�
    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    NMI_Init(NMI_Falling);                                          // ����NMI
    Set_Vector_Handler(VECTOR_UNMI, NMI_IRQ_Handler);
    NMI_ITConfig(ENABLE);
    //EnableInterrupts();              //ʹ���ж�
    while (1)
    {
        DELAY_MS(500);
    }
}

