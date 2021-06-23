#ifndef ADC_H_
#define ADC_H_

#include"common.h"  

//宏定义ADC12MEM8 to ADC12MEM15采样与保持时间  0-15
#define M8_M15_SAMPLE_HOLD_TIME  3
//宏定义ADC12MEM0 to ADC12MEM7 采样与保持时间  0-15
#define M0_M7_SAMPLE_HOLD_TIME   3

typedef enum
{
  ADC_8bit              ,  //8 位精度,最大值256-1
  ADC_10bit             ,  //10位精度,最大值1024-1
  ADC_12bit             ,  //12位精度,最大值4096-1
}ADC_Resolution;   //精度
typedef enum
{
  ADC_VREF_1_5V         ,  //内部1.5V
  ADC_VREF_2_5V         ,  //内部2.5V
  ADC_VREF_3_3V         ,  //供电电压作为参考电压
  ADC_VREF_External     ,  //使用外部提供参考电压
}ADC_VREF;         //参考电压
typedef enum
{
  ADC_CH0        =BIT0,  //P6.0
  ADC_CH1        =BIT1,  //P6.1
  ADC_CH2        =BIT2,  //P6.2
  ADC_CH3        =BIT3,  //P6.3
  ADC_CH4        =BIT4,  //P6.4
  ADC_CH5        =BIT5,  //P6.5
  ADC_CH6        =BIT6,  //P6.6
  ADC_CH7        =BIT7,  //P6.7
  ADC_CH8        =BIT8,  //VeREF+    //外部正基准
  ADC_CH9        =BIT9,  //VeREF-    //外部负基准
  ADC_CH10       =BITA,  //片内温度传感器
  ADC_CH11       =BITB,  //(AVCC-AVSS)/2
  ADC_CH12       =BITC,  //P7.0
  ADC_CH13       =BITD,  //P7.1
  ADC_CH14       =BITE,  //P7.2
  ADC_CH15       =BITF,  //P7.3 
  ADC_CH_NUM     =16u,   //通道数量
}ADC_CHn;       //通道

extern void     ADC_Init(uint16_t ch, ADC_VREF, ADC_Resolution);                //初始化一个或多个通道
extern void     ADC_DeInit(ADC_CHn);                                            //复位ADC通道，取消该通道ADC功能
extern void     ADC_StartConv(void);                                            //启动一次转换
extern void     ADC_WaitBusy(void);                                             //等待ADC不忙
extern uint16_t ADC_ReadChanelValue (ADC_CHn);                                  //读取ADC结果
extern uint16_t ADC_ReadChanelOnce  (ADC_CHn);                                  //转转一次并读取ADC结果
extern void     ADC_ITConfig   (ADC_CHn, STATUS ITState);                       //ADC中断设置
extern STATUS   ADC_GetITStatus(ADC_CHn);                                       //获取某一通道中断标志状态
extern void     ADC_ClearITPendingBit(ADC_CHn);                                 //清除某一通道中断标志位
extern void *   ADC_GetChanelMemoryAddr(ADC_CHn);                               //获取某一通道结果寄存器地址

#endif