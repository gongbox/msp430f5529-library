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
    if(UART_GetITStatus (SIM_UART, UART_RX_IRQn) == TRUE);     //获取串口某一个中断标志
    {
      if(UART_GetChar(SIM_UART) != (*text++))                                        //读取一个字节1个字节
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
  UART_PutString(SIM_UART, "AT+CSCS=\"GSM\"");                      //发送字符串
  //等待接收到返回信息“OK”
  //if(SIM_RecevieTextCompare("OK",500))
}