#include "msp430f5529_nmi.h"

const SFR_MemMapPtr SFRX = SFR_BASE_PTR;
/*************************************************************************
*  函数名称：NMI_Init(NMIIES ies)
*  功能说明：配置NMI
*  参数说明：NMIIES ies : 社会中NMI中断触发沿
*  函数返回：无
*  使用示例：NMI_Init(NMI_Rising); //设置NMI为上升沿触发中断
*************************************************************************/  
void  NMI_Init(NMIIES ies)
{
  SFRX->SYS.NMI = ENABLE;                     //使能NMI功能
  SFRX->IE.NMI = RESET;                       //禁止NMI中断
  if(ies == NMI_Rising)
  {
    SFRX->SYS.NMIIES = BIT_SET;                   //设置NMI中断沿
  }
  else
  {
    SFRX->SYS.NMIIES = RESET;
  }
  SFRX->IFG.NMI = RESET;                      //清除NMI中断标志
}
/*************************************************************************
*  函数名称：NMI_DeInit()
*  功能说明：屏蔽NMI功能
*  参数说明：无
*  函数返回：无
*  使用示例：NMI_DeInit(); //取消NMI功能
*************************************************************************/  
void    NMI_DeInit   (void)
{
  SFRX->SYS.NMI = DISABLE;                    //禁止NMI功能
}
/*************************************************************************
*  函数名称：NMI_ITConfig (STATUS ITState)
*  功能说明：设置是否使能NMI中断
*  参数说明：BOOL ITState :是否使能NMI中断
*  函数返回：无
*  使用示例：NMI_ITConfig (ENABLE);  //使能NMI中断
*************************************************************************/  
void  NMI_ITConfig (STATUS ITState)
{
  if(ITState != DISABLE)
  {
    SFRX->IE.NMI = ENABLE;                    //设置使能NMI中断
  }
  else
  {
    SFRX->IE.NMI = DISABLE;
  }
}
/*************************************************************************
*  函数名称：NMI_GetITStatus(void)
*  功能说明：读取NMI中断标志
*  参数说明：无
*  函数返回：STATUS : NMI中断事件是否发生
*  使用示例：if(TRUE == NMI_GetITStatus()){...} //判断NMI中断事件是否发生
*************************************************************************/  
STATUS  NMI_GetITStatus(void)               
{
  return ((SFRX->IFG.NMI) ? TRUE : FALSE);       //返回中断事件标志
}
/*************************************************************************
*  函数名称：NMI_ClearITPendingBit(void)
*  功能说明：清除NMI中断标志
*  参数说明：无
*  函数返回：无
*  使用示例：NMI_ClearITPendingBit();  //清除NMI中断标志
*************************************************************************/  
void  NMI_ClearITPendingBit(void)
{
   SFRX->IFG.NMI = RESET;                       //清除中断事件标志
}