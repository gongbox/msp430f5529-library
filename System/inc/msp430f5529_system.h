#ifndef SYSTEM_H_
#define SYSTEM_H_

//宏定义初始化时钟频率及时钟源
#define FLL_REF         FLLREF_REFO     //FLL参考时钟源
#define FLLREF_DIV      FLLREF_DIV_1    //FLL参考时钟分频系数
#define DCO_CLK_HZ      25MHZ            //DCO时钟频率
//主时钟设置
#define MCLK_SOURCE     CLOCK_DCO       //主时钟时钟源
#define MCLK_DIV        CLOCK_DIV_1     //主时钟时钟分频系数
//系统时钟设置
#define SMCLK_SOURCE    CLOCK_XT2       //系统时钟时钟源
#define SMCLK_DIV       CLOCK_DIV_1     //系统时钟分频系数
//辅助时钟设置
#define ACLK_SOURCE     CLOCK_REFO      //辅助时钟时钟源
#define ACLK_DIV        CLOCK_DIV_1     //辅助时钟分频系数

/*
 * 配置延时函数
 */
//宏定义延时使用的定时器，注释该宏定义则使用软件延时，
//#define DELAY_TIMER      TIMER_B0

#if(defined(DELAY_TIMER))
#include "timer.h"
#define DELAY_MS(ms)     TIMER_Delay_Ms(DELAY_TIMER,ms)
#define DELAY_US(us)     TIMER_Delay_Us(DELAY_TIMER,us)
#else
#include "delay_help.h"
//判断主时钟源是否为DCO或DCO_DIV，如果是则使用精确延时函数
#if(TOVALUE(MCLK_SOURCE) == TOVALUE(CLOCK_DCO) || TOVALUE(MCLK_SOURCE) == TOVALUE(CLOCK_DCO_DIV))
#include "intrinsics.h"   
#define CPU_FRE          (DCO_CLK_HZ/TOVALUE(MCLK_DIV)) //宏定义主时钟频率
#define DELAY_MS(ms)     __delay_cycles((long)((double)ms*CPU_FRE/1000.0))
#define DELAY_US(us)     __delay_cycles((long)((double)us*CPU_FRE/1000000.0))
#else
#include "delay.h"
#define DELAY_MS(ms)     delay_ms(ms)
#define DELAY_US(us)     delay_us(us)
#endif
#endif
/*
 * 配置调试输出函数
 */
//宏定义Print函数使用的UART模块
//#define DEBUG_UART_PRINT               //宏定义是否使用串口打印作为printf函数打印，注释该宏定义则使用Termite IO作为显示
#define PRINT_UART       UART0          //宏定义默认打印串口模块号          
#define PRINT_BAUD       115200         //宏定义默认打印串口波特率 

#if(defined(DEBUG) && defined(DEBUG_UART_PRINT))
#include "uart.h"
#ifndef PRINT_UART
#error 没有宏定义"PRINT_UART",请在system.h里宏定义，如#define PRINT_UART  UART0
#endif
#ifndef PRINT_BAUD
#error 没有宏定义"PRINT_BAUD",请在system.h里宏定义，如#define PRINT_BAUD  115200
#endif
#endif
#if (defined(DEBUG))
#include "stdio.h"
#define DEBUG_PRINTF(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)	/*无需打印调试信息时，请将宏内容注释掉*/
#else
#define DEBUG_PRINTF(FORMAT,...)
#endif

//总中断禁止与使能宏定义
#define DisableInterrupts       __disable_interrupt
#define EnableInterrupts        __enable_interrupt

#endif