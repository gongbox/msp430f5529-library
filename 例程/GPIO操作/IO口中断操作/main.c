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

__interrupt void GPIO_IRQ_Handler()
{
 if(GPIO_GetITStatus(P1,1)==TRUE)
 {
   GPIO_TurnBit (P1,0);             //翻转输出状态
   GPIO_ClearITPendingBit(P1,1);                //清除位中断标志
 }
 if(GPIO_GetITStatus(P2,1)==TRUE)
 {
   GPIO_TurnBit (P4,7);             //翻转输出状态
   GPIO_ClearITPendingBit(P2,1);                //清除位中断标志
 }
}
void main()
{
  DisableInterrupts();          //禁止总中断
  
  Set_Vector_Handler(VECTOR_PORT1,GPIO_IRQ_Handler);    //设置中断向量，最好先设置中断向量，在开启中断
  Set_Vector_Handler(VECTOR_PORT2,GPIO_IRQ_Handler);    //设置中断向量
  GPIO_Init     (P1,1 ,GPI|PULL_UP|IRQ_FALLING);                   //设为上拉且下降沿触发中断
  GPIO_Init     (P2,1 ,GPI|PULL_UP|IRQ_FALLING);
  GPIO_Init     (P1,0 ,GPO);                            //设为输出方向
  GPIO_Init     (P4,7 ,GPO);
  GPIO_ITConfig (P1,1 ,ENABLE);                           //使能中断
  GPIO_ITConfig (P2,1 ,DISABLE);                          //禁止中断
  
  EnableInterrupts();
  while(1)
  {  
    DELAY_MS(500);                 //延时500MS
  }
}






