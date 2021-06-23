#include "sim.h"
#include "uart.h"


void SIM_Init()
{
  UART_Init(SIM_UART,SIM_UART_BAUD);
}
STATUS SIM_RecevieTextCompare(char * text,int16 timeout)
{
  while((timeout--) && (*text != '\0'))
  {
    if(UART_GetITStatus (SIM_UART, UART_RX_IRQn) == TRUE);     //��ȡ����ĳһ���жϱ�־
    {
      if(UART_GetChar(SIM_UART) != (*text++))                                        //��ȡһ���ֽ�1���ֽ�
      {
        return FALSE;
      }
    }
    dlay_ms(1);
  }
  return TRUE;
}
void SIM_SendMsg(char * telNumber,char * text)
{
  UART_PutString(SIM_UART, "AT+CSCS=\"GSM\"");                      //�����ַ���
  //�ȴ����յ�������Ϣ��OK��
  //if(SIM_RecevieTextCompare("OK",500))
}