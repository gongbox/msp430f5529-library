#include "msp430f5529_remap.h"

/*******************************************************************************
*  �������ƣ�GPIO_PinRemapConfig(PORTn portn,uint8_t pin,GPIO_Remap remap)
*  ����˵����GPIO��ӳ�䣬����ʹ��һGPIO�ڣ�Ŀǰֻ��P4�˿ڣ�P4.0-P4.7��֧�֣�
             ʵ�ֿɹ�ѡ�����һ���ù��ܣ��μ�.h�ļ���ö��ѡ�
*  ע��    ��ĳЩ���ܻ�����Ҫ����Ӳ������������򼰵ڶ�����
*  ����˵����PORTn portn        :GPIO��ӳ��˿�
             uint8_t pin          :GPIO��ӳ������
             GPIO_Remap remap   :GOIO��ӳ�书��
*  �������أ���
*  ʹ��ʾ����GPIO_PinRemapConfig(P4,7,GPIO_Remap_MCLK)�� //P4.7��ӳ��ΪMCLK��ʱ�����
********************************************************************************/
void  GPIO_PinRemapConfig(PORTn portn,uint8_t pin,GPIO_Remap remap)
{
#ifdef  GPIO_PORT_SIZE_WORD
  ASSERT(((portn == (PORTB)) && (pin>=8))||((portn == (PORT4)) && (pin<=7)),
         "GPIO_PinRemapConfig",
         "portn,pin����������");              //���Լ���������
  if(portn == PORTB)pin = pin - 8;
#else
  ASSERT(portn == PORT4,"GPIO_PinRemapConfig","portn����������");                             //���Լ���������
#endif 
  PMAPPWD = PMAPKEY;                                            //����Կ��
  *(&P4MAP0 + pin) = remap;                                     //�˿���ӳ������ 
  PMAPCTL |= PMAPRECFG;                                         //�����������õĶ˿�ӳ��
  
  if(((remap>=GPIO_Remap_TB0CCR0A) && (remap<=GPIO_Remap_TB0CCR6A)) 
      || (remap==GPIO_Remap_MCLK))      //�������Щ���ù�������Ҫ��Ϊ�������Ҳ���ܲ��ǣ�������Ҳ��һ������Ҫ��Ϊ�������
  {
    GPIO_Init(PORT4,pin,GPO | SEL);
  }
  else
  {
    GPIO_Init(PORT4,pin,SEL);
  }
  PMAPPWD = 0;                                                 //��ֹ����
}

