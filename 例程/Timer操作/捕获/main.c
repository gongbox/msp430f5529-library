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

/*
 *ʹ��ʱ�̽�P3.5��P2.0����,�������ֵӦ�ӽ�PWM��Ƶ��
 */
volatile uint32_t PulseWidth = 0;
volatile int16_t overflowTime = 0;
//��ʱ��A1�жϷ�����򣬶�ȡ��������⵽�ľ��� 
__interrupt void TIMER_A1_IRQ_Handler()
{
    static uint16_t start_value = 0, stop_value = 0;
    LED_Turn(LED1);               //ָʾ����˸
    if (TIMER_GetITStatus(TIMER_A1, TIMER_CCR1_IRQn) == TRUE)  //��ȡĳһͨ���жϱ�־
    {
        //�������������
        if (TIMER_GetChannelPinInValue(TIMER_A1, TIMER_CH1) == BIT_SET)
        {
            start_value = TIMER_Capture_GetValue(TIMER_A1, TIMER_CH1); //��ȡ�����ò�������ʱ�ļ���ֵ
            overflowTime = 0;
        }
        //��������½���
        else
        {
            stop_value = TIMER_Capture_GetValue(TIMER_A1, TIMER_CH1); //��ȡ�����ò�������ʱ�ļ���ֵ
            PulseWidth = TIMER_Capture_CalTime_Us(start_value, stop_value,
                                                  overflowTime);  //��ȡ����ʱ�䲢ת��Ϊ����
            if (PulseWidth > 1500)
            {
                TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 1000); //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
            }
            else
            {
                TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 2000); //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
            }
            LowPowerMode_Exit();                           //�˳��͹���ģʽ
        }
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_CCR1_IRQn);    //���TIMER��ĳһ���жϱ�־
    }
    if (TIMER_GetITStatus(TIMER_A1, TIMER_OverFlow_IRQn) == TRUE)     //������
    {
        overflowTime++;          //���ֵ��1
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_OverFlow_IRQn); //���TIMER��ĳһ���жϱ�־
    }
}
void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    //��ʱ��B0��Ƶ��10K
    //TIMER_PWM_MultiChannel_Init      (TIMER_B0,10000, 1 ,TIMER_CH5);   //��ʱ��TB0��ͨ��5��P3.5
    TIMER_PWM_Init(TIMER_B0, 50, TIMER_CH5);   //��ʱ��TB0��ͨ��5��P3.5
    TIMER_PWM_SetChannelDuty(TIMER_B0, TIMER_CH5, 1000);     //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�

    Set_Vector_Handler(VECTOR_TIMER_A1, TIMER_A1_IRQ_Handler);    //���ý����ж�����
    TIMER_Capture_Init(TIMER_A1, TIMER_CH1, CAP_RisingAndFalling); //��ʼ��һ��ͨ��Ϊ����ģʽ�� 
    TIMER_ITConfig(TIMER_A1, TIMER_CCR1_IRQn, ENABLE);       //�����Ƿ�ʹ��TIMER��ĳһ���ж�
    TIMER_ITConfig(TIMER_A1, TIMER_OverFlow_IRQn, ENABLE);   //�����Ƿ�ʹ��TIMER��ĳһ���ж�

    EnableInterrupts();
    while (1)
    {
        LowPowerMode_Enter(LPM_1);              //����͹���ģʽ
        printf("PulseWidth:%ld US\n", PulseWidth);
    }
}

