#include "LED.h"
#include "msp430f5529_gpio.h"
#include "msp430f5529_system.h"
//����LED����
const GPIO_PIN LED_PIN[LED_NUM]=
{
  {LED1_PORT,LED1_PIN},
  {LED2_PORT,LED2_PIN},
};//LED����
/*******************************************************************************
*  �������ƣ�LED_Init( LEDn led )
*  ����˵����LED�Ƴ�ʼ��
*  ����˵����LEDn led :LED��LED0,LED1...��
*  �������أ���
*  ʹ��ʾ����LED_Init( LED0 );   //��ʼ��LED0
********************************************************************************/
void LED_Init( LEDn led )
{
  if(led == LED_ALL)
  {
    for(led = LED1;led < LED_NUM;led++)
    {
      GPIO_Init(LED_PIN[led].Port, LED_PIN[led].Pin, GPO);
    }
  }
  else
  {
    GPIO_Init(LED_PIN[led].Port, LED_PIN[led].Pin, GPO);
  }
  LED_Off (led);
}
/*******************************************************************************
*  �������ƣ�LED_SetStatus(LEDn led,STATUS status)
*  ����˵��������LED״̬
*  ����˵����LEDn led      :LED��LED1,LED2...��
             STATUS status :�����Ƿ����LED
*  �������أ���
*  ʹ��ʾ����LED_SetStatus(LED1,SET);  //LED1��
********************************************************************************/
void LED_SetStatus(LEDn led,STATUS status)
{
  status = (LED_ON == BIT_SET) ? status : (STATUS)!status;
  
  if(led == LED_ALL)
  {
    for(led = LED1;led < LED_NUM;led++)
    {
      GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, status);
    }
  }
  else
  {
    GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, status);
  }
}
/*******************************************************************************
*  �������ƣ�LED_Off (LEDn led)
*  ����˵�����ر�lED
*  ����˵����LEDn led      :LED��LED1,LED2...��
*  �������أ���
*  ʹ��ʾ����LED_Off(LED1);  //LED1��
********************************************************************************/
void LED_Off (LEDn led)
{
  if(led == LED_ALL)
  {
    for(led = LED1;led < LED_NUM;led++)
    {
      GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, LED_OFF);
    }
  }
  else
  {
    GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, LED_OFF);
  }
}
/*******************************************************************************
*  �������ƣ�LED_On  (LEDn led)
*  ����˵��������
*  ����˵����LEDn led      :LED��LED1,LED2...��
*  �������أ���
*  ʹ��ʾ����LED_On(LED0);  //LED0��
********************************************************************************/
void LED_On  (LEDn led)
{
  if(led == LED_ALL)
  {
    for(led = LED1;led < LED_NUM;led++)
    {
      GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, LED_ON);
    }
  }
  else
  {
    GPIO_WriteBit(LED_PIN[led].Port, LED_PIN[led].Pin, LED_ON);
  }
}
/*******************************************************************************
*  �������ƣ�LED_Turn(LEDn led)
*  ����˵�����Ʒ�ת
*  ����˵����LEDn led      :LED��LED1,LED2...��
*  �������أ���
*  ʹ��ʾ����LED_Turn(LED0);  //LED0�����ת
********************************************************************************/
void LED_Turn(LEDn led)
{
  if(led == LED_ALL)
  {
    for(led = LED1;led < LED_NUM;led++)
    {
      GPIO_TurnBit(LED_PIN[led].Port, LED_PIN[led].Pin);
    }
  }
  else
  {
    GPIO_TurnBit(LED_PIN[led].Port, LED_PIN[led].Pin);
  }
}
/*******************************************************************************
*  �������ƣ�water_lights(void)
*  ����˵������ˮ��
*  ����˵������
*  �������أ���
*  ʹ��ʾ����water_lights(); //��ˮ����һ��
********************************************************************************/
void LED_Water_Lights(void)
{
  for(LEDn led = LED1;led < LED_NUM;led++)
  {
    LED_Turn(led);
    DELAY_MS(500);
  }
}
