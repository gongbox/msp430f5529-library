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

void main()
{
  DisableInterrupts();          //��ֹ���ж�
  
  LED_Init(LED1);              //LED�Ƴ�ʼ��
  
  //��ʱ��A0��Ƶ��1K,ռ�ձ�Ϊ0,��ʼ��4��ͨ�����ֱ��ǣ�TIMER_CH1(P1.2), TIMER_CH2(P1.3), TIMER_CH3(P1.4), TIMER_CH4(P1.5)
  //TIMER_PWM_MultiChannel_Init(TIMER_A0,1000, 4, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);  
  TIMER_PWM_Init(TIMER_A0,1000, TIMER_CH1, TIMER_CH2, TIMER_CH3, TIMER_CH4);   
  
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH1,0);       //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH2,200);     //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH3,700);     //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
  TIMER_PWM_SetChannelDuty  (TIMER_A0,TIMER_CH4,1000);    //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
  
  //��ʱ��B0��Ƶ��1HZ��ռ�ձ�Ϊ0,��ʼ��һ��ͨ����TIMER_CH5(P3.5)
  TIMER_PWM_MultiChannel_Init(TIMER_B0, 1, 1, TIMER_CH5);  
  //TIMER_PWM_Init(TIMER_B0, 1, TIMER_CH5); 
  
  TIMER_PWM_SetChannelDuty  (TIMER_B0,TIMER_CH5,500);     //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
  GPIO_PinRemapConfig(P4,7,GPIO_Remap_TB0CCR5A);        //GPIO�˿���ӳ��
  
  while(1)
  {  
    DELAY_MS(500);
    LED_Turn(LED1);              //ָʾ����˸
  }
}

