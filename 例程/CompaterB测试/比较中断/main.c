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

__interrupt void COMPATER_IRQ_Handler()
{
    if (CompaterB_GetITStatus(CB_RaisingIRQn))       //�ж��Ƿ���������
    {
        LED_On(LED1);               //ָʾ����˸
        LED_Off(LED2);
        CompaterB_ClearITPendingBit(CB_RaisingIRQn);  //����������жϱ�־
    }
    else if (CompaterB_GetITStatus(CB_FallingIRQn))  //�ж��Ƿ����½���
    {
        LED_On(LED2);               //ָʾ����˸
        LED_Off(LED1);
        CompaterB_ClearITPendingBit(CB_FallingIRQn);  //����½����жϱ�־
    }
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    CompaterB_Init(CB_CH2, CB_VREF_2_0V);   //P6.2
    Set_Vector_Handler(VECTOR_COMP_B, COMPATER_IRQ_Handler);      //�����жϷ�����
    CompaterB_ITConfig(CB_RaisingIRQn, ENABLE);                    //ʹ���������ж�
    CompaterB_ITConfig(CB_FallingIRQn, ENABLE);                    //ʹ���½����ж�

    EnableInterrupts();              //ʹ���ж�
    while (1)
    {
    }
}

