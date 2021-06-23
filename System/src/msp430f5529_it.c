#include "msp430f5529_it.h"
#include "LED.h"
#include "include.h"
/*******************************************************************************
*  �������ƣ�default_isr(void)
*  ����˵����Ĭ�ϵ��жϺ��������ĳһ���жϿ��������������ж�������ӳ�䵽RAM
             ��û����Set_Vector_Handler(VECTORn vector,__interrupt void pfunc_handler(void))�����жϷ�������
             �����ж�ʱ��������Ĭ���жϷ�������������жϷ���������ָʾ��ָʾ�жϽ������
             ���ô��ڴ�ӡ������Ϣ
*  ����˵������
*  �������أ���
*  ʹ��ʾ������
********************************************************************************/
__interrupt void default_isr(void)
{
#ifdef DEBUG
#ifdef DEBUG_UART_PRINT
    //�����������Ѿ���ʼ�������ڣ������������޸���SMCLKƵ�����������ò����ʣ��Ա�������ò�����
    UART_SetBaudRate (Print_UART,Print_Baud);                    
#endif
    LED_Init(LED_ALL);                        //LED�Ƴ�ʼ��
    //while (1)  //����ȡ����ѭ��
    {
       DEBUG_PRINTF("\n��δ֪�жϽ��룬�����Ƿ����ж�ʹ��ȴδ�����жϷ�������\n");  //��ӡ������Ϣ
       LED_Water_Lights();                                         //����ˮ����ָʾ�������״̬
    }
#endif
}