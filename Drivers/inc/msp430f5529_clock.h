#ifndef CLOCK_H_  
#define CLOCK_H_

#include "common.h"
#include "msp430f5529_system.h"

//宏定义外接时钟，设置时钟很重要
#define EXTAL_IN_XT1_HZ   (  32768)
#define EXTAL_IN_XT2_HZ   (4000000)
//宏定义内部REFO，VLO时钟频率
#define VLOCLK_FREQUENCY  (  10000)
#define REFOCLK_FREQUENCY (  32768)

typedef struct 
{
  struct
  {
    uint32_t nHZ;
    uint32_t nKHZ;
    float    fMHZ;
  }MCLK,SMCLK,ACLK;
  uint32_t DCO_FLL_Frequency;
}CLOCK;
extern CLOCK g_sClock; //全局变量，时钟频率
typedef enum
{
  FLLREF_XT1    =0u,
  FLLREF_REFO   =2u,
  FLLREF_XT2    =5u
}FLLREF_Source;  //FLL参考时钟源
typedef enum
{
  FLLREF_DIV_1     ,   //不分频
  FLLREF_DIV_2     ,   //二分频
  FLLREF_DIV_4     ,   //四分频
  FLLREF_DIV_8     ,   //八分频
  FLLREF_DIV_12    ,   //十二分频
  FLLREF_DIV_16    ,   //十六分频
}FLLREF_DIVx;    //FLL参考时钟分频
typedef enum
{
  CLOCK_XT1        ,   //XT1      
  CLOCK_VLO        ,   //内部10K
  CLOCK_REFO       ,   //内部32768
  CLOCK_DCO        ,   //内部DCO,如果使用内部DCO作为时钟的话，需要先调用：DCO_PLLConfig设置DCO频率
  CLOCK_DCO_DIV    ,   //DCO分频(分频为1且不允许修改，与DCO同频)
  CLOCK_XT2        ,   //XT2
}CLOCK_Source;  //时钟源
typedef enum
{
  CLOCK_DIV_1      ,   //不分频
  CLOCK_DIV_2      ,   //二分频
  CLOCK_DIV_4      ,   //四分频
  CLOCK_DIV_8      ,   //八分频
  CLOCK_DIV_16     ,   //十六分频
  CLOCK_DIV_32     ,   //三十二分频
}CLOCK_DIVx;    //时钟分频
//设置DCO时钟频率,注意：使用DCO作为时钟源的频率值会改变
extern void CLOCK_DCO_PLLConfig(FLLREF_Source, FLLREF_DIVx, uint32_t DCO_FLL_Fre);   //DCO_FLL倍频环设置
//设置时钟源及分频
extern void CLOCK_MCLK_Config (CLOCK_Source , CLOCK_DIVx);                         //主时钟配置，
extern void CLOCK_SMCLK_Config(CLOCK_Source , CLOCK_DIVx);                         //子系统时钟配置
extern void CLOCK_ACLK_Config (CLOCK_Source , CLOCK_DIVx);                         //辅助时钟配置
//时钟输出
#define MCLK_OUT()                              GPIO_Init(P7,7,GPO | SEL)       //主时钟输出,   P7.7
#define SMCLK_OUT()                             GPIO_Init(P2,2,GPO | SEL)       //子系统时钟输出P2.2
#define ACLK_OUT()                              GPIO_Init(P1,0,GPO | SEL)       //辅助时钟输出  P1.0   
#define CLOCK_OUT()                             do{MCLK_OUT();SMCLK_OUT();ACLK_OUT();}while(0)  //所有时钟输出
#endif