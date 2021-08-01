/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
 * @brief      MSP430F5529 平台主程序

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

#include "nrf2401.h"

volatile BOOL NRF24L01_ReceiveFLAG = FALSE, NRF24L01_SendOkFLAG = FALSE;
volatile uint8_t uart_data[256];
volatile uint8_t nrf24l01_data[256];
volatile uint16_t uart_data_num = 0;
__interrupt void UART_RX_IRQ_Handler()
{
    if (UART_GetITStatus(UART1, UART_RX_IRQn) == TRUE)   //清除串口某一个中断标志
    {
        //传输速率高时，接收中断里不要做其他事件，否则会丢失数据，另外主时钟也要设高一点
        uart_data[uart_data_num] = UART_GetChar(UART1);             //读取一个字节1个字节
        uart_data_num++;
        UART_ClearITPendingBit(UART1, UART_RX_IRQn);    //清除串口某一个中断标志
    }
}
__interrupt void GPIO_IRQ_Handler()
{
    if (GPIO_GetITStatus(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN) == TRUE)
    {
        NRF2401_IRQ_Handler();
        GPIO_ClearITPendingBit(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN);     //清除位中断标志
    }
}
void IRQ_Init()
{
    Set_Vector_Handler(VECTOR_PORT2, GPIO_IRQ_Handler);
    GPIO_Init(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN, GPI | IRQ_RISING);
    GPIO_ITConfig(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN, ENABLE);
}
//接收中断中最好不要做其他事，否者会丢失数据
void RecevieData()
{
    NRF24L01_ReceiveFLAG = TRUE;
}
void SendDataOK()
{
    NRF24L01_SendOkFLAG = TRUE;
}
void main()
{
    DisableInterrupts();          //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化

    UART_Init(UART1, 115200);             //初始化UART1模块,波特率115200，波特率高时最好把主时钟 设高些
    Set_Vector_Handler(VECTOR_UART1, UART_RX_IRQ_Handler);    //设置接收中断向量
    UART_ITConfig(UART1, UART_RX_IRQn, TRUE);                //开串口接收中断

    s_NRF24L01 nrf24l01;
    nrf24l01.CallBack_IRQ_Init = IRQ_Init;
    nrf24l01.pReceiveData = nrf24l01_data;
    nrf24l01.CallBack_RecevieData = RecevieData;
    nrf24l01.CallBack_SendDataOk = SendDataOK;
    nrf24l01.CallBack_SendOutTime = NULL;

    uint8_t status = NRF2401_Init(nrf24l01);                //初始化
    LED_SetStatus(LED1, (status == NRF_OK) ? BIT_SET : RESET);     //检查24L01是否存在
    NRF2401_SetRXMode();

    EnableInterrupts();
    while (1)
    {
        if (NRF24L01_ReceiveFLAG == TRUE)
        {
            NRF24L01_ReceiveFLAG = FALSE;
            UART_Printf(UART1, "%s\n", nrf24l01_data);
        }
        if (uart_data_num > 0)
        {
            NRF2401_SetTXMode();
            for (int i = 0; i < uart_data_num; i += 32)
            {
                NRF2401_SendData((uint8_t*) &uart_data[i]);
            }
            while (NRF24L01_SendOkFLAG == FALSE)
                ;
            NRF24L01_SendOkFLAG = FALSE;
            NRF2401_SetRXMode();
            uart_data_num = 0;
        }
    }
}
