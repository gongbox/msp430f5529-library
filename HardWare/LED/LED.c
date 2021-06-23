#include "LED.h"
#include "msp430f5529_gpio.h"
#include "msp430f5529_system.h"
//定义LED引脚
const GPIO_PIN LED_PIN[LED_NUM]=
{
  {LED1_PORT,LED1_PIN},
  {LED2_PORT,LED2_PIN},
};//LED引脚
/*******************************************************************************
*  函数名称：LED_Init( LEDn led )
*  功能说明：LED灯初始化
*  参数说明：LEDn led :LED（LED0,LED1...）
*  函数返回：无
*  使用示例：LED_Init( LED0 );   //初始化LED0
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
*  函数名称：LED_SetStatus(LEDn led,STATUS status)
*  功能说明：设置LED状态
*  参数说明：LEDn led      :LED（LED1,LED2...）
             STATUS status :设置是否点亮LED
*  函数返回：无
*  使用示例：LED_SetStatus(LED1,SET);  //LED1亮
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
*  函数名称：LED_Off (LEDn led)
*  功能说明：关闭lED
*  参数说明：LEDn led      :LED（LED1,LED2...）
*  函数返回：无
*  使用示例：LED_Off(LED1);  //LED1灭
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
*  函数名称：LED_On  (LEDn led)
*  功能说明：灯亮
*  参数说明：LEDn led      :LED（LED1,LED2...）
*  函数返回：无
*  使用示例：LED_On(LED0);  //LED0亮
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
*  函数名称：LED_Turn(LEDn led)
*  功能说明：灯翻转
*  参数说明：LEDn led      :LED（LED1,LED2...）
*  函数返回：无
*  使用示例：LED_Turn(LED0);  //LED0输出翻转
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
*  函数名称：water_lights(void)
*  功能说明：流水灯
*  参数说明：无
*  函数返回：无
*  使用示例：water_lights(); //流水灯亮一次
********************************************************************************/
void LED_Water_Lights(void)
{
  for(LEDn led = LED1;led < LED_NUM;led++)
  {
    LED_Turn(led);
    DELAY_MS(500);
  }
}
