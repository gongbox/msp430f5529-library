#ifndef DMA_H_
#define DMA_H_

#include"common.h"

typedef enum 
{
  DMA_CH0       ,
  DMA_CH1       ,
  DMA_CH2       ,
}DMA_CHn;    //DMA通道
typedef enum
{
  DMATSEL_DMA_REQ    =0 ,  // 0:  DMA_REQ (sw) 
  DMATSEL_TA0CCR0       ,  // 1:  Timer0_A (TA0CCR0.IFG) //带中断触发的需关闭中断使能位，传输后IFG标志位自动清零
  DMATSEL_TA0CCR2       ,  // 2:  Timer0_A (TA0CCR2.IFG) 
  DMATSEL_TA1CCR0       ,  // 3:  Timer1_A (TA1CCR0.IFG) 
  DMATSEL_TA1CCR2       ,  // 4:  Timer1_A (TA1CCR2.IFG) 
  DMATSEL_TA2CCR0       ,  // 5:  Timer2_A (TA2CCR0.IFG) 
  DMATSEL_TA2CCR2       ,  // 6:  Timer2_A (TA2CCR2.IFG) 
  DMATSEL_TB0CCR0       ,  // 7:  TimerB (TB0CCR0.IFG) 
  DMATSEL_TB0CCR2       ,  // 8:  TimerB (TB0CCR2.IFG) 
  DMATSEL_USCIA0RX   =16,  // 16: USCIA0 receive 
  DMATSEL_USCIA0TX      ,  // 17: USCIA0 transmit 
  DMATSEL_USCIB0RX      ,  // 18: USCIB0 receive 
  DMATSEL_USCIB0TX      ,  // 19: USCIB0 transmit 
  DMATSEL_USCIA1RX      ,  // 20: USCIA1 receive 
  DMATSEL_USCIA1TX      ,  // 21: USCIA1 transmit 
  DMATSEL_USCIB1RX      ,  // 22: USCIB1 receive 
  DMATSEL_USCIB1TX      ,  // 23: USCIB1 transmit 
  DMATSEL_ADC12IFG      ,  // 24: ADC12IFGx 
  DMATSEL_USB_FNRXD  =27,  // 27: USB FNRXD 
  DMATSEL_USB_READY     ,  // 28: USB ready 
  DMATSEL_MPY           ,  // 29: Multiplier ready 
  DMATSEL_DMAxIFG       ,  // 30: previous DMA channel DMAxIFG 
  DMATSEL_DMAE0         ,  // 31: ext. Trigger (DMAE0) 
}DMA_Trigger;         //DMA触发源
typedef enum
{
  DMA_SRCAddr_KeepOn      =DMASRCINCR_0,     //源地址不变(默认)
  DMA_SRCAddr_Decrease    =DMASRCINCR_2,     //源地址减小
  DMA_SRCAddr_Increase    =DMASRCINCR_3,     //源地址增加
}DMA_SRC_ADDR_Config;      //DMA地址变化模式
typedef enum
{
  DMA_DSTAddr_KeepOn      =DMADSTINCR_0,     //目的地址不变(默认)
  DMA_DSTAddr_Decrease    =DMADSTINCR_2,     //目的地址减小
  DMA_DSTAddr_Increase    =DMADSTINCR_3,     //目的地址增加
}DMA_DST_ADDR_Config;      //DMA地址变化模式
typedef enum
{
  DMA_WORD                 = 0,              //以字传输(默认)
  DMA_BYTE         = DMASRCBYTE|DMADSTBYTE,  //以字节为单位传输
}DMA_DataSize;             //DMA传输单位
typedef enum
{
  DMA_Single               =DMADT_0,         //单次传输(默认)
  DMA_Block                =DMADT_1,         //块传输
  DMA_Burst_Block          =DMADT_2,         //突发块传输
  DMA_Repeated_Single      =DMADT_4,         //重复单次传输
  DMA_Repeated_Block       =DMADT_5,         //重复块传输
  DMA_Repeated_Burst_Block =DMADT_6,         //重复突发块传输
}DMA_TransmitMode;          //DMA传输模式
typedef enum
{
  DMA_Edge                 =0,               //边沿触发(默认)
  DMA_Level                =DMALEVEL         //高电平触发     
}DMA_TriggerMode;           //DMA触发方式

extern void   DMA_Init(DMA_CHn, void *SADDR, void *DADDR, DMA_Trigger, uint16_t count, uint16_t config);  //DMA初始化配置
extern void   DMA_Cmd (DMA_CHn, STATUS State);                                  //DMA使能或禁止
extern void   DMA_SoftwareTrigger(DMA_CHn);                                    //DMA软件触发一次传输

extern void   DMA_ITConfig (DMA_CHn, STATUS ITState);                           //设置使能或禁止DMA中断
extern STATUS DMA_GetITStatus(DMA_CHn);                                         //判断某一个通道中断标志位 
extern void   DMA_ClearITPendingBit(DMA_CHn);                                   //DMA清除通道中断标志

#endif