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

__interrupt void TIMER1_A0_IRQ_Handler()
{
    KEY_20msTimingProcess(KEY1);
    KEY_20msTimingProcess(KEY2);
}
void KEY1_EventHandler(KEY_EVENT event)
{
    switch (event)
    {
    case KEY_CLICK:
        LED_SetStatus(LED1, TRUE);                                     //����LED״̬
        LED_SetStatus(LED2, FALSE);                                    //����LED״̬
        break;
    case KEY_DOUBLE_CLICK:
        LED_SetStatus(LED1, FALSE);                                    //����LED״̬
        LED_SetStatus(LED2, TRUE);                                     //����LED״̬
        break;
    case KEY_LONG_CLICK:
        LED_SetStatus(LED1, TRUE);                                     //����LED״̬
        LED_SetStatus(LED2, TRUE);                                     //����LED״̬
        break;
    default:
        break;
    }
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    KEY_Init(KEY1, KEY1_EventHandler); 	                //������ʼ������
    KEY_Init(KEY2, NULL); 	                //������ʼ������

    Set_Vector_Handler(VECTOR_TIMER_A1_CCR0, TIMER1_A0_IRQ_Handler); //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
    TIMER_Interval_Ms(TIMER_A1, 20);    //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
    TIMER_ITConfig(TIMER_A1, TIMER_CCR0_IRQn, ENABLE);           //ʹ��TIMER��ĳһ���ж�

    EnableInterrupts();
    while (1)
    {
        if (KEY_isPressed(KEY2))
        {
            LED_SetStatus(LED1, FALSE);                                //����LED״̬
            LED_SetStatus(LED2, FALSE);                                //����LED״̬
        }
    }
}
