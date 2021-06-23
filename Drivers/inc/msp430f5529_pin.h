#ifndef PIN_H_
#define PIN_H_

#include "common.h"

#define GPIO_PIN_DIR(port,pin)    BIT_VALUE_IN_BYTE(port##DIR,pin)                   //输入方向寄存器
#define GPIO_PIN_OUT(port,pin)    BIT_VALUE_IN_BYTE(port##OUT,pin)                   //输出状态寄存器
#define GPIO_PIN_IN(port,pin)     BIT_VALUE_IN_BYTE(port##IN ,pin)                   //输入状态寄存器
#define GPIO_PIN_SEL(port,pin)    BIT_VALUE_IN_BYTE(port##SEL,pin)                   //第二功能寄存器
#define GPIO_PIN_REN(port,pin)    BIT_VALUE_IN_BYTE(port##REN,pin)                   //上下拉电阻寄存器
#define GPIO_PIN_IE(port,pin)     BIT_VALUE_IN_BYTE(port##IE ,pin)                   //输入中断寄存器
#define GPIO_PIN_IES(port,pin)    BIT_VALUE_IN_BYTE(port##IES,pin)                   //中断触发沿寄存器
#define GPIO_PIN_IFG(port,pin)    BIT_VALUE_IN_BYTE(port##IFG,pin)                   //中断标志寄存器

typedef enum
{
  PIN1_0 = 0x00  ,PIN1_1,PIN1_2,PIN1_3,PIN1_4,PIN1_5,PIN1_6,PIN1_7,
  PIN2_0 = 0x10  ,PIN2_1,PIN2_2,PIN2_3,PIN2_4,PIN2_5,PIN2_6,PIN2_7,
  PIN3_0 = 0x20  ,PIN3_1,PIN3_2,PIN3_3,PIN3_4,PIN3_5,PIN3_6,PIN3_7,
  PIN4_0 = 0x30  ,PIN4_1,PIN4_2,PIN4_3,PIN4_4,PIN4_5,PIN4_6,PIN4_7,
  PIN5_0 = 0x40  ,PIN5_1,PIN5_2,PIN5_3,PIN5_4,PIN5_5,PIN5_6,PIN5_7,
  PIN6_0 = 0x50  ,PIN6_1,PIN6_2,PIN6_3,PIN6_4,PIN6_5,PIN6_6,PIN6_7,
  PIN7_0 = 0x60  ,PIN7_1,PIN7_2,PIN7_3,PIN7_4,PIN7_5,PIN7_6,PIN7_7,
  PIN8_0 = 0x70  ,PIN8_1,PIN8_2,PIN8_3,PIN8_4,PIN8_5,PIN8_6,PIN8_7,
#ifdef GPIO_PORT_SIZE_WORD
  PINA_0 = PIN1_0,PINA_1,PINA_2,PINA_3,PINA_4,PINA_5,PINA_6,PINA_7,
  PINA_8 = PIN2_0,PINA_9,PINA_10,PINA_11,PINA_12,PINA_13,PINA_14,PINA_15,
  PINB_0 = PIN3_0,PINB_1,PINB_2,PINB_3,PINB_4,PINB_5,PINB_6,PINB_7,
  PINB_8 = PIN4_0,PINB_9,PINB_10,PINB_11,PINB_12,PINB_13,PINB_14,PINB_15,
  PINC_0 = PIN5_0,PINC_1,PINC_2,PINC_3,PINC_4,PINC_5,PINC_6,PINC_7,
  PINC_8 = PIN6_0,PINC_9,PINC_10,PINC_11,PINC_12,PINC_13,PINC_14,PINC_15,
  PIND_0 = PIN7_0,PIND_1,PIND_2,PIND_3,PIND_4,PIND_5,PIND_6,PIND_7,
  PIND_8 = PIN8_0,PIND_9,PIND_10,PIND_11,PIND_12,PIND_13,PIND_14,PIND_15,
#endif
}PIN;
typedef struct GPIO_PIN
{
  PORTn   Port;
  uint8_t Pin;
}GPIO_PIN;

#endif