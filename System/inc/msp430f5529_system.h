#ifndef SYSTEM_H_
#define SYSTEM_H_

//�궨���ʼ��ʱ��Ƶ�ʼ�ʱ��Դ
#define FLL_REF         FLLREF_REFO     //FLL�ο�ʱ��Դ
#define FLLREF_DIV      FLLREF_DIV_1    //FLL�ο�ʱ�ӷ�Ƶϵ��
#define DCO_CLK_HZ      25MHZ            //DCOʱ��Ƶ��
//��ʱ������
#define MCLK_SOURCE     CLOCK_DCO       //��ʱ��ʱ��Դ
#define MCLK_DIV        CLOCK_DIV_1     //��ʱ��ʱ�ӷ�Ƶϵ��
//ϵͳʱ������
#define SMCLK_SOURCE    CLOCK_XT2       //ϵͳʱ��ʱ��Դ
#define SMCLK_DIV       CLOCK_DIV_1     //ϵͳʱ�ӷ�Ƶϵ��
//����ʱ������
#define ACLK_SOURCE     CLOCK_REFO      //����ʱ��ʱ��Դ
#define ACLK_DIV        CLOCK_DIV_1     //����ʱ�ӷ�Ƶϵ��

/*
 * ������ʱ����
 */
//�궨����ʱʹ�õĶ�ʱ����ע�͸ú궨����ʹ�������ʱ��
//#define DELAY_TIMER      TIMER_B0

#if(defined(DELAY_TIMER))
#include "timer.h"
#define DELAY_MS(ms)     TIMER_Delay_Ms(DELAY_TIMER,ms)
#define DELAY_US(us)     TIMER_Delay_Us(DELAY_TIMER,us)
#else
#include "delay_help.h"
//�ж���ʱ��Դ�Ƿ�ΪDCO��DCO_DIV���������ʹ�þ�ȷ��ʱ����
#if(TOVALUE(MCLK_SOURCE) == TOVALUE(CLOCK_DCO) || TOVALUE(MCLK_SOURCE) == TOVALUE(CLOCK_DCO_DIV))
#include "intrinsics.h"   
#define CPU_FRE          (DCO_CLK_HZ/TOVALUE(MCLK_DIV)) //�궨����ʱ��Ƶ��
#define DELAY_MS(ms)     __delay_cycles((long)((double)ms*CPU_FRE/1000.0))
#define DELAY_US(us)     __delay_cycles((long)((double)us*CPU_FRE/1000000.0))
#else
#include "delay.h"
#define DELAY_MS(ms)     delay_ms(ms)
#define DELAY_US(us)     delay_us(us)
#endif
#endif
/*
 * ���õ����������
 */
//�궨��Print����ʹ�õ�UARTģ��
//#define DEBUG_UART_PRINT               //�궨���Ƿ�ʹ�ô��ڴ�ӡ��Ϊprintf������ӡ��ע�͸ú궨����ʹ��Termite IO��Ϊ��ʾ
#define PRINT_UART       UART0          //�궨��Ĭ�ϴ�ӡ����ģ���          
#define PRINT_BAUD       115200         //�궨��Ĭ�ϴ�ӡ���ڲ����� 

#if(defined(DEBUG) && defined(DEBUG_UART_PRINT))
#include "uart.h"
#ifndef PRINT_UART
#error û�к궨��"PRINT_UART",����system.h��궨�壬��#define PRINT_UART  UART0
#endif
#ifndef PRINT_BAUD
#error û�к궨��"PRINT_BAUD",����system.h��궨�壬��#define PRINT_BAUD  115200
#endif
#endif
#if (defined(DEBUG))
#include "stdio.h"
#define DEBUG_PRINTF(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)	/*�����ӡ������Ϣʱ���뽫������ע�͵�*/
#else
#define DEBUG_PRINTF(FORMAT,...)
#endif

//���жϽ�ֹ��ʹ�ܺ궨��
#define DisableInterrupts       __disable_interrupt
#define EnableInterrupts        __enable_interrupt

#endif