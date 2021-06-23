/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 ƽ̨������

 * @author     CTGU-GB
 * @version    v2.7
 * @date       2016-11-26
 */
#include "include.h"

uint16_t data=1000;
float fdata=1.0; 
__interrupt void TIMER1_A0_IRQ_Handler()
{
   //GPIO_TurnBit  (PB, 15);//PIN_OUT(P4,7) = !PIN_OUT(P4,7);
   GPIO_TurnBit  (P4, 7);
   data++;  
   fdata+=0.1;
}
void main()
{
  DisableInterrupts();            //��ֹ���ж�
  SMCLK_OUT();
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  OLED_Init();                       //OLED��ʼ��
  //LCD_INIT();
    
  Set_Vector_Handler(VECTOR_TIMER_A1_CCR0, TIMER1_A0_IRQ_Handler);   //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
  TIMER_Interval_Ms(TIMER_A1, 500);    //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
  TIMER_ITConfig (TIMER_A1, TIMER_CCR0_IRQn, ENABLE);            //ʹ��TIMER��ĳһ���ж�
  
  //OLED��ʾ�ַ����ַ�����������С����һ�������͹���
  OLED_PrintfAt(FONT_ASCII_6X8,0,0,"A");//��ʾһ���ַ�
  OLED_PrintfAt(FONT_ASCII_6X8,0,6,"BCDE!");//��ʾһ���ַ���
  OLED_PrintfAt(FONT_ASCII_6X8,1,0,"%d",-100);//��ʾһ������
  OLED_PrintfAt(FONT_ASCII_6X8,2,0,"%.5f",-100.008310);//��ʾһ��������
  
  EnableInterrupts();
  while(1)
  {  
    //OLED_PrintfAt(FONT_ASCII_6X8,3,0,"data=%d\nfdata=%f",data,fdata);//��ʾ���� 
     DELAY_MS(500);
     GPIO_TurnBit  (P1, 0);
     //GPIO_TurnBit  (PA, 0);
     //PIN_OUT(P1,0) = !PIN_OUT(P1,0);
  }
}