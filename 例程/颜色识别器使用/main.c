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
 * @date       2016-01-30
 */
#include "include.h"
uint16_t data=1000;
float fdata=1.0;
#define s2 PIN_OUT(P1,4)
#define s3 PIN_OUT(P1,5)
uint8_t count=0;
uint16_t overflowTime=0;
uint16_t temp;
//��������������
uint8_t rp=3,gp=3,bp=8; //����������ӣ����廷�������޸�
uint16_t color_red,color_green,color_blue;
__interrupt void TIMER2_A1_IRQ_Handler()
{
  if(TIMER_GetITStatus(TIMER_A2,TIMER_OverFlow_IRQn))
  {
    overflowTime++;
    TIMER_ClearITPendingBit(TIMER_A2,TIMER_OverFlow_IRQn);     //���TIMER��ĳһ���жϱ�־
  }
}
__interrupt void TIMER1_A0_IRQ_Handler()
{
        LED_Turn(LED1);               //ָʾ����˸
   	//   count+1ʵ���ȼ����ɫ,�ټ����ɫ,Ȼ�����ɫ,ѭ�����       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //ѡ���˲���Ϊ��ɫ     
		color_red=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);    //�������ʱ���� TCS230 �����������
                TIMER_Pluse_Clear   (TIMER_A2);                               //����һ����ʱ�����������ֵ 
		color_red/=rp;
	}	
	else if(count==1)
	{            
		count++;
		s2=1;s3=0;            //ѡ���˲���Ϊ��ɫ
		color_green=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);    //�������ʱ���� TCS230 �����������
                TIMER_Pluse_Clear   (TIMER_A2);                               //����һ����ʱ�����������ֵ 
		color_green/=gp;
               
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //ѡ���˲���Ϊ��ɫ
		color_blue=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);            //��ȡĳһ����ʱ���������ֵ;    //�������ʱ���� TCS230 �����������	
		TIMER_Pluse_Clear   (TIMER_A2);                               //����һ����ʱ�����������ֵ
                color_blue/=bp;	
                
	}
        overflowTime=0;
}

void main()
{
  DisableInterrupts();            //��ֹ���ж�
  
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  LCD_INIT();
  GPIO_MultiBits_Init(P1,BIT4+BIT5,GPO);
  
  Set_Vector_Handler(VECTOR_TIMER1_A0,TIMER1_A0_IRQ_Handler);   //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
  TIMER_Interval_Ms(TIMER_A1,10);       //����TA1Ϊ��ʱ�жϣ��ж�ʱ��Ϊ500Ms
  TIMER_ITConfig (TIMER_A1,TIMER_CCR0_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�
  
  Set_Vector_Handler(VECTOR_TIMER2_A1,TIMER2_A1_IRQ_Handler);   //���ö�ʱ��TA1���жϷ������Ϊ��TIMER1_A0_IRQ_Handler
  TIMER_Pluse_Init  (TIMER_A2);                               //��ʼ��һ����ʱ������������� P2.2
  TIMER_ITConfig (TIMER_A2,TIMER_OverFlow_IRQn,ENABLE);              //ʹ��TIMER��ĳһ���ж�
  
  EnableInterrupts();
  while(1)
  {  
    LCD_Printf_8x16(0,0,"Color_R:%d",color_red);//��ʾһ������
    LCD_Printf_8x16(0,20,"Color_G:%d",color_green);//��ʾһ������  
    LCD_Printf_8x16(0,40,"Color_B:%d",color_blue);//��ʾһ������  
    LCD_FillPart(0,60,100,100,RGB565_TO_COLOR(color_red,color_green,color_blue));  
    //DELAY_MS(500);
    LED_Turn(LED1);               //ָʾ����˸
  } 
}