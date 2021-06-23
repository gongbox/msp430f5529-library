#include "msp430f5529_compater.h"
#include "msp430f5529_gpio.h"

const COMPB_MemMapPtr CB = COMPB_BASE_PTR;
/*******************************************************************************
*  函数名称：CompaterB_Init(CompaterB_CHn CB0,CompaterB_CHn CB1)
*  功能说明：初始化比较器输入源
*  参数说明：CompaterB_CHn CB0 :比较器输入通道+
             CompaterB_CHn CB1 :比较器输入通道-
*  函数返回：无
*  参考资料：http://www.docin.com/p-676623167.html
*  使用示例： CompaterB_Init(CB_CH0,CB_VREF_2_0V);    //初始化比较器CB_CH0为正输入端，CB_VREF_2_0V为负输入端
********************************************************************************/
void CompaterB_Init(CompaterB_CHn CB0,CompaterB_CHn CB1)
{
  ASSERT((CB0<CB_CH11) || (CB1<CB_CH11),"CompaterB_Init","CompaterB比较器只能有一个通道使用参考电压！\n");//只能有一个通道使用参考电压
  //设置引脚  
  if(CB0 <= CB_CH11)
  { //初始化引脚为第二功能
    GPIO_Init((CB0 < CB_CH8) ? P6 : P7, (CB0 < CB_CH8) ? CB0 : (CB0-8),GPI|SEL );
    CB->IPEN = BIT_SET;                 //输入通道使能
    CB->IPSEL = CB0;                //输入通道选择
    CB->CTL3 |= (1<<CB0);           //关闭输入缓存
  }
  else
  {
    CBCTL2 = 0u;
    CB->RSEL = RESET;                   //参考电压应用到比较器+端        
  }

  if(CB1 <= CB_CH11)
  { //初始化引脚为第二功能
    GPIO_Init((CB1 < CB_CH8) ? P6 : P7, (CB1 < CB_CH8) ? CB1 : (CB1-8),GPI|SEL );
    CB->IMEN = BIT_SET;                 //输入通道使能
    CB->IMSEL = CB1;                //输入通道选择
    CB->CTL3 |= (1<<CB1);           //关闭输入缓存
  }
  else
  {
    CB->RSEL = BIT_SET;                   //参考电压应用到比较器-端
  }

  if((CB0>=CB_VREF_1_5V) || (CB1>=CB_VREF_1_5V))  //如果有一端使用参考电压
  {
    CB->MRVS = 0;
    if((CB0==CB_VREF_VCC) || (CB1==CB_VREF_VCC))
    {
      CB->REFL = 0;
      CB->RS = 1;                                                    //外部参考电压禁止,使用Vcc作为参考电压源
    }
    else if((CB0==CB_VREF_1_5V) || (CB1==CB_VREF_1_5V)) //
    {
      CB->REFL = 1;
      CB->RS = 3;
      CB->REFACC = RESET;                                                //参考电压为1.5v,使用外部参考电压分压
    }
    else if((CB0==CB_VREF_2_0V) || (CB1==CB_VREF_2_0V)) //
    {
      CB->REFL = 2;
      CB->RS = 3;
      CB->REFACC = RESET;                                                //参考电压为2.0v,使用外部参考电压分压
    }
    else if((CB0==CB_VREF_2_5V) || (CB1==CB_VREF_2_5V)) //
    {
      CB->REFL = 3;
      CB->RS = 3;
      CB->REFACC = RESET;                                               //参考电压为2.5v,使用外部参考电压分压
    }
    CB->REF0 = 31;
    CB->REF1 = 31;
  }

  CB->PWRMD = 1;                             //正常电源模式 
  CB->ON = BIT_SET;                              // 打开比较器B  
}
/*******************************************************************************
*  函数名称: CompaterB_ExchangeChanel(STATUS status)  
*  功能说明：交换输入通道极性
*  参数说明：STATUS status：是否交换极性
*  函数返回：无
*  使用示例：CompaterB_ExchangeChanel(DISABLE);   //禁止交换比较器输入通道   
********************************************************************************/
void  CompaterB_ExchangeChanel(STATUS status)                                      
{
  CB->EX = status;
}
/*******************************************************************************
*  函数名称：CompatorB_GetValue(void)
*  功能说明：读取比较的结果
*  参数说明：无
*  函数返回：STATUS :BIT_SET 正输入端电压大于负输入端电压，RESET 正输入端电压小于于负输入端电压
*  使用示例：
********************************************************************************/
STATUS CompatorB_GetValue(void)
{
  return ((CB->CB_OUT) ? BIT_SET : RESET);
}
/*******************************************************************************
*  函数名称: CompaterB_ITConfig (CompaterB_IRQn irqn, STATUS ITState)
*  功能说明：设置中断是否使能并配置中断触发沿
*  参数说明：CompaterB_IRQn irqn:选择为上升沿还是下降沿中断
             BOOL ITState       :是否使能中断
*  函数返回：无
*  使用示例：CompaterB_ITConfig (CB_RaisingIRQn, ENABLE);  //比较器设置为上升沿中断，并使能中断
********************************************************************************/
void CompaterB_ITConfig (CompaterB_IRQn irqn, STATUS ITState)
{
  CB->IES = RESET;                 //上升沿CBIFG，下降沿CBIIFG
  if(ITState != DISABLE)//设置是否允许中断
  {
    CB->CB_IFG &=~irqn;              //清中断标志位
    CB->CB_IE  |= irqn;
  }
  else
  {
    CB->CB_IE  &=~irqn;
  }                      
}
/*******************************************************************************
*  函数名称：CompaterB_GetITStatus(CompaterB_IRQn irqn)
*  功能说明：读取比较中断标志状态
*  参数说明：CompaterB_IRQn irqn:选择为上升沿还是下降沿中断标志
*  函数返回：STATUS ：中断标志是否置位
*  使用示例：if(TRUE == CompaterB_GetITStatus(CB_RaisingIRQn)){...} //判断中断标志是否置位
********************************************************************************/
STATUS  CompaterB_GetITStatus(CompaterB_IRQn irqn)
{
  return ((CB->CB_IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  函数名称：CompaterB_ClearITPendingBit(CompaterB_IRQn irqn)
*  功能说明：清除比较中断标志位
*  参数说明：CompaterB_IRQn irqn:选择为上升沿还是下降沿中断标志
*  函数返回：无
*  使用示例：CompaterB_ClearITPendingBit(CB_RaisingIRQn);   //清除上升沿中断标志
********************************************************************************/
void  CompaterB_ClearITPendingBit(CompaterB_IRQn irqn)
{
  CB->CB_IFG &=~ irqn;
}