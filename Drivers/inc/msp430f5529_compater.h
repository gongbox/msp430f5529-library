#ifndef COMPATER_B_H_
#define COMPATER_B_H_

#include "common.h"

typedef enum
{
  CB_CH0                ,//P6.0
  CB_CH1                ,//P6.1
  CB_CH2                ,//P6.2
  CB_CH3                ,//P6.3
  CB_CH4                ,//P6.4
  CB_CH5                ,//P6.5
  CB_CH6                ,//P6.6
  CB_CH7                ,//P6.7
  CB_CH8                ,//P7.0
  CB_CH9                ,//P7.1
  CB_CH10               ,//P7.2
  CB_CH11               ,//P7.3
  CB_VREF_1_5V          ,//使用1.5V作为参考电压
  CB_VREF_2_0V          ,//使用2.0V作为参考电压
  CB_VREF_2_5V          ,//使用2.5V作为参考电压
  CB_VREF_VCC           ,//使用Vcc作为参考电压
}CompaterB_CHn;      //比较器通道
typedef enum
{
  CB_RaisingIRQn         =0x01u,     //上升沿触发中断
  CB_FallingIRQn         =0x02u,     //下降沿触发中断
}CompaterB_IRQn; //比较器中断触发沿

extern void    CompaterB_Init(CompaterB_CHn CB0, CompaterB_CHn CB1);            //比较器初始化，CB0为正相输入通道，CB1为反向输入通道
extern STATUS  CompatorB_GetValue(void);                                        //获取比较器比较结果
extern void    CompaterB_ExchangeChanel(STATUS status);                         //设置是否交换输入通道极性，开启中断后可能会触发一次中断
extern void    CompaterB_ITConfig   (CompaterB_IRQn, STATUS ITState);           //比较器中断设置
extern STATUS  CompaterB_GetITStatus(CompaterB_IRQn);                           //获取中断标志状态
extern void    CompaterB_ClearITPendingBit(CompaterB_IRQn);                     //清除中断标志
#endif