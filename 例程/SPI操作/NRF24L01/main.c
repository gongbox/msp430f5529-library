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

#include "nrf2401.h"
//�궨�巢�Ͷ˻��ǽ��ն�
//#define TX_MODE
BOOL NRF24L01_ReceiveFLAG = FALSE;
uint8_t nrf24l01_data[256];

__interrupt void GPIO_IRQ_Handler()
{
    if (GPIO_GetITStatus(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN) == TRUE)
    {
        NRF2401_IRQ_Handler();   //IRQ�ж�����������жϷ�����
        GPIO_ClearITPendingBit(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN);     //���λ�жϱ�־
    }
}
//��ʼ��IRQ���ţ�����Ϊ�������ж�
void IRQ_Init()
{
    Set_Vector_Handler(VECTOR_PORT2, GPIO_IRQ_Handler);
    GPIO_Init(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN, GPI | IRQ_RISING);
    GPIO_ITConfig(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN, ENABLE);
}
//�����ж�����ò�Ҫ�������£����߻ᶪʧ����
void RecevieData()
{
    NRF24L01_ReceiveFLAG = TRUE;
}
#include "stdarg.h"
void NRF24L01_Printf(const char *fmt, ...)
{
    char String[128];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(String, fmt, ap);
    NRF2401_SendData(String);
    va_end(ap);
}
void main()
{
    DisableInterrupts();          //��ֹ���ж�

    LED_Init(LED_ALL);              //LED�Ƴ�ʼ��

    s_NRF24L01 nrf24l01;
    nrf24l01.CallBack_IRQ_Init = IRQ_Init;            //����IRQ���ų�ʼ���ص�����
    nrf24l01.pReceiveData = nrf24l01_data;            //���ý���������ʼ��ַ
    nrf24l01.CallBack_RecevieData = RecevieData;      //���ý�������жϻص�����
    nrf24l01.CallBack_SendDataOk = NULL;              //���÷��ͳɹ��ص�����
    nrf24l01.CallBack_SendOutTime = NULL;             //���÷��ͳ�ʱ�ص�����

    uint8_t status = NRF2401_Init(nrf24l01);             //��ʼ��
    LED_SetStatus(LED1, (status == NRF_OK) ? BIT_SET : RESET);     //���24L01�Ƿ����
#ifdef TX_MODE
    NRF2401_SetTXMode();
#else
    NRF2401_SetRXMode();
#endif
    EnableInterrupts();
    while (1)
    {
#ifdef TX_MODE
    DELAY_MS(100);
    static uint16 num = 0;
    NRF24L01_Printf("Hello!-%d",num++);
#else
        if (NRF24L01_ReceiveFLAG == TRUE)
        {
            NRF24L01_ReceiveFLAG = FALSE;
            printf("%s\n", nrf24l01_data);
        }
#endif
    }
}
