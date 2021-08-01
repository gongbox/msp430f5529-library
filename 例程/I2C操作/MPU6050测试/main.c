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

char data = 0;
__interrupt void I2C_IRQ_Handler()
{
    if (I2C_GetITStatus(I2C0, I2C_NACK_IRQn) == TRUE)   //清除串口某一个中断标志
    {
        OLED_PrintfAt(FONT_ASCII_6X8, 0, 0, "%s", "NO ACK!"); //显示无应答错误，可能是从机地址不匹配
        I2C_ClearITPendingBit(I2C0, I2C_NACK_IRQn);    //清除串口某一个中断标志
    }
}
void main()
{
    DisableInterrupts();            //禁止总中断

    CLOCK_DCO_PLLConfig(FLLREF_REFO, FLLREF_DIV_1, 16MHZ);   //DCO_FLL倍频环设置
    LED_Init(LED_ALL);              //LED灯初始化
    OLED_Init();                    //OLED初始化

    Set_Vector_Handler(VECTOR_USCI_B0, I2C_IRQ_Handler);    //设置接收中断向量

    I2C_MasterInit(I2C0, 0x68, 100KHZ);                     //初始化I2C模块（主机模式）
    I2C_ITConfig(I2C0, I2C_NACK_IRQn, ENABLE);            //使能I2C的某一个中断
    EnableInterrupts();
    while (1)
    {
        I2C_WriteReg(I2C0, 0x24, 14);            //新修改值
        OLED_DispDec(FONT_ASCII_6X8, 2, 0, I2C_ReadReg(I2C0, 0x24), 3); //显示接收到的数据
        DELAY_MS(500);
    }
}

