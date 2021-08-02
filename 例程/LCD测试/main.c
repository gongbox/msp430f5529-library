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
uint16_t data = 1000;
float fdata = 1.0;
__interrupt void TIMER1_A0_IRQ_Handler()
{
    P4OUT ^= BIT7;
    data++;
    fdata += 0.1;
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�
    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    LCD_INIT();                       //Һ������ʼ����ͨ���궨��ӿڵ���

    Set_Vector_Handler(VECTOR_TIMER1_A0, TIMER1_A0_IRQ_Handler); //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
    TIMER_Interval_Ms(TIMER_A1, 500);    //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
    TIMER_ITConfig(TIMER_A1, TIMER_CCR0_IRQn, ENABLE);           //ʹ��TIMER��ĳһ���ж�

    //LCD��ʾ�ַ����ַ�����������С����һ�������͹���
    LCD_Printf_8x16(0, 0, "A");              //��ʾһ���ַ�
    LCD_Printf_8x16(8, 0, "BCDE!");              //��ʾһ���ַ���
    LCD_Printf_8x16(0, 16, "%d", -100);              //��ʾһ������
    LCD_Printf_8x16(0, 32, "%f", -100.008310);              //��ʾһ��������

    EnableInterrupts();
    while (1)
    {
        LCD_Printf_8x16(0, 48, "data=%d\n fdata=%f", data, fdata);       //��ʾ���� 
        DELAY_MS(100);
    }

}

