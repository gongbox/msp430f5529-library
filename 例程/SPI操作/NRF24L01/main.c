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
//宏定义发送端还是接收端
//#define TX_MODE
BOOL NRF24L01_ReceiveFLAG = FALSE;
uint8_t nrf24l01_data[256];

__interrupt void GPIO_IRQ_Handler()
{
    if (GPIO_GetITStatus(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN) == TRUE)
    {
        NRF2401_IRQ_Handler();   //IRQ中断引脚中添加中断服务函数
        GPIO_ClearITPendingBit(NRF2401_IRQ_PORT, NRF2401_IRQ_PIN);     //清除位中断标志
    }
}
//初始化IRQ引脚，配置为上升沿中断
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
    DisableInterrupts();          //禁止总中断

    LED_Init(LED_ALL);              //LED灯初始化

    s_NRF24L01 nrf24l01;
    nrf24l01.CallBack_IRQ_Init = IRQ_Init;            //配置IRQ引脚初始化回调函数
    nrf24l01.pReceiveData = nrf24l01_data;            //配置接收数据起始地址
    nrf24l01.CallBack_RecevieData = RecevieData;      //配置接收完成中断回调函数
    nrf24l01.CallBack_SendDataOk = NULL;              //配置发送成功回调函数
    nrf24l01.CallBack_SendOutTime = NULL;             //配置发送超时回调函数

    uint8_t status = NRF2401_Init(nrf24l01);             //初始化
    LED_SetStatus(LED1, (status == NRF_OK) ? BIT_SET : RESET);     //检查24L01是否存在
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
