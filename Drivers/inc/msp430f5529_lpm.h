#ifndef LPM_H_
#define LPM_H_

#include "common.h"
#include "msp430f5529.h"
#include "msp430f5529_MemMap.h"

//低功耗模式
typedef enum
{
  LPM_AM  =(0x0u),                                 //活动模式   ，CPU活动，MCLK活动，SMCLK活动，ACLK活动
  LPM_0   =(CPUOFF),                               //低功耗模式0，CPU禁止，MCLK禁止，SMCLK活动，ACLK活动
  LPM_1   =(SCG0+CPUOFF),                          //低功耗模式1，CPU禁止，MCLK禁止，SMCLK活动，ACLK活动
  LPM_2   =(SCG1+CPUOFF),                          //低功耗模式2，CPU禁止，MCLK禁止，SMCLK禁止，ACLK活动
  LPM_3   =(SCG1+SCG0+CPUOFF),                     //低功耗模式3，CPU禁止，MCLK禁止，SMCLK禁止，ACLK活动
  LPM_4   =(SCG1+SCG0+OSCOFF+CPUOFF),              //低功耗模式4，CPU禁止，MCLK禁止，SMCLK禁止，ACLK禁止
}LPMx;

extern void LowPowerMode_Enter(LPMx);                  //低功耗模式设置，注意不能在中断中使用
#define     LowPowerMode_Exit()    LPM4_EXIT           //中断后退出低功耗模式,注意一定要在中断中使用

#endif