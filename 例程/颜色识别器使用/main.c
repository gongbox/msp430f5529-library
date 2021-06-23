/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,CTGU-GB
 *     All rights reserved.
 *
 *
 * @file       main.c
* @brief      MSP430F5529 平台主程序

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
//变量、常量定义
uint8_t rp=3,gp=3,bp=8; //定义比例因子，具体环境可以修改
uint16_t color_red,color_green,color_blue;
__interrupt void TIMER2_A1_IRQ_Handler()
{
  if(TIMER_GetITStatus(TIMER_A2,TIMER_OverFlow_IRQn))
  {
    overflowTime++;
    TIMER_ClearITPendingBit(TIMER_A2,TIMER_OverFlow_IRQn);     //清除TIMER的某一个中断标志
  }
}
__interrupt void TIMER1_A0_IRQ_Handler()
{
        LED_Turn(LED1);               //指示灯闪烁
   	//   count+1实现先检测绿色,再检测蓝色,然后检测红色,循环检测       
	if(count==0)
	{
		count++;    
		s2=1;s3=1;             //选择滤波器为绿色     
		color_red=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);    //计算这段时间内 TCS230 的输出脉冲数
                TIMER_Pluse_Clear   (TIMER_A2);                               //清零一个定时器的脉冲计数值 
		color_red/=rp;
	}	
	else if(count==1)
	{            
		count++;
		s2=1;s3=0;            //选择滤波器为蓝色
		color_green=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);    //计算这段时间内 TCS230 的输出脉冲数
                TIMER_Pluse_Clear   (TIMER_A2);                               //清零一个定时器的脉冲计数值 
		color_green/=gp;
               
	}	
	else if(count==2)
	{            
		count=0;
		s2=0;s3=0;            //选择滤波器为红色
		color_blue=TIMER_Pluse_GetValue    (TIMER_A2,overflowTime);            //读取某一个定时器脉冲计数值;    //计算这段时间内 TCS230 的输出脉冲数	
		TIMER_Pluse_Clear   (TIMER_A2);                               //清零一个定时器的脉冲计数值
                color_blue/=bp;	
                
	}
        overflowTime=0;
}

void main()
{
  DisableInterrupts();            //禁止总中断
  
  LED_Init(LED_ALL);              //LED灯初始化
  LCD_INIT();
  GPIO_MultiBits_Init(P1,BIT4+BIT5,GPO);
  
  Set_Vector_Handler(VECTOR_TIMER1_A0,TIMER1_A0_IRQ_Handler);   //设置定时器TA1的中断服务程序为：TIMER1_A0_IRQ_Handler
  TIMER_Interval_Ms(TIMER_A1,10);       //设置TA1为定时中断，中断时间为500Ms
  TIMER_ITConfig (TIMER_A1,TIMER_CCR0_IRQn,ENABLE);              //使能TIMER的某一个中断
  
  Set_Vector_Handler(VECTOR_TIMER2_A1,TIMER2_A1_IRQ_Handler);   //设置定时器TA1的中断服务程序为：TIMER1_A0_IRQ_Handler
  TIMER_Pluse_Init  (TIMER_A2);                               //初始化一个定时器用作脉冲计数 P2.2
  TIMER_ITConfig (TIMER_A2,TIMER_OverFlow_IRQn,ENABLE);              //使能TIMER的某一个中断
  
  EnableInterrupts();
  while(1)
  {  
    LCD_Printf_8x16(0,0,"Color_R:%d",color_red);//显示一个整数
    LCD_Printf_8x16(0,20,"Color_G:%d",color_green);//显示一个整数  
    LCD_Printf_8x16(0,40,"Color_B:%d",color_blue);//显示一个整数  
    LCD_FillPart(0,60,100,100,RGB565_TO_COLOR(color_red,color_green,color_blue));  
    //DELAY_MS(500);
    LED_Turn(LED1);               //指示灯闪烁
  } 
}