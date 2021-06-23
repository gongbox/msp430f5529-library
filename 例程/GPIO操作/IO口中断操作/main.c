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

__interrupt void GPIO_IRQ_Handler()
{
 if(GPIO_GetITStatus(P1,1)==TRUE)
 {
   GPIO_TurnBit (P1,0);             //��ת���״̬
   GPIO_ClearITPendingBit(P1,1);                //���λ�жϱ�־
 }
 if(GPIO_GetITStatus(P2,1)==TRUE)
 {
   GPIO_TurnBit (P4,7);             //��ת���״̬
   GPIO_ClearITPendingBit(P2,1);                //���λ�жϱ�־
 }
}
void main()
{
  DisableInterrupts();          //��ֹ���ж�
  
  Set_Vector_Handler(VECTOR_PORT1,GPIO_IRQ_Handler);    //�����ж�����������������ж��������ڿ����ж�
  Set_Vector_Handler(VECTOR_PORT2,GPIO_IRQ_Handler);    //�����ж�����
  GPIO_Init     (P1,1 ,GPI|PULL_UP|IRQ_FALLING);                   //��Ϊ�������½��ش����ж�
  GPIO_Init     (P2,1 ,GPI|PULL_UP|IRQ_FALLING);
  GPIO_Init     (P1,0 ,GPO);                            //��Ϊ�������
  GPIO_Init     (P4,7 ,GPO);
  GPIO_ITConfig (P1,1 ,ENABLE);                           //ʹ���ж�
  GPIO_ITConfig (P2,1 ,DISABLE);                          //��ֹ�ж�
  
  EnableInterrupts();
  while(1)
  {  
    DELAY_MS(500);                 //��ʱ500MS
  }
}






