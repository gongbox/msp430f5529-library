#ifndef NMI_H_
#define NMI_H_

#include "common.h"

//NMI 
typedef enum
{
  NMI_Rising             ,                 //上升沿
  NMI_Falling            ,                 //下降沿
}NMIIES;          //NMI中断边沿选择

extern void    NMI_Init     (NMIIES ies);                                       // 初始化NMI功能，并选择中断边沿
extern void    NMI_DeInit   (void);                                             // 屏蔽NMI功能
extern void    NMI_ITConfig (STATUS ITState);                                   // 设置是否使能NMI中断
extern STATUS  NMI_GetITStatus(void);                                           // 获取NMI中断标志
extern void    NMI_ClearITPendingBit(void);                                     // 清除NMI中断标志

#endif