#ifndef _LED_H_
#define _LED_H_

#include "common.h"

typedef enum
{ 
  LED1          ,
  LED2          ,
  LED_ALL       ,
  LED_NUM =  LED_ALL,
}LEDn;

//宏定义LED灯端口
#define LED1_PORT       P1
#define LED1_PIN        0
#define LED2_PORT       P4
#define LED2_PIN        7
//宏定义LED状态
#define LED_ON          BIT_SET
#define LED_OFF         RESET

void LED_Init(LEDn);                                                            //LED灯初始化
void LED_SetStatus(LEDn, STATUS status);                                        //设置LED状态
void LED_Off (LEDn);                                                            //指示灯熄灭
void LED_On  (LEDn);                                                            //指示灯亮
void LED_Turn(LEDn);                                                            //指示灯闪烁
void LED_Water_Lights(void);                                                    //流水灯

#endif
