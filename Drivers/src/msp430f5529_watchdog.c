#include "msp430f5529_watchdog.h"
#include "msp430f5529_clock.h"

static const uint32_t WDT_Counter[]={2000000000u,128000000u,8192000u,512000u,32768u,8192u,512u,64u};
const WDT_MemMapPtr WDTX = WDT_BASE_PTR;
extern const SFR_MemMapPtr SFRX;
uint16_t WatchDog_Time_Cal(uint32_t ms);
/*******************************************************************************
*  �������ƣ�DisableWatchDog(void)
*  ����˵�����رտ��Ź�
*  ����˵������
*  �������أ���
********************************************************************************/
void DisableWatchDog(void)
{
  WDTCTL = (WDTCTL&0xFF)|(WDTPW+WDTHOLD);
} 
/*******************************************************************************
*  �������ƣ�WatchDog_Init(uint32_t ms) 
*  ����˵�������ÿ��Ź�ʱ�䣬ע�ⶨʱʱ��ֻ�ǱȽϽӽ�����ʱ���һ��ʱ�䣬������׼,���ؼ�ʱ��ƫ
*  ����˵����uint32_t ms          ������ʱ��
*  �������أ�//���ʱ�䣨��λ��ms��
*  ʹ��ʾ���� WatchDog_Init(500);   //���ÿ��Ź�ʱ��Ϊ500ms
********************************************************************************/
void  WatchDog_Init(uint32_t ms)                                        //���Ź���������
{
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //�رտ��Ź��ж�
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val ; //����
}
/*******************************************************************************
*  �������ƣ�WatchDog_Timer_Init(uint32_t ms) 
*  ����˵�������ÿ��Ź���Ϊ��ʱ��������ʱ�䣬ע�ⶨʱʱ��ֻ�ǱȽϽӽ�����ʱ���һ��ʱ�䣬������׼,���ؼ�ʱ��ƫ
*  ����˵����uint32_t ms          ������ʱ��
*  �������أ�//���ʱ�䣨��λ��ms��
*  ʹ��ʾ���� WatchDog_Timer_Init(500);   //���ÿ��Ź���ʱʱ��Ϊ500ms
********************************************************************************/
void  WatchDog_Timer_Init(uint32_t ms)                                  //���Ź���ʱ�����ܳ�ʼ��
{
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //�رտ��Ź��ж�
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val | WDTTMSEL; //����
}
/*******************************************************************************
*  �������ƣ�WatchDog_Delay_Ms(uint32_t ms) 
*  ����˵�������ÿ��Ź���ʱһ��ʱ�䣬ע�ⶨʱʱ��ֻ�ǱȽϽӽ�����ʱ���һ��ʱ�䣬������׼,���ؼ�ʱ��ƫ
*  ����˵����uint32_t ms          ������ʱ��
*  �������أ�//���ʱ�䣨��λ��ms��
*  ʹ��ʾ����WatchDog_Delay_Ms(500);   //�����ڴ˴�ʹ�ÿ��Ź���ʱ500ms
********************************************************************************/
void  WatchDog_Delay_Ms  (uint32_t ms)                                  //���Ź���ʱʹ��
{
  SFRX->IFG.WDT = RESET;
  uint16_t WDT_Reg_Val = WatchDog_Time_Cal(ms);
  SFRX->IE.WDT = DISABLE;                      //�رտ��Ź��ж�
  WDTCTL = WDTPW | WDTCNTCL | WDT_Reg_Val | WDTTMSEL; //����
  while(SFRX->IFG.WDT == RESET);
}
/*************************************************************************
*  �������ƣ�WatchDog_Feed(void)
*  ����˵����ι��
*  ����˵������
*  �������أ���
*  ʹ��ʾ����WatchDog_Feed();  //ι����������Ź�����ֵ
*************************************************************************/  
void  WatchDog_Feed(void)
{
  WDTCTL =(WDTCTL&0X00FF) | (WDTCNTCL + WDTPW);                //����ֵ����
}
/*************************************************************************
*  �������ƣ�WatchDog_ITConfig(STATUS ITState)
*  ����˵�������ÿ��Ź��ж��Ƿ�ʹ��
*  ����˵����STATUS ITState ���Ƿ�ʹ���ж�
*  �������أ���
*  ʹ��ʾ����WatchDog_ITConfig(ENABLE);  //ʹ�ܿ��Ź��ж�
*************************************************************************/
void  WatchDog_ITConfig(STATUS ITState)
{
  if(ITState != DISABLE)
  {
    SFRX->IFG.WDT = RESET;
    SFRX->IE.WDT = ENABLE;  //�������Ź��ж�
  }
  else
  {
    SFRX->IE.WDT = DISABLE;
  }
}
/*************************************************************************
*  �������ƣ�SoftWare_RESET(void)
*  ����˵�����������һ�θ�λ
*  ����˵������
*  �������أ���
*  ʹ��ʾ����SoftWare_RESET();  //�����λһ�ε�Ƭ��
*****************************************************{********************/  
void  SoftWare_RESET(void)
{
  WDTCTL = 0x0000;
}
//���Ź�����ʱ�亯��
uint16_t WatchDog_Time_Cal(uint32_t ms)
{
  float smclk_OffectTime,aclk_OffectTime,vlo_OffectTime; //���ʱ��
  int32_t smclk_OffectTimeTemp = 0,aclk_OffectTimeTemp = 0 ,vlo_OffectTimeTemp = 0;
  uint16_t smclk_DividerValue = 7 ,aclk_DividerValue = 7, vlo_DividerValue = 7;
  uint32_t SelectClock;
  uint32_t WDT_CounterValue;
  /**********************����ʱ��Դ����ʱʱ��**********************************/
  //SMCLK
  SelectClock = g_sClock.SMCLK.nHZ/1000;
  WDT_CounterValue = ms*SelectClock;
  while((WDT_CounterValue > WDT_Counter[smclk_DividerValue]) && (smclk_DividerValue > 0))
  {
    smclk_DividerValue --;
  }
  if(smclk_DividerValue == 7 || smclk_DividerValue == 0)
  {
    smclk_OffectTimeTemp = (WDT_Counter[smclk_DividerValue]-WDT_CounterValue);
  }
  else //������ʱ����ѡ�����ʱ����̵�
  {
    if((WDT_Counter[smclk_DividerValue ]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[smclk_DividerValue +1]))
    {
      smclk_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++smclk_DividerValue ]);
    }
    else
    {
      smclk_OffectTimeTemp = (WDT_Counter[smclk_DividerValue ]-WDT_CounterValue);
    }
  }
  smclk_OffectTimeTemp = smclk_OffectTimeTemp>0 ? smclk_OffectTimeTemp : -smclk_OffectTimeTemp;
  smclk_OffectTime = (float)smclk_OffectTimeTemp/(float)g_sClock.SMCLK.nHZ;
  /*ACLK*/
  SelectClock = g_sClock.ACLK.nHZ;
  WDT_CounterValue = ms*SelectClock/1000;
  while((WDT_CounterValue > WDT_Counter[aclk_DividerValue]) && (aclk_DividerValue > 0))
  {
    aclk_DividerValue--;
  }
  if(aclk_DividerValue == 7 || aclk_DividerValue == 0)
  {
    aclk_OffectTimeTemp = (WDT_Counter[aclk_DividerValue]-WDT_CounterValue);
  }
  else //������ʱ����ѡ�����ʱ����̵�
  {
    if((WDT_Counter[aclk_DividerValue ]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[aclk_DividerValue +1]))
    {
      aclk_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++aclk_DividerValue]);
    }
    else
    {
      aclk_OffectTimeTemp = (WDT_Counter[aclk_DividerValue]-WDT_CounterValue);
    }  
  }
  aclk_OffectTimeTemp = aclk_OffectTimeTemp>0 ? aclk_OffectTimeTemp : -aclk_OffectTimeTemp;
  aclk_OffectTime = (float)aclk_OffectTimeTemp/(float)g_sClock.ACLK.nHZ;
  /**VLOCLK_10K*/
  SelectClock = VLOCLK_FREQUENCY;
  WDT_CounterValue = ms*SelectClock/1000;
  while((WDT_CounterValue > WDT_Counter[vlo_DividerValue]) && (vlo_DividerValue > 0))
  {
    vlo_DividerValue--;
  }
  if(vlo_DividerValue == 7 || vlo_DividerValue == 0)
  {
    vlo_OffectTimeTemp = (WDT_Counter[vlo_DividerValue]-WDT_CounterValue);
  }
  else //������ʱ����ѡ�����ʱ����̵�
  {
    if((WDT_Counter[vlo_DividerValue]-WDT_CounterValue) > (WDT_CounterValue-WDT_Counter[vlo_DividerValue+1]))
    {
      vlo_OffectTimeTemp = (WDT_CounterValue-WDT_Counter[++vlo_DividerValue]);
    }
    else
    {
      vlo_OffectTimeTemp = (WDT_Counter[vlo_DividerValue]-WDT_CounterValue);
    }
  }
  vlo_OffectTimeTemp = vlo_OffectTimeTemp>0 ? vlo_OffectTimeTemp : -vlo_OffectTimeTemp;
  vlo_OffectTime = (float)vlo_OffectTimeTemp/(float)VLOCLK_FREQUENCY; 
  /*********�Ƚϸ���ʱ��Դ�Ķ�ʱ���ȣ�ѡ����߾��ȵ�ʱ��Դ����ʱʱ��**********/
  if((smclk_OffectTime <= aclk_OffectTime) && (smclk_OffectTime <= vlo_OffectTime))
  {
    return (WDTSSEL__SMCLK | smclk_DividerValue);
  }
  else if((aclk_OffectTime <= smclk_OffectTime) && (aclk_OffectTime <= vlo_OffectTime))
  {
    return (WDTSSEL__ACLK | aclk_DividerValue);
  }
  else
  {
    return (WDTSSEL__VLO | vlo_DividerValue);
  }
}