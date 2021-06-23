#include "msp430f5529_dma.h"

const DMA_MemMapPtr      DMAX = DMA_BASE_PTR;
/*******************************************************************************
*  函数名称：DMA_Init(DMA_CHn dma_ch, void *SADDR, void *DADDR, DMA_Trigger Trigger, uint16_t count, uint16_t config)
*  功能说明：初始化一个DMA通道
*  参数说明：DMA_CHn dma_ch             :DMA通道
             void *SADDR                :DMA传输源地址
             void *DADDR                :DMA传输目的地址
             DMA_Trigger Trigger        :DMA传输触发条件
             uint16_t count               :DMA传输次数
             uint16_t config              :DMA传输配置
*  函数返回：无
*  使用示例： DMA_Init(DMA_CH2, &Sdata, &DData,DMATSEL_DMA_REQ,255,  DMA_SRCAddr_KeepOn|DMA_DSTAddr_KeepOn|DMA_WORD|DMA_Single|DMA_Edge);
                       //通道2，源地址，目的地址，触发源，传输255个字，源地址不变，目的地址不变，字传输，单次触发，边沿触发
********************************************************************************/
void DMA_Init(DMA_CHn dma_ch, void *SADDR, void *DADDR, DMA_Trigger Trigger, uint16_t count, uint16_t config)
{ 
  DMAX->CHX[dma_ch].EN = RESET;                          //关闭DMA模块
  DMAX->CHX[dma_ch].DA = (uint32_t)DADDR;                  //设置目的地址
  DMAX->CHX[dma_ch].SA = (uint32_t)SADDR;                  //设置源地址
  DMAX->CHX[dma_ch].SZ = count;                          //设置写入数据大小
  
  //DMAX->TSSEL[dma_ch] = Trigger;                         //这样写会有问题，不知道为什么！
  if((dma_ch%2) == 0)
  {
   *(&DMACTL0 + (dma_ch/2)) &= 0xFF00;      //先清除触发源
   *(&DMACTL0 + (dma_ch/2)) |= Trigger;     //再设置触发源
  }
  else
  {
    *(&DMACTL0 + (dma_ch/2)) &= 0x00FF;
    *(&DMACTL0 + (dma_ch/2)) |= Trigger<<8;
  }
  DMAX->CHX[dma_ch].CTL = config;                       //配置DMA模块
  DMAX->CHX[dma_ch].EN = BIT_SET;                           //使能DMA模块
}
/*******************************************************************************
*  函数名称：DMA_Cmd  (DMA_CHn dma_ch,BOOL State)
*  功能说明：DMA使能或禁止
*  参数说明：DMA_CHn dma_ch     :DMA传输通道
             STATUS State       :DMA是否使能
*  函数返回：无
*  使用示例：DMA_Cmd  (DMA_CH1,ENABLE);  //使能DMA通道1
********************************************************************************/
void DMA_Cmd  (DMA_CHn dma_ch,STATUS State)
{
  DMAX->CHX[dma_ch].EN = State; 
}
/*******************************************************************************
*  函数名称：DMA_SoftwareTrigger(DMA_CHn dma_ch)
*  功能说明：软件触发一次DMA传输
*  参数说明：DMA_CHn dma_ch     :DMA传输通道
*  函数返回：无
*  使用示例：DMA_SoftwareTrigger(DMA_CH1);  //软件触发一次DMA通道1传输
********************************************************************************/
void DMA_SoftwareTrigger(DMA_CHn dma_ch)
{    
  DMAX->CHX[dma_ch].REQ = RESET; 
  DMAX->CHX[dma_ch].REQ = BIT_SET; 
}
/*******************************************************************************
*  函数名称：DMA_ITConfig (DMA_CHn dma_ch,STATUS ITState)
*  功能说明：设置是否使能DMA某一通道完成中断
*  参数说明：DMA_CHn dma_ch     :DMA传输通道
             BOOL ITState       :是否使能DMA传输完成中断
*  函数返回：无
*  使用示例：DMA_ITConfig (DMA_CH0,ENABLE);  //使能DMA通道1传输完成中断
********************************************************************************/
void DMA_ITConfig (DMA_CHn dma_ch,STATUS ITState)
{
  DMAX->CHX[dma_ch].IFG = RESET;
  DMAX->CHX[dma_ch].IE = ITState; 
}
/*******************************************************************************
*  函数名称：DMA_GetITStatus(DMA_CHn dma_ch)
*  功能说明：判断某一个通道中断标志位状态
*  参数说明：DMA_CHn dma_ch     :DMA传输通道
*  函数返回：STATUS  :TRUE 中断事件发生 FALSE 中断事件未发生
*  使用示例：if(TRUE == DMA_GetITStatus(DMA_CH0)){...}  //判断DMA通道1是否发生传输完成中断
********************************************************************************/
STATUS DMA_GetITStatus(DMA_CHn dma_ch)
{
  return ((DMAX->CHX[dma_ch].IFG) ? TRUE : FALSE);
}
/*******************************************************************************
*  函数名称：DMA_ClearITPendingBit(DMA_CHn dma_ch)
*  功能说明：DMA清除某一通道中断标志
*  参数说明：DMA_CHn dma_ch     :DMA传输通道
*  函数返回：无
*  使用示例：DMA_ClearITPendingBit(DMA_CH0);  //清除DMA通道1传输完成中断标志位
********************************************************************************/
void DMA_ClearITPendingBit(DMA_CHn dma_ch)
{
  DMAX->CHX[dma_ch].IFG = RESET;
}
