#include "msp430f5529_rtc.h"
#include "msp430f5529_clock.h"

const RTC_MemMapPtr  RTC = RTC_BASE_PTR; 
/*******************************************************************************
*  �������ƣ�RTC_Init(Calendar nowDate)
*  ����˵����RTC ��ʼ��������ʱ��
*  ����˵����Calendar nowDate : ���õ�ǰʱ��
*  �������أ���
*  ʹ��ʾ����Calendar  nowDate;RTC_Init(nowDate);//��ʼ��RTC������RTCʱ��
*  ע�⣺����ģʽ��Ҫ��ACLKΪ32768HZ
********************************************************************************/
void RTC_Init(Calendar nowDate)
{
  ASSERT(g_sClock.ACLK.nHZ == 32768,"RTC_Init","ʱ�����ò������뽫ACLKƵ������Ϊ32768HZ��");                //����ģʽ��Ҫ��ACLKΪ32768HZ
  /*uint32_t clock_div_result = g_sClock.ACLK.nHZ;          
  uint16_t clock_div_value = 0;//����Ƶ����
  while(clock_div_result>1)  //���Ϸ�Ƶ��ֱ����Ƶ���ֵΪ1��С��1��Ҫ��Ϊ1��
  {
    clock_div_result = clock_div_result>>1; //����Ƶһ��
    clock_div_value++;                      //��Ƶϵ����һ
  };
  ASSERT((clock_div_result==1)&&(clock_div_value>=2)&&(clock_div_value<=16));        //Ҫ��ʱ�Ӷ���Ƶ���16�Σ�������2�Σ��ҷ�Ƶ���Ƶ��ֵΪ1HZ
  */
  RTCCTL01 = RTCHOLD + RTCMODE + RTCSSEL__RT1PS + RTC_TimeEvent;           //ʹ������ģʽ
  
  RTCPS0CTL =   RT0PSHOLD ;             //ʹ��ACLK��Ϊʱ��
  RTCPS1CTL  =  RT1PSHOLD + RT1SSEL_2; //ʹ�� RTCPS0�����Ϊʱ��Դ     
  
  //RTC->RT1_PSDIV = (clock_div_value <= 9) ? (clock_div_value-2u) : 7u;
  //RTC->RT0_PSDIV = (clock_div_value >  9) ? (clock_div_value-9u) : 0u;
  
  RTC->HOLD = RESET;                 //��RTCģ�� 
  RTC->RT0_PSHOLD = RESET;           //��RTCPS0CTL
  RTC->RT1_PSHOLD = RESET;           //��RTCPS1CTL
  
  RTC_SetTime(nowDate);    
}
/*******************************************************************************
*  �������ƣ�RTC_SetTime(Calendar nowDate)
*  ����˵�������õ�ǰʱ��
*  ����˵����Calendar nowDate : ��ǰʱ��
*  �������أ���
*  ʹ��ʾ����RTC_SetTime(nowDate);  //����RTCʱ��
********************************************************************************/
void RTC_SetTime(Calendar nowDate)
{
  (*((Calendar *)(&RTCSEC))) = nowDate;  
}
/*******************************************************************************
*  �������ƣ�RTC_GetTime(void)
*  ����˵������ȡ��ǰʱ��
*  ����˵������
*  �������أ�Calendar nowDate : ��ǰʱ��
*  ʹ��ʾ����Calendar nowDate = RTC_GetTime();  //��ȡ��ǰʱ��
********************************************************************************/
Calendar RTC_GetTime(void)
{
  while(RTC->RDY == RESET);                  //�ȴ�ʱ��׼���ö�
  return (*((Calendar *)(&RTCSEC)));         //��Ϊ�üĴ����ṹ��Calendar�ṹ����ͬ��ֱ�ӽ��Ĵ���ֵ����
}
/*******************************************************************************
*  �������ƣ�RTC_SetAlarm    (Calendar alarm_Time,Alarm_Type alarm_type)
*  ����˵������������ʱ�估����
*  ����˵����Calendar alarm_Time     :����ʱ��
             Alarm_Type alarm_type   :��������
*  �������أ���
*  ʹ��ʾ����RTC_SetAlarm (alarm_Time,Every_Days); //����ʱ�����ã�ÿ�촥��һ�������¼�
********************************************************************************/
void RTC_SetAlarm    (Calendar alarm_Time,Alarm_Type alarm_type)
{
  //д������ʱ��
  RTCAMIN  = alarm_Time.Minutes    ;
  RTCAHOUR = alarm_Time.Hours      ;
  RTCADAY  = alarm_Time.DayOfMonth ; 
  
  BIT_VALUE_IN_BYTE(RTCAMIN,7) = BIT_SET;
  BIT_VALUE_IN_BYTE(RTCAHOUR,7) = (alarm_type == Every_Days  ||
                              alarm_type == Every_Weeks ||
                              alarm_type == Every_Months 
                              ) ? BIT_SET : RESET;
  BIT_VALUE_IN_BYTE(RTCADOW,7) = alarm_type == Every_Weeks  ? BIT_SET : RESET;
  BIT_VALUE_IN_BYTE(RTCADAY,7) = alarm_type == Every_Months ? BIT_SET : RESET;
}
/*******************************************************************************
*  �������ƣ�RTC_ITConfig   (RTC_IRQn irqn, STATUS ITState)
*  ����˵��: ����RTCĳһ���ж��Ƿ�ʹ��
*  ����˵����RTC_IRQn irqn   : �ж��¼�ѡ��
             STATUS ITState  : �Ƿ�ʹ���ж�
*  �������أ���
*  ʹ��ʾ����RTC_ITConfig   (RTC_Alarm_IRQn, ENABLE);  //ʹ�������ж�
********************************************************************************/
void RTC_ITConfig   (RTC_IRQn irqn, STATUS ITState)
{
  if(ITState != DISABLE)
  {
    RTC->IE |= irqn;
  }
  else
  {
    RTC->IE &=~ irqn;
  }
}
/*******************************************************************************
*  �������ƣ�RTC_GetITStatus(RTC_IRQn irqn)
*  ����˵������ȡRTCĳһλ�жϱ�־
*  ����˵����RTC_IRQn irqn : �ж��¼�ѡ��
*  �������أ�STATUS : TRUE �ж��¼�������FALSE �ж��¼�δ����
*  ʹ��ʾ����if(TRUE == RTC_GetITStatus(RTC_Alarm_IRQn)){...}  //�ж�����ʱ���Ƿ���
********************************************************************************/
STATUS RTC_GetITStatus(RTC_IRQn irqn)
{
  return ((RTC->IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  �������ƣ�RTC_ClearITPendingBit(RTC_IRQn irqn)
*  ����˵�������RTCĳһλ�жϱ�־
*  ����˵����RTC_IRQn irqn : �ж��¼�ѡ��
*  �������أ���
*  ʹ��ʾ����RTC_ClearITPendingBit(RTC_Alarm_IRQn); //���RTC�����жϱ�־λ
********************************************************************************/
void RTC_ClearITPendingBit(RTC_IRQn irqn)
{
  RTC->IFG &=~ irqn;
}
