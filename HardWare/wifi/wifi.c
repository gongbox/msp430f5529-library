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
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
void WIFI_Init()
{
  /*
  UART_PutString      (UART0,"AT+CIOBAUD=57600\r\n");             //
  while(datalength==0); 
  DELAY_MS(500);
  LCD_Printf(databuf);
  datalength=0;
  UART_Init        (UART0,57600);                   //��ʼ��uartxģ��
  UART_IRQ_Enable  (UART0,UART_RX_Interrupt);            //�����ڽ����ж�
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
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
void WIFI_SendString(char * string)
{
  UART_Printf      (UART0,"AT+CIPSEND=%d\r\n",strlen(string));             //
  UART_Printf      (UART0,"%s\r\n",string);             //
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
void WIFI_FIFO_Write(char data)
{
  if(wifi_data_length>=FIFO_SIZE)
    return;
  wifi_data_buf[wifi_data_length++]=data;
  if(wifi_data_length>=FIFO_SIZE)    //�������ֵʱ���ص���ʼλ��
    wifi_data_length=0;
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
char * WIFI_FIFO_Read_String()
{
  wifi_data_buf[wifi_data_length]='\0'; //�ַ������һλ����Ϊ'\0'
  wifi_data_length=0;                   //��ȡ��ʣ������Ϊ0
  return wifi_data_buf;
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
char WIFI_FIFO_Read()
{
  if(wifi_data_length>0)  //������ʱ��ȡ����
    return wifi_data_buf[--wifi_data_length];
  else                  //������ʱ����0
    return 0;
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/ 
uint16 WIFI_FIFO_Count()
{
  return wifi_data_length;
}