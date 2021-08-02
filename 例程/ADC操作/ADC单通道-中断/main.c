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
float ad;
__interrupt void ADC_IRQ_Handler()
{
    if (ADC_GetITStatus(ADC_CH2) == TRUE)
    {
        ad = ADC_ReadChanelValue(ADC_CH2) * 3.3 / 1023;   //��ȡ����Զ������־λ
        ADC_ClearITPendingBit(ADC_CH2);
    }
}
void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    OLED_Init();

    ADC_Init(ADC_CH2, ADC_VREF_3_3V, ADC_10bit);       //��ʼ��ͨ��,P6.2
    Set_Vector_Handler(VECTOR_ADC12, ADC_IRQ_Handler);
    ADC_ITConfig(ADC_CH2, ENABLE);

    EnableInterrupts();
    while (1)
    {
        ADC_StartConv();
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "ADValue:\n%f V", ad);     //��ָ��λ�ô�ӡ
    }
}

