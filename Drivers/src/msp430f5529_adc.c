#include "msp430f5529_adc.h"
#include "msp430f5529_system.h"

const ADC_MemMapPtr ADC12 = ADC_BASE_PTR;     //ADC�Ĵ����ṹ��
static uint8_t  InitialedChannel_Num = 0u;      //ADC��ʼ����ͨ��������Ϊ���ܻ��γ�ʼ�����ͨ��������ȡ��̬����
static uint16_t InitialedChannel_Bit = 0u;      //�Ѿ���ʼ����ADCͨ��
static uint16_t ADC_Channel_Bit[ADC_CH_NUM] = {0u};   //�ڲ���ʼ����־λ
static inline int8_t  ADC_GetChanelNum(ADC_CHn ch); //�ڲ�����
/*******************************************************************************
*  �������ƣ�ADC_Init(uint16_t ch,ADC_VREF Vref,ADC_Resolution nBit)
*  ����˵����ADC��ʼ��һ������ADCͨ��
*  ����˵����
            uint16_t ch     :Ҫ��ʼ����ͨ��
            ADCVREF Vref    :�ο���ѹ
            ADC_Resolution nBit   :ת������
*  �������أ���
*  ע��    ��1����ʼ��������ͨ��������ͬ��������ͨ��ת������ֻȡ�������һ�����õľ���
             2������ͨ���ڲ��ο���ѹֻ��ʹ��1.5��2.5V�е�һ��
*  ʹ��ʾ����ADC_Init(ADC_CH1+ADC_CH3,ADC_VREF_1_5V,ADC_10bit);        //ͬʱ��ʼ������ͨ����ʹ���ڲ��ṩ��1.5V�ο���ѹ��10λ����
********************************************************************************/
void ADC_Init(uint16_t ch,ADC_VREF Vref,ADC_Resolution nBit)
{
  ADC12->ENC = DISABLE;                 //�ȶԸ�λ��λ��������λ����Щ�Ĵ��������ٽ��в���
  uint16_t SREF_RegValue = 0u;
  switch(Vref)           //ѡ��ο���ѹ
  {
  case ADC_VREF_1_5V:
    REFCTL0 &=~ REFMSTR;                //��REF�ο���ѹ����Ȩ����ADC
    ADC12->ADCREFON = BIT_SET;
    ADC12->REF2_5V = RESET;
    SREF_RegValue = ADC12_SREF_1;       //ʹ���ڲ��ṩ�Ĳο���ѹ
    break;
  case ADC_VREF_2_5V:
    REFCTL0 &=~ REFMSTR;
    ADC12->ADCREFON = BIT_SET;
    ADC12->REF2_5V = BIT_SET;
    SREF_RegValue = ADC12_SREF_1;       //ʹ���ڲ��ṩ�Ĳο���ѹ
    break;
  case ADC_VREF_External:      
    P5SEL |= BIT0;       //VeREF+
    P5SEL |= BIT1;       //VeREF-
    SREF_RegValue = ADC12_SREF_2;       //ʹ���ⲿ�ο���ѹ
  case ADC_VREF_3_3V:
    SREF_RegValue = ADC12_SREF_0;       //ʹ�õ�Դ��ѹ��Ϊ�ο���ѹ,
    break;
  default :break;
  }
  //��ʼ��ͨ��
  uint16_t CH_Mask = 0x01;
  for(uint8_t CH_Num=0; CH_Num < ADC_CH_NUM; CH_Num++)
  {
    if(ch & CH_Mask)   //Ҫ��ʼ����ͨ��
    {
      if((CH_Mask & InitialedChannel_Bit) == 0)   //�����ͨ��δ����ʼ����               
      {
        ADC12->MCTL[InitialedChannel_Num].INCH = CH_Num;
        ADC12->MCTL[InitialedChannel_Num].REF  = SREF_RegValue;                 //�ο���ѹѡ��ͨ������
        ADC_Channel_Bit[InitialedChannel_Num] = CH_Mask;                        //����ͨ����ʼ����־��λ                                                    
        InitialedChannel_Bit |= CH_Mask;                                        //��λ�ѳ�ʼ����ͨ����־λ
        InitialedChannel_Num ++;                                                //��ʼ����ͨ������1
      }
      else                                      //�����֮ǰ�Ѿ���ʼ������ͨ������Ҫ�������������һ�£�ֻ����ͨ���ο���ѹ
      {
        ADC12->MCTL[ADC_GetChanelNum((ADC_CHn)CH_Mask)].REF = SREF_RegValue;   //�ı�ο���ѹ                                  
      }
    }
    CH_Mask <<= 1;
  }
  if(InitialedChannel_Num > 1)                 //�ж��ͨ��ʱҪ��������ͨ������λ
  {
    for(int i=0; i<(InitialedChannel_Num-1); i++)   //ǰ������ͨ�����н���λ����
    {
      ADC12->MCTL[i].EOS = RESET;
    }
    ADC12->MCTL[InitialedChannel_Num-1].EOS = BIT_SET;  //���һͨ����������ͨ��ת������λ
  }
  //һ������
  ADC12->SHT0 = M0_M7_SAMPLE_HOLD_TIME; //0-7ͨ�������뱣��ʱ��
  ADC12->SHT1 = M8_M15_SAMPLE_HOLD_TIME;//8-15ͨ�������뱣��ʱ��
  ADC12->ON = BIT_SET;                      // ����ʱ�䣬AD�ں˿���
  ADC12->MSC = BIT_SET;
  ADC12->SSEL = ADC12_SSEL_ADC12OSC;    //ʱ��ѡ��
  ADC12->SHP = BIT_SET;
  ADC12->CONSEQ = (InitialedChannel_Num > 1) ? BIT_SET : RESET;//�ж��ͨ��ʱ��Ϊ����ͨ������ת��ģʽ,����ͨ��ʱ��Ϊ��ͨ������ת��ģʽ              
  //����ADC����
  ADC12->RES = nBit;    
  //����ʱ��
  UCS->MODOSC_REQEN = BIT_SET;                                                   //ADCʹ�õ���MODCLK �������������ø�ʱ�ӣ�Լ5MHz����
  //����ͨ��Ϊ�ڶ�����
  if(ch & 0x00ff)
  {
     GPIO_MultiBits_Init(P6,(ch & 0x00ff),GPI | SEL);
  }
  if(ch & 0xf000)
  {
     GPIO_MultiBits_Init(P7,((ch & 0xf000)>>12),GPI | SEL);
  }
  if(ch & ADC_CH10) //���ʹ�����¶ȴ�����ͨ���������¶ȴ�����
  {
    ADC12->TCOFF = RESET;
  }
  ADC12->ENC = ENABLE;                                                        //��λ�����ڳ�ʼ����ɺ�����λ
  //DELAY_MS(5);                                                                 //��ʱ�ȴ��ο���ѹ����
}
/*******************************************************************************
*  �������ƣ�ADC_DeInit(ADC_CHn ch)
*  ����˵����ADC��λĳһͨ��
*  ����˵����ADC_CHn ch ��Ҫ��λ��ͨ��
*  �������أ���
*  ע��    ��
*  ʹ��ʾ����ADC_DeInit(ADC_CH1);         //��λͨ��ADC_CH1������ͨ����ADC���ܱ����Σ���Ϊ��ͨIO��ʹ��                              
********************************************************************************/
void   ADC_DeInit(ADC_CHn ch)
{
  ADC12->ENC = RESET;                                                    //�ȶԸ�λ��λ��������λ����Щ�Ĵ��������ٽ��в���
  uint16_t CH_Mask = 0x01;
  for(uint8_t CH_Num=0; CH_Num < ADC_CH_NUM; CH_Num++)
  {
    if(ch & CH_Mask)   //Ҫ��λ��ͨ��
    {
      if(CH_Mask & InitialedChannel_Bit)  //�����ͨ���Ѿ���ʼ������
      {
        uint8_t chanel = ADC_GetChanelNum((ADC_CHn)CH_Mask);
        InitialedChannel_Bit &=~ CH_Mask;            //��ͨ����ʼ����־����
        InitialedChannel_Num --;                     //�ѳ�ʼ��ͨ������һ
        //����ͨ��ǰ��
        for(;chanel < InitialedChannel_Num; chanel++)
        {
          ADC12->MCTL[chanel] = ADC12->MCTL[chanel + 1];
          ADC_Channel_Bit[chanel] =  (ADC_CHn)(1 << ADC12->MCTL[chanel + 1].INCH);
        }
        //���һͨ���ָ�Ĭ��ֵ
        ADC12->MCTL[chanel].INCH = RESET;         //���øüĴ���ͨ��Ϊ0
        ADC12->MCTL[chanel].REF = RESET;          //���øüĴ���ͨ���ο���ѹΪĬ��
        ADC12->MCTL[chanel].EOS = RESET;          //��������ͨ��ת������λΪ0
        ADC_Channel_Bit[chanel] = RESET;          //���ø�ͨ����ʼ����־λ��λ
      }
    }
    CH_Mask <<= 1;
  }
  if(InitialedChannel_Num <= 1)                 //��ֻ��һ��ͨ��ʱȡ������ͨ��ת������λ
  {
    ADC12->MCTL[InitialedChannel_Num-1].EOS = RESET;  //
  }
  ADC12->CONSEQ = (InitialedChannel_Num > 1) ? BIT_SET : RESET;//�ж��ͨ��ʱ��Ϊ����ͨ������ת��ģʽ,����ͨ��ʱ��Ϊ��ͨ������ת��ģʽ              

  //����ͨ��ΪIO�ڹ���
  if(ch & 0x00ff)
  {
     GPIO_MultiBits_Init(P6,(uint8_t)(ch & 0x00ff),GPI);
  }
  if(ch & 0xf000)
  {
     GPIO_MultiBits_Init(P7,(uint8_t)((ch & 0xf000)>>12),GPI);
  }
  ADC12->ENC = BIT_SET;                                                        //��λ�����ڳ�ʼ����ɺ�����λ
}
/*******************************************************************************
*  �������ƣ�ADC_StartConv(void)
*  ����˵��������һ��ADCת��
*  ����˵������
*  �������أ���
*  ע��    �����ж��ͨ��ʱ������ͬʱʹ���ͨ��ת��һ�Σ������ǶԵ�һͨ������ת��
*  ʹ��ʾ����ADC_StartConv();   //����ͨ������һ��ADCת��
********************************************************************************/
void ADC_StartConv(void)
{
  ADC12->SC = BIT_RESET;
  ADC12->SC = BIT_SET;        //��������ת��
}
/*******************************************************************************
*  �������ƣ�ADC_WaitBusy()
*  ����˵�����ȴ�ADC��æ
*  ����˵������
*  �������أ���
*  ע��    ����
*  ʹ��ʾ����ADC_WaitBusy()��  //�ȴ�ת�����
********************************************************************************/
void   ADC_WaitBusy()
{
  while(ADC12->ADBUSY == BIT_SET);//�ȴ�ADC��æ
}
/*******************************************************************************
*  �������ƣ�ADC_ReadChanelValue(ADC_CHn ch)                           
*  ����˵������ȡһ��ADCͨ����ת�����
*  ����˵����ADC_CHn ch ��Ҫ��ȡ��ͨ��
*  �������أ���ͨ��ת�����
*  ʹ��ʾ����uint16 data = ADC_ReadChanelValue(ADC_CH0);  //��ȡADC_CH0ͨ��ת�����
********************************************************************************/
uint16_t ADC_ReadChanelValue(ADC_CHn ch)                           
{
  return (ADC12->MEM[ADC_GetChanelNum(ch)]);                          //��ȡת���������ȡ����Զ������Ӧ��־λ
}
/*******************************************************************************
*  �������ƣ�ADC_ReadChanelOnce(ADC_CHn ch)
*  ����˵����ת������ȡһ��ADCͨ����ֵ
*  ����˵����ADC_CHn ch ��Ҫת������ȡ��ͨ��
*  �������أ���ͨ��ת�����
*  ʹ��ʾ����uint16 data = ADC_ReadChanelOnce(ADC_CH0);  //��������һ��ADCת������ȡADC_CH0ͨ��ת�����
********************************************************************************/
uint16_t ADC_ReadChanelOnce(ADC_CHn ch)
{
  ADC_StartConv();
  ADC_WaitBusy();
  return ADC_ReadChanelValue(ch);
}
/*******************************************************************************
*  �������ƣ�ADC_ITConfig   (ADC_CHn ch,STATUS ITState)
*  ����˵���������Ƿ�ʹ��ADCĳ��ͨ�����ж�
*  ����˵����ADC_CHn ch ��Ҫ���õ�ͨ��
             STATUS ITState ���ж��Ƿ�ʹ��
*  �������أ���
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
*  �������ƣ�ADC_GetITStatus(ADC_CHn ch)
*  ����˵������ȡĳһͨ���жϱ�־״̬
*  ����˵����ADC_CHn ch ��Ҫ��ȡ��ͨ��
*  �������أ���ͨ���ж�״̬��־�Ƿ���λ��TRUE:��λ RESET:��λ��
********************************************************************************/
STATUS  ADC_GetITStatus(ADC_CHn ch)
{
  return ((ADC12->IFG & (1<<ADC_GetChanelNum(ch))) ? TRUE :FALSE);
}
 /*******************************************************************************
*  �������ƣ�ADC_ClearITPendingBit(ADC_CHn ch)
*  ����˵�������ĳһͨ���жϱ�־λ
*  ����˵����ADC_CHn ch ��Ҫ����жϱ�־��ͨ��
*  �������أ���
********************************************************************************/
void   ADC_ClearITPendingBit(ADC_CHn ch)
{
  ADC12->IFG &=~(1<<ADC_GetChanelNum(ch));
}
/*****************************************************************************
*  �������ƣ�ADC_GetChanelMemoryAddr(ADC_CHn ch)
*  ����˵������ȡ��Ӧͨ���Ľ���Ĵ�����ַ����Ҫ����DMAʱ��Ҫ��
*  ����˵����ADC_CHn ch ��Ҫ��ȡ�����ַ��ͨ��
*  �������أ�void * :ת�������ַ
********************************************************************************/
void * ADC_GetChanelMemoryAddr(ADC_CHn ch)
{
  return (void *)(&(ADC12->MEM[ADC_GetChanelNum(ch)]));
}
/*******************************************************************************
*  �������ƣ�ADC_GetChanelNum(ADC_CHn ch)
*  ����˵������ȡ��Ӧͨ���ļĴ������ֵ
*  ����˵����ADC_CHn ch ��Ҫ��ȡ��ŵ�ͨ��
*  �������أ����,ʧ�ܷ���-1
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