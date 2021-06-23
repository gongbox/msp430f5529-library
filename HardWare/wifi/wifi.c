#include "wifi.h"
#include "uart.h"
#include "LCD_API.h"
#include "DELAY.h"

uint16 wifi_data_length=0;
char wifi_data_buf[FIFO_SIZE];
const char * WIFI_CWMODE="AT+CWMODE=3\r\n";
const char * WIFI_RST   ="AT+RST\r\n";
const char * WIFI_CWJAP ="AT+CWJAP=\"613\",\"1234567890\"\r\n";
const char * WIFI_CIPSTART="AT+CIPSTART=\"TCP\",\"192.168.0.113\",8765\r\n";
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void WIFI_Init()
{
  /*
  UART_PutString      (UART0,"AT+CIOBAUD=57600\r\n");             //
  while(datalength==0); 
  DELAY_MS(500);
  LCD_Printf(databuf);
  datalength=0;
  UART_Init        (UART0,57600);                   //初始化uartx模块
  UART_IRQ_Enable  (UART0,UART_RX_Interrupt);            //开串口接收中断
  */
  UART_PutString      (UART0,WIFI_CWMODE);             // 
  while(wifi_data_length==0); 
  DELAY_MS(500);
  LCD_Printf(wifi_data_buf);
  wifi_data_length=0;
  UART_PutString      (UART0,WIFI_RST);             //  
  while(wifi_data_length==0); 
  DELAY_MS(500);
  LCD_Printf(wifi_data_buf);
  wifi_data_length=0;
  UART_PutString      (UART0,WIFI_CWJAP);             // 
  while(wifi_data_length==0); 
  DELAY_MS(2000);
  LCD_Printf(wifi_data_buf);
  wifi_data_length=0;
  UART_PutString      (UART0,WIFI_CIPSTART);             // 
  while(wifi_data_length==0); 
  DELAY_MS(2000);
  LCD_Printf(wifi_data_buf);
  wifi_data_length=0;
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void WIFI_SendString(char * string)
{
  UART_Printf      (UART0,"AT+CIPSEND=%d\r\n",strlen(string));             //
  UART_Printf      (UART0,"%s\r\n",string);             //
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
void WIFI_FIFO_Write(char data)
{
  if(wifi_data_length>=FIFO_SIZE)
    return;
  wifi_data_buf[wifi_data_length++]=data;
  if(wifi_data_length>=FIFO_SIZE)    //超过最大值时，回到起始位置
    wifi_data_length=0;
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
char * WIFI_FIFO_Read_String()
{
  wifi_data_buf[wifi_data_length]='\0'; //字符串最后一位设置为'\0'
  wifi_data_length=0;                   //读取后剩余数据为0
  return wifi_data_buf;
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
char WIFI_FIFO_Read()
{
  if(wifi_data_length>0)  //有数据时读取数据
    return wifi_data_buf[--wifi_data_length];
  else                  //无数据时返回0
    return 0;
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/ 
uint16 WIFI_FIFO_Count()
{
  return wifi_data_length;
}