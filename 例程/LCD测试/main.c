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
    DisableInterrupts();            //禁止总中断
    LED_Init(LED_ALL);              //LED灯初始化

    LCD_INIT();                       //液晶屏初始化，通过宏定义接口调用

    Set_Vector_Handler(VECTOR_TIMER1_A0, TIMER1_A0_IRQ_Handler); //设置定时器TA1的中断服务程序为：TIMER1_A0_IRQ_Handler
    TIMER_Interval_Ms(TIMER_A1, 500);    //设置TA1为定时中断，中断时间为500Ms
    TIMER_ITConfig(TIMER_A1, TIMER_CCR0_IRQn, ENABLE);           //使能TIMER的某一个中断

    //LCD显示字符，字符串，整数，小数，一个函数就够了
    LCD_Printf_8x16(0, 0, "A");              //显示一个字符
    LCD_Printf_8x16(8, 0, "BCDE!");              //显示一个字符串
    LCD_Printf_8x16(0, 16, "%d", -100);              //显示一个整数
    LCD_Printf_8x16(0, 32, "%f", -100.008310);              //显示一个浮点数

    EnableInterrupts();
    while (1)
    {
        LCD_Printf_8x16(0, 48, "data=%d\n fdata=%f", data, fdata);       //显示更新 
        DELAY_MS(100);
    }

}

