#include "msp430f5529_compater.h"
#include "msp430f5529_gpio.h"

const COMPB_MemMapPtr CB = COMPB_BASE_PTR;
/*******************************************************************************
*  �������ƣ�CompaterB_Init(CompaterB_CHn CB0,CompaterB_CHn CB1)
*  ����˵������ʼ���Ƚ�������Դ
*  ����˵����CompaterB_CHn CB0 :�Ƚ�������ͨ��+
             CompaterB_CHn CB1 :�Ƚ�������ͨ��-
*  �������أ���
*  �ο����ϣ�http://www.docin.com/p-676623167.html
*  ʹ��ʾ���� CompaterB_Init(CB_CH0,CB_VREF_2_0V);    //��ʼ���Ƚ���CB_CH0Ϊ������ˣ�CB_VREF_2_0VΪ�������
********************************************************************************/
void CompaterB_Init(CompaterB_CHn CB0,CompaterB_CHn CB1)
{
  ASSERT((CB0<CB_CH11) || (CB1<CB_CH11),"CompaterB_Init","CompaterB�Ƚ���ֻ����һ��ͨ��ʹ�òο���ѹ��\n");//ֻ����һ��ͨ��ʹ�òο���ѹ
  //��������  
  if(CB0 <= CB_CH11)
  { //��ʼ������Ϊ�ڶ�����
    GPIO_Init((CB0 < CB_CH8) ? P6 : P7, (CB0 < CB_CH8) ? CB0 : (CB0-8),GPI|SEL );
    CB->IPEN = BIT_SET;                 //����ͨ��ʹ��
    CB->IPSEL = CB0;                //����ͨ��ѡ��
    CB->CTL3 |= (1<<CB0);           //�ر����뻺��
  }
  else
  {
    CBCTL2 = 0u;
    CB->RSEL = RESET;                   //�ο���ѹӦ�õ��Ƚ���+��        
  }

  if(CB1 <= CB_CH11)
  { //��ʼ������Ϊ�ڶ�����
    GPIO_Init((CB1 < CB_CH8) ? P6 : P7, (CB1 < CB_CH8) ? CB1 : (CB1-8),GPI|SEL );
    CB->IMEN = BIT_SET;                 //����ͨ��ʹ��
    CB->IMSEL = CB1;                //����ͨ��ѡ��
    CB->CTL3 |= (1<<CB1);           //�ر����뻺��
  }
  else
  {
    CB->RSEL = BIT_SET;                   //�ο���ѹӦ�õ��Ƚ���-��
  }

  if((CB0>=CB_VREF_1_5V) || (CB1>=CB_VREF_1_5V))  //�����һ��ʹ�òο���ѹ
  {
    CB->MRVS = 0;
    if((CB0==CB_VREF_VCC) || (CB1==CB_VREF_VCC))
    {
      CB->REFL = 0;
      CB->RS = 1;                                                    //�ⲿ�ο���ѹ��ֹ,ʹ��Vcc��Ϊ�ο���ѹԴ
    }
    else if((CB0==CB_VREF_1_5V) || (CB1==CB_VREF_1_5V)) //
    {
      CB->REFL = 1;
      CB->RS = 3;
      CB->REFACC = RESET;                                                //�ο���ѹΪ1.5v,ʹ���ⲿ�ο���ѹ��ѹ
    }
    else if((CB0==CB_VREF_2_0V) || (CB1==CB_VREF_2_0V)) //
    {
      CB->REFL = 2;
      CB->RS = 3;
      CB->REFACC = RESET;                                                //�ο���ѹΪ2.0v,ʹ���ⲿ�ο���ѹ��ѹ
    }
    else if((CB0==CB_VREF_2_5V) || (CB1==CB_VREF_2_5V)) //
    {
      CB->REFL = 3;
      CB->RS = 3;
      CB->REFACC = RESET;                                               //�ο���ѹΪ2.5v,ʹ���ⲿ�ο���ѹ��ѹ
    }
    CB->REF0 = 31;
    CB->REF1 = 31;
  }

  CB->PWRMD = 1;                             //������Դģʽ 
  CB->ON = BIT_SET;                              // �򿪱Ƚ���B  
}
/*******************************************************************************
*  ��������: CompaterB_ExchangeChanel(STATUS status)  
*  ����˵������������ͨ������
*  ����˵����STATUS status���Ƿ񽻻�����
*  �������أ���
*  ʹ��ʾ����CompaterB_ExchangeChanel(DISABLE);   //��ֹ�����Ƚ�������ͨ��   
********************************************************************************/
void  CompaterB_ExchangeChanel(STATUS status)                                      
{
  CB->EX = status;
}
/*******************************************************************************
*  �������ƣ�CompatorB_GetValue(void)
*  ����˵������ȡ�ȽϵĽ��
*  ����˵������
*  �������أ�STATUS :BIT_SET ������˵�ѹ���ڸ�����˵�ѹ��RESET ������˵�ѹС���ڸ�����˵�ѹ
*  ʹ��ʾ����
********************************************************************************/
STATUS CompatorB_GetValue(void)
{
  return ((CB->CB_OUT) ? BIT_SET : RESET);
}
/*******************************************************************************
*  ��������: CompaterB_ITConfig (CompaterB_IRQn irqn, STATUS ITState)
*  ����˵���������ж��Ƿ�ʹ�ܲ������жϴ�����
*  ����˵����CompaterB_IRQn irqn:ѡ��Ϊ�����ػ����½����ж�
             BOOL ITState       :�Ƿ�ʹ���ж�
*  �������أ���
*  ʹ��ʾ����CompaterB_ITConfig (CB_RaisingIRQn, ENABLE);  //�Ƚ�������Ϊ�������жϣ���ʹ���ж�
********************************************************************************/
void CompaterB_ITConfig (CompaterB_IRQn irqn, STATUS ITState)
{
  CB->IES = RESET;                 //������CBIFG���½���CBIIFG
  if(ITState != DISABLE)//�����Ƿ������ж�
  {
    CB->CB_IFG &=~irqn;              //���жϱ�־λ
    CB->CB_IE  |= irqn;
  }
  else
  {
    CB->CB_IE  &=~irqn;
  }                      
}
/*******************************************************************************
*  �������ƣ�CompaterB_GetITStatus(CompaterB_IRQn irqn)
*  ����˵������ȡ�Ƚ��жϱ�־״̬
*  ����˵����CompaterB_IRQn irqn:ѡ��Ϊ�����ػ����½����жϱ�־
*  �������أ�STATUS ���жϱ�־�Ƿ���λ
*  ʹ��ʾ����if(TRUE == CompaterB_GetITStatus(CB_RaisingIRQn)){...} //�ж��жϱ�־�Ƿ���λ
********************************************************************************/
STATUS  CompaterB_GetITStatus(CompaterB_IRQn irqn)
{
  return ((CB->CB_IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  �������ƣ�CompaterB_ClearITPendingBit(CompaterB_IRQn irqn)
*  ����˵��������Ƚ��жϱ�־λ
*  ����˵����CompaterB_IRQn irqn:ѡ��Ϊ�����ػ����½����жϱ�־
*  �������أ���
*  ʹ��ʾ����CompaterB_ClearITPendingBit(CB_RaisingIRQn);   //����������жϱ�־
********************************************************************************/
void  CompaterB_ClearITPendingBit(CompaterB_IRQn irqn)
{
  CB->CB_IFG &=~ irqn;
}