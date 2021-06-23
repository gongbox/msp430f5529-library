#ifndef _GPIO_H_
#define _GPIO_H_

#include "common.h"

//是否使用16位IO端口定义,使用则包含PORTA-PORTD。注意这时调用IO口操作函数由于需要求取port,pin值，执行效率会变低
//#define GPIO_PORT_SIZE_WORD

//引脚对应掩码宏定义
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

typedef enum
{
  //常用写法
  P1            ,PORT1 = P1,
  P2            ,PORT2 = P2,
  P3            ,PORT3 = P3,
  P4            ,PORT4 = P4,
  P5            ,PORT5 = P5,
  P6            ,PORT6 = P6,
  P7            ,PORT7 = P7,
  P8            ,PORT8 = P8,
#ifdef GPIO_PORT_SIZE_WORD
//注意实际上PORTA是P1.0-P2.7的所有16个IO口，PORTB是P3.0-P4.7的所有16个IO口,PORTC,PORTD以此类推
  PA            ,PORTA = PA,  //P1.0-P2.7
  PB            ,PORTB = PB,  //P3.0-P4.7
  PC            ,PORTC = PC,  //P5.0-P6.7
  PD            ,PORTD = PD,  //P7.0-P8.7
#endif
}PORTn;  //端口
typedef enum
{
  GPI                   =0x0001u,// 输入方向（默认）
  GPO                   =0x0002u,// 输出方向
  
  LDS                   =0x0004u,// 低驱动能力（默认）
  HDS                   =0x0008u,// 高驱动能力
  
  PULL_DOWN             =0x0010u,// 下拉
  PULL_UP               =0x0020u,// 上拉
  
  IRQ_RISING            =0x0040u,// 上升沿触发中断
  IRQ_FALLING           =0x0080u,// 下降沿触发中断
  
  SEL                   =0x0100u,// 第二复用功能
}GPIO_Config;     //GPIO模式
/**************************IO口操作函数****************************************/
extern void      GPIO_MultiBits_Init(PORTn, uint16_t BITs, uint16_t config);    //一位或多位IO口功能初始化,参数BITs为BIT0,BIT1，...BIT7或他们的和等
#define          GPIO_Init(port,pin,config)         GPIO_MultiBits_Init(port,(1u<<pin),config)

extern void      GPIO_SetBits  (PORTn, uint16_t BITs);                          //一位或多位IO口输出高电平
extern void      GPIO_ResetBits(PORTn, uint16_t BITs);                          //一位或多位IO口输出低电平

extern void      GPIO_WriteBit (PORTn, uint8_t pin, STATUS level);              //某1位IO口输出电平设置
extern STATUS    GPIO_ReadBit  (PORTn, uint8_t pin);                            //某1位IO口输入电平获取
extern void      GPIO_TurnBit  (PORTn, uint8_t pin);                            //某1位IO口输出电平翻转

extern void      GPIO_WriteByte(PORTn, uint8_t value);                          //某8位IO口输出电平设置
extern uint8_t   GPIO_ReadByte (PORTn);                                         //某8位IO口输入电平获取
#ifdef GPIO_PORT_SIZE_WORD
extern void      GPIO_WriteWord(PORTn, uint16_t value);                         //某16位IO口输出电平设置
extern uint16_t  GPIO_ReadWord (PORTn);                                         //某16位IO口输入电平获取
#endif
extern void      GPIO_ITConfig (PORTn, uint8_t pin, STATUS ITState);            //使能或禁止某1位输入端口的中断
extern STATUS    GPIO_GetITStatus(PORTn, uint8_t pin);                          //获取某一位端口中断标志
extern void      GPIO_ClearITPendingBit(PORTn, uint8_t pin);                    //清除某一位中断标志

//位操作宏定义，使用PIN作为参数
#include "msp430f5529_pin.h"
#define          GPIO_PinInit(pin,config)       GPIO_Init((PORTn)((pin>>4)+PORT1),(pin&0x0f),config)
#define          GPIO_PinWriteBit(pin,level)    GPIO_WriteBit((PORTn)((pin>>4)+PORT1),(pin&0x0f),level) 
#define          GPIO_PinReadBit(pin)           GPIO_ReadBit((PORTn)((pin>>4)+PORT1),(pin&0x0f)) 
#define          GPIO_PinTurnBit(pin)           GPIO_TurnBit((PORTn)((pin>>4)+PORT1),(pin&0x0f)) 
/********************************************************************************************
实现像51单片机一样的位控制
如PIN_OUT(P1,1)=1;便是将P1.1位输出高电平，不影响P1端口的其他位，int value=PIN_IN(P1,0);便是读取P1.0引脚的输入电平值（0或1）
注意:port,pin参数必须是P1-P8,可以是宏定义,但不能为变量,
*********************************************************************************************/
#define          PIN_DIR(port,pin)               GPIO_PIN_DIR(port,pin)         //输入方向寄存器
#define          PIN_OUT(port,pin)               GPIO_PIN_OUT(port,pin)         //输出状态寄存器
#define          PIN_IN(port,pin)                GPIO_PIN_IN(port,pin)          //输入状态寄存器
#define          PIN_SEL(port,pin)               GPIO_PIN_SEL(port,pin)         //第二功能寄存器
#define          PIN_REN(port,pin)               GPIO_PIN_REN(port,pin)         //上下拉电阻寄存器
#define          PIN_IE(port,pin)                GPIO_PIN_IE(port,pin)          //输入中断寄存器
#define          PIN_IES(port,pin)               GPIO_PIN_IES(port,pin)         //中断触发沿寄存器
#define          PIN_IFG(port,pin)               GPIO_PIN_IFG(port,pin)         //中断标志寄存器

#endif