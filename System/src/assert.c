/********************                                        
 * 文件名       ：assert.c
 * 描述         ：断言条件不成立的执行函数
 *
 * 实验平台     ：MSP430F5529
 * 库版本       ：
 * 嵌入系统     ：
 * 备    注     ：
 *
 * 作者         ：
**********************************************************************************/

/*
 * File:        assert.c
 * Purpose:     Provide macro for software assertions
 *
 * Notes:       ASSERT macro defined in assert.h calls assert_failed()
 */

#include "common.h"
#include "LED.h"
#include "msp430f5529_system.h"

const char ASSERT_FAILED_STR[] = "断言条件不成立，请检测错误原因：\n断言发生在%s文件的第%d行！\n断言所在函数：%s\n断言失败说明：%s\t—— by CTGU-GB\n\n";
/********************************************************************/
//如果断言条件不成立，进入了错误状态，就会打印错误信息和用LED来显示状态
void assert_failed(char *file, int line,char *funName,char *str)
{
#ifdef DEBUG
#ifdef DEBUG_UART_PRINT
    //启动函数中已经初始化过串口，主函数中若修改了SMCLK频率则需再设置波特率，以便从新设置波特率
    UART_SetBaudRate (Print_UART,Print_Baud);                    
#endif
    LED_Init(LED_ALL);                        //LED灯初始化
    while (1)
    {
        DEBUG_PRINTF(ASSERT_FAILED_STR, file, line,funName,str);  //打印错误信息
        LED_Water_Lights();                               //用流水灯来指示进入错误状态
    }
#endif
}
/********************************************************************/
