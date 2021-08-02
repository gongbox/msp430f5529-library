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

char data = 0;
__interrupt void I2C_IRQ_Handler()
{
    if (I2C_GetITStatus(I2C0, I2C_NACK_IRQn) == TRUE)   //�������ĳһ���жϱ�־
    {
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "%s", "NO ACK!"); //��ʾ��Ӧ����󣬿����Ǵӻ���ַ��ƥ��
        I2C_ClearITPendingBit(I2C0, I2C_NACK_IRQn);    //�������ĳһ���жϱ�־
    }
}
void main()
{
    DisableInterrupts();            //��ֹ���ж�

    CLOCK_DCO_PLLConfig(FLLREF_REFO, FLLREF_DIV_1, 16MHZ);   //DCO_FLL��Ƶ������
    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
    OLED_Init();                    //OLED��ʼ��

    Set_Vector_Handler(VECTOR_USCI_B0, I2C_IRQ_Handler);    //���ý����ж�����

    I2C_MasterInit(I2C0, 0x68, 100KHZ);                     //��ʼ��I2Cģ�飨����ģʽ��
    I2C_ITConfig(I2C0, I2C_NACK_IRQn, ENABLE);            //ʹ��I2C��ĳһ���ж�
    EnableInterrupts();
    while (1)
    {
        I2C_WriteReg(I2C0, 0x24, 14);            //���޸�ֵ
        OLED_DispDec(FONT_ASCII_6X8, 2, 0, I2C_ReadReg(I2C0, 0x24), 3); //��ʾ���յ�������
        DELAY_MS(500);
    }
}

