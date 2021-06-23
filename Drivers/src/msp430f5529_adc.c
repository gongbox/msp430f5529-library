#include "msp430f5529_adc.h"
#include "msp430f5529_system.h"

const ADC_MemMapPtr ADC12 = ADC_BASE_PTR;     //ADC寄存器结构体
static uint8_t  InitialedChannel_Num = 0u;      //ADC初始化的通道数，因为可能会多次初始化多个通道，这里取静态变量
static uint16_t InitialedChannel_Bit = 0u;      //已经初始化的ADC通道
static uint16_t ADC_Channel_Bit[ADC_CH_NUM] = {0u};   //内部初始化标志位
static inline int8_t  ADC_GetChanelNum(ADC_CHn ch); //内部函数
/*******************************************************************************
*  函数名称：ADC_Init(uint16_t ch,ADC_VREF Vref,ADC_Resolution nBit)
*  功能说明：ADC初始化一个或多个ADC通道
*  参数说明：
            uint16_t ch     :要初始化的通道
            ADCVREF Vref    :参考电压
            ADC_Resolution nBit   :转换精度
*  函数返回：无
*  注意    ：1：初始化后所有通道精度相同，即所有通道转换精度只取决于最后一次配置的精度
             2：所有通道内部参考电压只能使用1.5或2.5V中的一种
*  使用示例：ADC_Init(ADC_CH1+ADC_CH3,ADC_VREF_1_5V,ADC_10bit);        //同时初始化两个通道，使用内部提供的1.5V参考电压，10位精度
********************************************************************************/
void ADC_Init(uint16_t ch,ADC_VREF Vref,ADC_Resolution nBit)
{
  ADC12->ENC = DISABLE;                 //先对该位复位，否则置位后有些寄存器不能再进行操作
  uint16_t SREF_RegValue = 0u;
  switch(Vref)           //选择参考电压
  {
  case ADC_VREF_1_5V:
    REFCTL0 &=~ REFMSTR;                //把REF参考电压控制权交给ADC
    ADC12->ADCREFON = BIT_SET;
    ADC12->REF2_5V = RESET;
    SREF_RegValue = ADC12_SREF_1;       //使用内部提供的参考电压
    break;
  case ADC_VREF_2_5V:
    REFCTL0 &=~ REFMSTR;
    ADC12->ADCREFON = BIT_SET;
    ADC12->REF2_5V = BIT_SET;
    SREF_RegValue = ADC12_SREF_1;       //使用内部提供的参考电压
    break;
  case ADC_VREF_External:      
    P5SEL |= BIT0;       //VeREF+
    P5SEL |= BIT1;       //VeREF-
    SREF_RegValue = ADC12_SREF_2;       //使用外部参考电压
  case ADC_VREF_3_3V:
    SREF_RegValue = ADC12_SREF_0;       //使用电源电压作为参考电压,
    break;
  default :break;
  }
  //初始化通道
  uint16_t CH_Mask = 0x01;
  for(uint8_t CH_Num=0; CH_Num < ADC_CH_NUM; CH_Num++)
  {
    if(ch & CH_Mask)   //要初始化的通道
    {
      if((CH_Mask & InitialedChannel_Bit) == 0)   //如果该通道未被初始化过               
      {
        ADC12->MCTL[InitialedChannel_Num].INCH = CH_Num;
        ADC12->MCTL[InitialedChannel_Num].REF  = SREF_RegValue;                 //参考电压选择，通道设置
        ADC_Channel_Bit[InitialedChannel_Num] = CH_Mask;                        //将该通道初始化标志置位                                                    
        InitialedChannel_Bit |= CH_Mask;                                        //置位已初始化的通道标志位
        InitialedChannel_Num ++;                                                //初始化的通道数加1
      }
      else                                      //如果是之前已经初始化过的通道，则要在这里从新设置一下，只设置通道参考电压
      {
        ADC12->MCTL[ADC_GetChanelNum((ADC_CHn)CH_Mask)].REF = SREF_RegValue;   //改变参考电压                                  
      }
    }
    CH_Mask <<= 1;
  }
  if(InitialedChannel_Num > 1)                 //有多个通道时要加上序列通道结束位
  {
    for(int i=0; i<(InitialedChannel_Num-1); i++)   //前面所有通道序列结束位清零
    {
      ADC12->MCTL[i].EOS = RESET;
    }
    ADC12->MCTL[InitialedChannel_Num-1].EOS = BIT_SET;  //最后一通道加上序列通道转换结束位
  }
  //一般配置
  ADC12->SHT0 = M0_M7_SAMPLE_HOLD_TIME; //0-7通道采样与保持时间
  ADC12->SHT1 = M8_M15_SAMPLE_HOLD_TIME;//8-15通道采样与保持时间
  ADC12->ON = BIT_SET;                      // 采样时间，AD内核开启
  ADC12->MSC = BIT_SET;
  ADC12->SSEL = ADC12_SSEL_ADC12OSC;    //时钟选择
  ADC12->SHP = BIT_SET;
  ADC12->CONSEQ = (InitialedChannel_Num > 1) ? BIT_SET : RESET;//有多个通道时设为序列通道单次转换模式,单个通道时设为单通道单次转换模式              
  //设置ADC精度
  ADC12->RES = nBit;    
  //开启时钟
  UCS->MODOSC_REQEN = BIT_SET;                                                   //ADC使用的是MODCLK 所以在这里配置该时钟，约5MHz左右
  //配置通道为第二功能
  if(ch & 0x00ff)
  {
     GPIO_MultiBits_Init(P6,(ch & 0x00ff),GPI | SEL);
  }
  if(ch & 0xf000)
  {
     GPIO_MultiBits_Init(P7,((ch & 0xf000)>>12),GPI | SEL);
  }
  if(ch & ADC_CH10) //如果使用了温度传感器通道，则开启温度传感器
  {
    ADC12->TCOFF = RESET;
  }
  ADC12->ENC = ENABLE;                                                        //该位必须在初始化完成后再置位
  //DELAY_MS(5);                                                                 //延时等待参考电压建立
}
/*******************************************************************************
*  函数名称：ADC_DeInit(ADC_CHn ch)
*  功能说明：ADC复位某一通道
*  参数说明：ADC_CHn ch ：要复位的通道
*  函数返回：无
*  注意    ：
*  使用示例：ADC_DeInit(ADC_CH1);         //复位通道ADC_CH1，即该通道的ADC功能被屏蔽，作为普通IO口使用                              
********************************************************************************/
void   ADC_DeInit(ADC_CHn ch)
{
  ADC12->ENC = RESET;                                                    //先对该位复位，否则置位后有些寄存器不能再进行操作
  uint16_t CH_Mask = 0x01;
  for(uint8_t CH_Num=0; CH_Num < ADC_CH_NUM; CH_Num++)
  {
    if(ch & CH_Mask)   //要复位的通道
    {
      if(CH_Mask & InitialedChannel_Bit)  //如果该通道已经初始化，则
      {
        uint8_t chanel = ADC_GetChanelNum((ADC_CHn)CH_Mask);
        InitialedChannel_Bit &=~ CH_Mask;            //该通道初始化标志屏蔽
        InitialedChannel_Num --;                     //已初始化通道数减一
        //所有通道前移
        for(;chanel < InitialedChannel_Num; chanel++)
        {
          ADC12->MCTL[chanel] = ADC12->MCTL[chanel + 1];
          ADC_Channel_Bit[chanel] =  (ADC_CHn)(1 << ADC12->MCTL[chanel + 1].INCH);
        }
        //最后一通道恢复默认值
        ADC12->MCTL[chanel].INCH = RESET;         //设置该寄存器通道为0
        ADC12->MCTL[chanel].REF = RESET;          //设置该寄存器通道参考电压为默认
        ADC12->MCTL[chanel].EOS = RESET;          //设置序列通道转换结束位为0
        ADC_Channel_Bit[chanel] = RESET;          //设置该通道初始化标志位复位
      }
    }
    CH_Mask <<= 1;
  }
  if(InitialedChannel_Num <= 1)                 //最只有一个通道时取消序列通道转换结束位
  {
    ADC12->MCTL[InitialedChannel_Num-1].EOS = RESET;  //
  }
  ADC12->CONSEQ = (InitialedChannel_Num > 1) ? BIT_SET : RESET;//有多个通道时设为序列通道单次转换模式,单个通道时设为单通道单次转换模式              

  //配置通道为IO口功能
  if(ch & 0x00ff)
  {
     GPIO_MultiBits_Init(P6,(uint8_t)(ch & 0x00ff),GPI);
  }
  if(ch & 0xf000)
  {
     GPIO_MultiBits_Init(P7,(uint8_t)((ch & 0xf000)>>12),GPI);
  }
  ADC12->ENC = BIT_SET;                                                        //该位必须在初始化完成后再置位
}
/*******************************************************************************
*  函数名称：ADC_StartConv(void)
*  功能说明：启动一次ADC转换
*  参数说明：无
*  函数返回：无
*  注意    ：若有多个通道时，将会同时使多个通道转换一次，而不是对单一通道进行转换
*  使用示例：ADC_StartConv();   //所有通道启动一次ADC转换
********************************************************************************/
void ADC_StartConv(void)
{
  ADC12->SC = BIT_RESET;
  ADC12->SC = BIT_SET;        //启动采样转换
}
/*******************************************************************************
*  函数名称：ADC_WaitBusy()
*  功能说明：等待ADC不忙
*  参数说明：无
*  函数返回：无
*  注意    ：无
*  使用示例：ADC_WaitBusy()；  //等待转换完成
********************************************************************************/
void   ADC_WaitBusy()
{
  while(ADC12->ADBUSY == BIT_SET);//等待ADC不忙
}
/*******************************************************************************
*  函数名称：ADC_ReadChanelValue(ADC_CHn ch)                           
*  功能说明：读取一个ADC通道的转换结果
*  参数说明：ADC_CHn ch ：要读取的通道
*  函数返回：该通道转换结果
*  使用示例：uint16 data = ADC_ReadChanelValue(ADC_CH0);  //读取ADC_CH0通道转换结果
********************************************************************************/
uint16_t ADC_ReadChanelValue(ADC_CHn ch)                           
{
  return (ADC12->MEM[ADC_GetChanelNum(ch)]);                          //读取转换结果，读取后会自动清除对应标志位
}
/*******************************************************************************
*  函数名称：ADC_ReadChanelOnce(ADC_CHn ch)
*  功能说明：转换并读取一个ADC通道的值
*  参数说明：ADC_CHn ch ：要转换并读取的通道
*  函数返回：该通道转换结果
*  使用示例：uint16 data = ADC_ReadChanelOnce(ADC_CH0);  //立即启动一次ADC转换并读取ADC_CH0通道转换结果
********************************************************************************/
uint16_t ADC_ReadChanelOnce(ADC_CHn ch)
{
  ADC_StartConv();
  ADC_WaitBusy();
  return ADC_ReadChanelValue(ch);
}
/*******************************************************************************
*  函数名称：ADC_ITConfig   (ADC_CHn ch,STATUS ITState)
*  功能说明：设置是否使能ADC某个通道的中断
*  参数说明：ADC_CHn ch ：要设置的通道
             STATUS ITState ：中断是否使能
*  函数返回：无
********************************************************************************/
void ADC_ITConfig   (ADC_CHn ch,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    ADC12->IE |=  (1<<ADC_GetChanelNum(ch));                           //
  }
  else
  {
    ADC12->IE &= ~(1<<ADC_GetChanelNum(ch));                           //
  }
}
/*******************************************************************************
*  函数名称：ADC_GetITStatus(ADC_CHn ch)
*  功能说明：获取某一通道中断标志状态
*  参数说明：ADC_CHn ch ：要读取的通道
*  函数返回：该通道中断状态标志是否置位（TRUE:置位 RESET:复位）
********************************************************************************/
STATUS  ADC_GetITStatus(ADC_CHn ch)
{
  return ((ADC12->IFG & (1<<ADC_GetChanelNum(ch))) ? TRUE :FALSE);
}
 /*******************************************************************************
*  函数名称：ADC_ClearITPendingBit(ADC_CHn ch)
*  功能说明：清除某一通道中断标志位
*  参数说明：ADC_CHn ch ：要清除中断标志的通道
*  函数返回：无
********************************************************************************/
void   ADC_ClearITPendingBit(ADC_CHn ch)
{
  ADC12->IFG &=~(1<<ADC_GetChanelNum(ch));
}
/*****************************************************************************
*  函数名称：ADC_GetChanelMemoryAddr(ADC_CHn ch)
*  功能说明：获取对应通道的结果寄存器地址（主要用于DMA时需要）
*  参数说明：ADC_CHn ch ：要获取结果地址的通道
*  函数返回：void * :转换结果地址
********************************************************************************/
void * ADC_GetChanelMemoryAddr(ADC_CHn ch)
{
  return (void *)(&(ADC12->MEM[ADC_GetChanelNum(ch)]));
}
/*******************************************************************************
*  函数名称：ADC_GetChanelNum(ADC_CHn ch)
*  功能说明：获取对应通道的寄存器编号值
*  参数说明：ADC_CHn ch ：要获取编号的通道
*  函数返回：编号,失败返回-1
********************************************************************************/
inline int8_t ADC_GetChanelNum(ADC_CHn ch)
{
  for(uint8_t Chanel_Num=0; Chanel_Num < ADC_CH_NUM; Chanel_Num++)
  {
    if(ch == ADC_Channel_Bit[Chanel_Num])
    {
      return Chanel_Num;
    }
  }
  return -1;
}