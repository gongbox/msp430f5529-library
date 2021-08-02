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

float ad1, ad3;
__interrupt void ADC_IRQ_Handler()
{
    if (ADC_GetITStatus(ADC_CH1) == TRUE)
    {
        ad1 = ADC_ReadChanelValue(ADC_CH1) * 3.3 / 1023;   //��ȡ����Զ������־λ
        ADC_ClearITPendingBit(ADC_CH1);
    }
    if (ADC_GetITStatus(ADC_CH3) == TRUE)
    {
        ad3 = ADC_ReadChanelValue(ADC_CH3) * 3.3 / 1023;
        ADC_ClearITPendingBit(ADC_CH3);
    }
}
void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    OLED_Init();

    ADC_Init(ADC_CH1 + ADC_CH3, ADC_VREF_3_3V, ADC_10bit);     //��ʼ��ͨ��,P6.1,P6.3
    Set_Vector_Handler(VECTOR_ADC12, ADC_IRQ_Handler);
    ADC_ITConfig(ADC_CH1, ENABLE);
    ADC_ITConfig(ADC_CH3, ENABLE);
    EnableInterrupts();
    while (1)
    {
        ADC_StartConv();                                              //����һ��ת��
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "ADValue:\n%f V\n%f V", ad1, ad3); //��ʾһ������
        DELAY_MS(500);
    }
}

