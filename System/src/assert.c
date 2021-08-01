/********************                                        
 * �ļ���       ��assert.c
 * ����         ������������������ִ�к���
 *
 * ʵ��ƽ̨     ��MSP430F5529
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 * ��    ע     ��
 *
 * ����         ��
 **********************************************************************************/

#include "common.h"
#include "led.h"
#include "msp430f5529_system.h"

const char ASSERT_FAILED_STR[] =
        "�����������������������ԭ��\n���Է�����%s�ļ��ĵ�%d�У�\n�������ں�����%s\n����ʧ��˵����%s\t���� by CTGU-GB\n\n";
/********************************************************************/
//������������������������˴���״̬���ͻ��ӡ������Ϣ����LED����ʾ״̬
void assert_failed(char *file, int line, char *funName, char *str)
{
#ifdef DEBUG
#ifdef DEBUG_UART_PRINT
    //�����������Ѿ���ʼ�������ڣ������������޸���SMCLKƵ�����������ò����ʣ��Ա�������ò�����
    UART_SetBaudRate (PRINT_UART,PRINT_BAUD);                    
#endif
    LED_Init(LED_ALL);                        //LED�Ƴ�ʼ��
    while (1)
    {
        DEBUG_PRINTF(ASSERT_FAILED_STR, file, line,funName,str);  //��ӡ������Ϣ
        LED_Water_Lights();                               //����ˮ����ָʾ�������״̬
    }
#endif
}
/********************************************************************/
