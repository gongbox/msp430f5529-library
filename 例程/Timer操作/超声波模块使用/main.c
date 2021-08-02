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

uint32_t PulseWidth = 0;
int16_t overflowTime = 0;
float distance = 0;
//��ʱ��A1�жϷ�����򣬶�ȡ��������⵽�ľ��� 
__interrupt void TIMER_A1_IRQ_Handler()
{
    static uint16_t start_value = 0, stop_value = 0;
    LED_Turn(LED1);               //ָʾ����˸
    if (TIMER_GetITStatus(TIMER_A1, TIMER_CCR1_IRQn))  //��ȡĳһͨ���жϱ�־
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
            distance = PulseWidth / 2 * 340.0 / 1000000;          //��ȡ����ʱ�䲢ת��Ϊ����

            LowPowerMode_Exit();                           //�˳��͹���ģʽ
            TIMER_Capture_Clear(TIMER_A1, TIMER_CH1);                //����
            overflowTime = 0;
        }
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_CCR1_IRQn);    //���TIMER��ĳһ���жϱ�־
    }
    if (TIMER_GetITStatus(TIMER_A1, TIMER_OverFlow_IRQn))     //������
    {
        overflowTime++;          //���ֵ��1
        TIMER_ClearITPendingBit(TIMER_A1, TIMER_OverFlow_IRQn); //���TIMER��ĳһ���жϱ�־
    }
}
//���Ź���ʱ�ж�
__interrupt void WatchDog_IRQ_Handler()
{
    PIN_OUT(P8,1) = BIT_SET;
    DELAY_US(10);
    PIN_OUT(P8,1) = RESET;
    //LowPowerMode_Exit();                           //�˳��͹���ģʽ
}
void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    //��ʱ�ж�ģ��
    GPIO_Init(P8, 1, GPO);                             //��ʼ

    Set_Vector_Handler(VECTOR_WDT, WatchDog_IRQ_Handler);
    WatchDog_Timer_Init(500);  //500MS�ж�
    WatchDog_ITConfig(ENABLE);                                   //���ÿ��Ź��ж��Ƿ�ʹ��

    Set_Vector_Handler(VECTOR_TIMER_A1, TIMER_A1_IRQ_Handler);    //���ý����ж�����
    TIMER_Capture_Init(TIMER_A1, TIMER_CH1, CAP_RisingAndFalling); //��ʼ��һ��ͨ��Ϊ����ģʽ�� 
    TIMER_ITConfig(TIMER_A1, TIMER_CCR1_IRQn, ENABLE);       //�����Ƿ�ʹ��TIMER��ĳһ���ж�
    TIMER_ITConfig(TIMER_A1, TIMER_OverFlow_IRQn, ENABLE);   //�����Ƿ�ʹ��TIMER��ĳһ���ж�

    EnableInterrupts();
    while (1)
    {
        LowPowerMode_Enter(LPM_1);              //����͹���ģʽ
        printf("distance:%f\n", distance);
    }
}

