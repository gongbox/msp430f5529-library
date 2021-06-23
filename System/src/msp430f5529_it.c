#include "msp430f5529_it.h"
#include "LED.h"
#include "include.h"
/*******************************************************************************
*  函数名称：default_isr(void)
*  功能说明：默认的中断函数，如果某一个中断开启，且设置了中断向量表映射到RAM
             但没调用Set_Vector_Handler(VECTORn vector,__interrupt void pfunc_handler(void))配置中断服务函数，
             发生中断时会进入这个默认中断服务函数，在这个中断服务函数里用指示灯指示中断进入错误
             并用串口打印错误信息
*  参数说明：无
*  函数返回：无
*  使用示例：无
********************************************************************************/
__interrupt void default_isr(void)
{
#ifdef DEBUG
#ifdef DEBUG_UART_PRINT
    //启动函数中已经初始化过串口，主函数中若修改了SMCLK频率则需再设置波特率，以便从新设置波特率
    UART_SetBaudRate (Print_UART,Print_Baud);                    
#endif
    LED_Init(LED_ALL);                        //LED灯初始化
    //while (1)  //这里取消死循环
    {
       DEBUG_PRINTF("\n有未知中断进入，请检查是否有中断使能却未配置中断服务函数！\n");  //打印错误信息
       LED_Water_Lights();                                         //用流水灯来指示进入错误状态
    }
#endif
}