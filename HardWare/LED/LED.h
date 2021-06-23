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

//�궨��LED�ƶ˿�
#define LED1_PORT       P1
#define LED1_PIN        0
#define LED2_PORT       P4
#define LED2_PIN        7
//�궨��LED״̬
#define LED_ON          BIT_SET
#define LED_OFF         RESET

void LED_Init(LEDn);                                                            //LED�Ƴ�ʼ��
void LED_SetStatus(LEDn, STATUS status);                                        //����LED״̬
void LED_Off (LEDn);                                                            //ָʾ��Ϩ��
void LED_On  (LEDn);                                                            //ָʾ����
void LED_Turn(LEDn);                                                            //ָʾ����˸
void LED_Water_Lights(void);                                                    //��ˮ��

#endif
