#include "msp430f5529_timer.h"
#include "msp430f5529_clock.h"
#include "msp430f5529_gpio.h"
#include "msp430f5529_system.h"
#include "stdarg.h"

//����4��ָ�����鱣�� TIMERX �ĵ�ַ
const TIMER_MemMapPtr TIMERX[TIMER_NUM] ={TIMER_A0_BASE_PTR,TIMER_A1_BASE_PTR,TIMER_A2_BASE_PTR,TIMER_B0_BASE_PTR};
//�������鱣��ռ�ձ����ֵ
const uint32_t TIMER_PRECISON[TIMER_NUM]={TIMER_PWM_A0_PRECISON,TIMER_PWM_A1_PRECISON,TIMER_PWM_A2_PRECISON,TIMER_PWM_B0_PRECISON};
//����ͨ������
const GPIO_PIN TIMER_CHANEL_PIN[TIMER_NUM][TIMER_CH_NUM]=
{// CH0    CH1    CH2    CH3    CH4    CH5    CH6
  {{P1,1},{P1,2},{P1,3},{P1,4},{P1,5}},                  //TIMER_A0
  {{P1,7},{P2,0},{P2,1}},                                //TIMER_A1
  {{P2,3},{P2,4},{P2,5}},                                //TIMER_A2
  {{P5,6},{P5,7},{P7,4},{P7,5},{P7,6},{P3,5},{P3,6}}     //TIMER_B0
};//ͨ������
const GPIO_PIN TIMER_CLK_PIN[TIMER_NUM] =
{
  {P1,0},                                                //TIMER_A0
  {P1,6},                                                //TIMER_A1
  {P2,2},                                                //TIMER_A2
  {P7,7}                                                 //TIMER_B0
};//�ⲿʱ����������
/*******************************************************************************
*  �������ƣ�TIMER_PWM_MultiChannel_Init(TIMERn timer, uint32_t fre, uint8_t ChannelNum,...)
*  ����˵������ʱ��PWM�����ʼ��(���Զ��ͨ��ͬʱ��ʼ��)
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint32_t fre         ��Ƶ��
             uint8_t ChannelNum   ��Ҫ��ʼ����ͨ����
             ...                ����ʼ����ͨ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�ʵ��һ����ʹ�ú�����ʼ��ʱ��Ҫ�����ʼ��ͨ��������ʼ��ͨ��������Ϊ0���磺TIMER_PWM_MultiChannel_Init(TIMER_A0, 1000, 0);��
             TIMER_PWM_MultiChannel_Init(TIMER_A0, 1000, 3, TIMER_CH1, TIMER_CH2, TIMER_CH4);
             //��ʱ��TIMER_A0��ʼ�����PWM����Ƶ��Ϊ1000Hz,��ʼ��3��ͨ�����ֱ��ǣ�TIMER_CH1,TIMER_CH2,TIMER_CH4
             ʵ������(ʹ�ú궨���ʼ��ʱ����Ҫ�����ʼ��ͨ����)
             TIMER_PWM_Init(TIMER_B0,1000,TIMER_CH1,TIMER_CH2,TIMER_CH3,TIMER_CH4,TIMER_CH5,TIMER_CH6);
             //��ʱ��TIMER_B0��ʼ�����PWM����Ƶ��Ϊ1000Hz,��ʼ��6��ͨ����TIMER_CH1,TIMER_CH2,TIMER_CH3,TIMER_CH4,TIMER_CH5,TIMER_CH6
********************************************************************************/
STATUS TIMER_PWM_MultiChannel_Init(TIMERn timer, uint32_t fre, uint8_t ChannelNum,...)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_PWM_MultiChannel_Init",
         "��ʱ���Ѿ�����ΪĬ����ʱ���ܣ���������Ϊ��������ʹ��");       //��������������»��궨��ö�ʱ����Ϊ��ʱ���õĶ�ʱ��������system.h���޸�DELAY_TIMER�궨��
#endif
  ASSERT((timer==TIMER_A0&&ChannelNum<=TIMER_CH4)
       ||(timer==TIMER_A1&&ChannelNum<=TIMER_CH2)
       ||(timer==TIMER_A2&&ChannelNum<=TIMER_CH2)
       ||(timer==TIMER_B0&&ChannelNum<=TIMER_CH6),
       "TIMER_PWM_MultiChannel_Init",
       "��ʱ��ͨ������������Χ��"); //���Լ���������*/
  
  va_list ap;
  va_start(ap,ChanelNum);
  for(int i=0;i < ChannelNum;i++)           //���ݳ�ʼ����ͨ��������һ��ʼ��ͨ��
  {
    TIMER_CHn ch = va_arg(ap, TIMER_CHn);  //��ȡҪ��ʼ����ͨ��
    if((ch == TIMER_CH0) || (ch >= TIMER_CH_NUM))   //�ж�ͨ��ֵ�Ƿ����
    {
      return ERROR;                                 //��������ERROR
    }
    //��ʼ���˿�
    GPIO_Init(TIMER_CHANEL_PIN[timer][ch].Port,TIMER_CHANEL_PIN[timer][ch].Pin,GPO|SEL);
    //��ʼ��ռ�ձȼ��������
    TIMERX[timer]->CCR[ch] = 0u;
    TIMERX[timer]->CCTL[ch].IE = RESET;               //�ر�ͨ���ж�
    TIMERX[timer]->CCTL[ch].OUTMOD = DEFAULT_PWM_OUTMOD;      //�����������
    TIMERX[timer]->CCTL[ch].CAPMODE = RESET;          //�Ƚ�ģʽ
  }
  va_end(ap);
  
  return TIMER_PWM_SetFrequency(timer,fre);
}
/*******************************************************************************
*  �������ƣ�TIMER_PWM_SetChannelOutmod(TIMERn timer,TIMER_CHn ch,TIMER_OUTMODn outmod)
*  ����˵����PWM�����������
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
             TIMER_PWM_OUTMODn outmod ���������ģʽ
*  �������أ���
*  ʹ�����̣�TIMER_PWM_SetChannelOutmod(TIMER_A0,TIMER_CH1,TIMER_PWM_OUTMOD3);  //�޸Ķ�ʱ��A0��TIMER_CH1ͨ���������
********************************************************************************/
void TIMER_PWM_SetChannelOutmod(TIMERn timer,TIMER_CHn ch,TIMER_PWM_OUTMODn outmod)
{
  TIMERX[timer]->CCTL[ch].OUTMOD = outmod;      //�����������
}
/*******************************************************************************
*  �������ƣ�TIMER_PWM_SetFrequency(TIMERn timer,uint32_t fre)
*  ����˵������ʱ��PWMƵ������(ע������û�и��ĸ���ͨ����ռ�ձȣ������޸�Ƶ�ʺ�ռ�ձȱ��ˣ���Ҫ��������ռ�ձ�)
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint32_t fre         ��Ƶ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_PWM_SetFrequency(TIMER_B0,1000);       //����ʱ��B���Ƶ���޸�Ϊ1000Hz
********************************************************************************/
STATUS  TIMER_PWM_SetFrequency(TIMERn timer,uint32_t fre)
{
  if(fre == 0u)
  {
    TIMERX[timer]->MC = TIMER_MC_STOP;              //��ͣ���PWM��
    return SUCCESS;
  }
  
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;           //��ѡ��ʱ��ԴΪSMCLK
  TIMERX[timer]->ID = 0u;                           //��Ƶϵ������
  //ʱ�Ӵ�С
  uint32_t Counter_Value = g_sClock.SMCLK.nHZ/fre;                 //�������ֵ
  while(Counter_Value > 65536u)        //���ﲻ��0xffff����Ϊ�����-1��������(0xffff+1)=65536
  {  //��Ƶһ��
     TIMERX[timer]->ID ++;          
     Counter_Value >>= 1;          
     
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //���ʱ����ζ��ѡ�����Ƶ�ʵ�ʱ����
     {
       if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK)   //���ʱ���Ѿ�����͵�ACLK�ˣ���ô��ζ�����õ�Ƶ��̫���ˣ�ʧ�ܷ���ERROR
       {
         return ERROR;
       }
       //����Ϊ���͵�ACLK����������
       TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
       TIMERX[timer]->ID = 0u;  
       Counter_Value = g_sClock.ACLK.nHZ/fre; 
       continue;  
     } 
  }
  if(Counter_Value <= 1)//�����������,˵��Ƶ�ʲ�����
  {
    return ERROR; 
  }

  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//ΪʲôҪ��һ������ΪTAR/TBR��������CCR0�仯��0����һ����������
  TIMERX[timer]->IE = RESET;                      //�ر��ж�
  TIMERX[timer]->MC = TIMER_MC_UP;                //������ģʽ
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_PWM_SetChannelDuty(TIMERn timer,TIMER_CHn ch,uint32_t duty)
*  ����˵����PWM���ռ�ձ�����
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
             uint32_t duty      ��ռ�ձ�
*  �������أ���
*  ʹ�����̣�TIMER_PWM_SetChannelDuty(TIMER_A0,TIMER_CH1,800);  //�޸Ķ�ʱ��A0��TIMER_CH1ͨ��ռ�ձ�Ϊ(800/TIMAR_A0_PRECISON)*100%
********************************************************************************/
void  TIMER_PWM_SetChannelDuty(TIMERn timer,TIMER_CHn ch,uint32_t duty)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_PWM_SetChannelDuty",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
  
  ASSERT(duty <= TIMER_PRECISON[timer],
         "TIMER_PWM_SetChannelDuty",
         "��ʱ��ռ�ձȲ���������Χ��");      //�ö��Լ�� ռ�ձ��Ƿ����
  
  TIMERX[timer]->CCR[ch] = (uint16_t)((duty * (TIMERX[timer]->CCR[0]+1))/TIMER_PRECISON[timer]);
}
/*******************************************************************************
*  �������ƣ�TIMER_Interval_Us(TIMERn timer,uint32_t us)
*  ����˵������ʱ����ʱ��ʼ��(��λ��us)
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint32_t us          : ��ʱʱ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_Interval_Us(TIMER_B0,2500);  //TB��ʱ���2.5ms�ж�
********************************************************************************/
STATUS  TIMER_Interval_Us(TIMERn timer,uint32_t us)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Interval_Us",
         "��ʱ���Ѿ�����ΪĬ����ʱ���ܣ���������Ϊ��������ʹ��");       //��������������»��궨��ö�ʱ����Ϊ��ʱ���õĶ�ʱ��������system.h���޸�DELAY_TIMER�궨��
#endif
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u;
  //����ֵ
  uint32_t Counter_Value = (uint32_t)(us * g_sClock.SMCLK.fMHZ);
  while(Counter_Value > 65536)
  {
     TIMERX[timer]->ID ++; 
     Counter_Value >>= 1;  
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //���ʱ����ζ��ѡ�����Ƶ�ʵ�ʱ����
     {
      if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK) //���ʱ���Ѿ�����͵�ACLK�ˣ���ô��ζ�����õ�Ƶ��̫���ˣ�ʧ�ܷ���ERROR
      {
        return ERROR;
      }
      //����Ϊ���͵�ACLK����������
      TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
      TIMERX[timer]->ID = 0u;  
      Counter_Value = (uint32_t)(us * g_sClock.ACLK.fMHZ);
      continue;  
     } 
  }
  if(Counter_Value <= 1)//�����������,˵��Ƶ�ʲ�����
  {
    return ERROR; 
  }
  
  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//ΪʲôҪ��һ������ΪTAR/TBR��������CCR0�仯��0����һ����������
  TIMERX[timer]->CCTL[0].CAPMODE = RESET;          //�Ƚ�ģʽ
  TIMERX[timer]->IE = RESET;             //�ر�����ж�
  TIMERX[timer]->CCTL[0].IE = RESET;     //�ر�ͨ���ж�
  TIMERX[timer]->MC = TIMER_MC_UP;      //������ģʽ
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Delay_Us(TIMERn timer,uint32_t us)
*  ����˵������ʱ����ʱ����λ��ms��
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint32_t us          : ��ʱʱ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_Delay_Us(TIMER_B0 ,5000);      //�ڴ˴���ʱ5ms
********************************************************************************/
STATUS TIMER_Delay_Us(TIMERn timer,uint32_t us)
{
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u;
  //����ֵ
  uint32_t Counter_Value = (uint32_t)(us * g_sClock.SMCLK.fMHZ);
  while(Counter_Value > 65536)
  {
     TIMERX[timer]->ID ++; 
     Counter_Value >>= 1;  
     if((TIMERX[timer]->ID == 3u) && (Counter_Value > 65536))   //���ʱ����ζ��ѡ�����Ƶ�ʵ�ʱ����
     {
      if(TIMERX[timer]->SSEL == TIMER_SSEL_ACLK) //���ʱ���Ѿ�����͵�ACLK�ˣ���ô��ζ�����õ�Ƶ��̫���ˣ�ʧ�ܷ���ERROR
      {
        return ERROR;
      }
      //����Ϊ���͵�ACLK����������
      TIMERX[timer]->SSEL = TIMER_SSEL_ACLK;
      TIMERX[timer]->ID = 0u;  
      Counter_Value = (uint32_t)(us * g_sClock.ACLK.fMHZ);
      continue;  
     } 
  }
  if(Counter_Value <= 1)//�����������,˵��Ƶ�ʲ�����
  {
    return ERROR; 
  }
  
  TIMERX[timer]->CCR[0] = (uint16_t)(Counter_Value - 1u);//ΪʲôҪ��һ������ΪTAR/TBR��������CCR0�仯��0����һ����������
  TIMERX[timer]->CCTL[0].CAPMODE = RESET;          //�Ƚ�ģʽ
  TIMERX[timer]->IE = RESET;             //�ر�����ж�
  TIMERX[timer]->CCTL[0].IE = RESET;     //�ر�ͨ���ж�
  TIMERX[timer]->MC = TIMER_MC_UP;
  
  TIMERX[timer]->CCTL[0].CTL = 0;
  TIMERX[timer]->IFG = RESET;
  while(TIMERX[timer]->IFG == RESET);
  TIMERX[timer]->CTL = 0;
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Pluse_Init    (TIMERn timer)
*  ����˵������ʱ�����������ʼ��
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_Pluse_Init    (TIMER_A1);    //��ʱ��TA1��ʼ��Ϊ�����������
********************************************************************************/
STATUS  TIMER_Pluse_Init    (TIMERn timer)
{ 
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Pluse_Init",
         "��ʱ���Ѿ�����ΪĬ����ʱ���ܣ���������Ϊ��������ʹ��");       //��������������»��궨��ö�ʱ����Ϊ��ʱ���õĶ�ʱ��������system.h���޸�DELAY_TIMER�궨��
#endif 
  //�˿�����Ϊ���뷽��//ѡ��ڶ�����
  GPIO_Init(TIMER_CLK_PIN[timer].Port,TIMER_CLK_PIN[timer].Pin,GPI|SEL);
  
  TIMERX[timer]->CLR = BIT_SET;                       
  TIMERX[timer]->SSEL = TIMER_SSEL_INCLK;             //ʱ��Դѡ��Ϊ�ⲿ����
  TIMERX[timer]->ID = 0u;                             //����Ƶ
  TIMERX[timer]->MC = TIMER_MC_CONTINOUS;             //��������ģʽ
  TIMERX[timer]->IE = RESET;                          //��ֹ����ж�
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Pluse_GetValue(TIMERn timer,uint16_t overflowTime)
*  ����˵������ʱ���������ֵ��ȡ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint16_t overflowTime ���������������
*  �������أ���ʱ���������ֵ
*  ʹ�����̣�
             int32 pulse = TIMER_Pluse_GetValue(TIMER_A1,0);    //��û�з������������¶�ȡ�������
             TIMER_Pluse_Clear   (TIMER_A1);                    //��ȡ��Ӧ��Ҫ����
********************************************************************************/
uint32_t TIMER_Pluse_GetValue(TIMERn timer,uint16_t overflowTime)
{
  return (TIMERX[timer]->R + (overflowTime*65536u));
}
/*******************************************************************************
*  �������ƣ�TIMER_Pluse_Clear   (TIMERn timer)
*  ����˵������ʱ���������ֵ����
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ���
*  ʹ�����̣�
             int32 pulse = TIMER_Pluse_Read    (TIMER_A1,0);    //��û�з������������¶�ȡ�������
             TIMER_Pluse_Clear   (TIMER_A1);                    //��ȡ��Ӧ��Ҫ����
********************************************************************************/
void   TIMER_Pluse_Clear   (TIMERn timer)
{
  TIMERX[timer]->R = 0;
}
/*******************************************************************************
*  �������ƣ�TIMER_Capture_Init(TIMERn timer,TIMER_CHn ch,TIMER_CAPTUREn capmode)
*  ����˵������ʱ�������ʼ��
*  ����˵����TIMERn timer        ����ʱ��ģ��
             TIMER_CHn ch        ����ʱ��ͨ��
             TIMER_CAPTUREn capmode������ģʽ
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_Capture_Init(TIMER_A0,TIMER_CH1,CAP_Rising);         //��ʱ��TA0 CCR1ͨ����ʼ��Ϊ��Ե���������ز���
********************************************************************************/
STATUS TIMER_Capture_Init(TIMERn timer,TIMER_CHn ch,TIMER_CAPTUREn capmode)
{
#ifdef DELAY_TIMER
  ASSERT((timer != DELAY_TIMER),
         "TIMER_Capture_Init",
         "��ʱ���Ѿ�����ΪĬ����ʱ���ܣ���������Ϊ��������ʹ��");       //��������������»��궨��ö�ʱ����Ϊ��ʱ���õĶ�ʱ��������system.h���޸�DELAY_TIMER�궨��
#endif
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_Init",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
 
  TIMERX[timer]->CCTL[ch].CAPMODE = BIT_SET;                //��Ϊ����ģʽ
  TIMERX[timer]->CCTL[ch].CM = capmode;                 //ѡ�񲶻���
  
  TIMERX[timer]->CCTL[ch].CCIS = (timer == TIMER_B0) ? TIMER_CCIS_1 : TIMER_CCIS_0;
  
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;
  TIMERX[timer]->ID = 0u; //����ѡ����ϵͳʱ�ӣ�����Ƶ���ر��ʱ��ʹ�ø���ʱ��
  TIMERX[timer]->IE = RESET;              //�ر�����ж�
  TIMERX[timer]->CCTL[ch].IE = RESET;     //�ر�ͨ���ж�
  TIMERX[timer]->MC = TIMER_MC_CONTINOUS;             //��������ģʽ
  
  //�˿�����Ϊ���뷽,ѡ��ڶ�����
  GPIO_Init(TIMER_CHANEL_PIN[timer][ch].Port,TIMER_CHANEL_PIN[timer][ch].Pin,GPI|SEL);
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Capture_SetMode(TIMERn timer, TIMER_CHn ch, TIMER_CAPTUREn cap_mode)
*  ����˵������ʱ�������������
*  ����˵����TIMERn timer        ����ʱ��ģ��
             TIMER_CHn ch        ����ʱ��ͨ��
             TIMER_CAPTUREn capmode������ģʽ
*  �������أ���
*  ʹ�����̣�TIMER_Capture_SetMode(TIMER_A0,TIMER_CH1,CAP_Rising);         //��ʱ��TA0 CCR1ͨ����ʼ��Ϊ��Ե���������ز���
********************************************************************************/
void   TIMER_Capture_SetMode   (TIMERn timer, TIMER_CHn ch, TIMER_CAPTUREn cap_mode)
{
   TIMERX[timer]->CCTL[ch].CM = cap_mode;                 //���ò�����
}
/*******************************************************************************
*  �������ƣ�TIMER_Capture_Clear     (TIMERn timer,TIMER_CHn ch)
*  ����˵����ĳһͨ����ȡ��������
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
*  �������أ�
*  ʹ�����̣�int16 overflowTime;  //����������ڶ�ʱ������ж���Ը�ֵ��һ
             ......
             data=TIMER_Capture_GetTime_Us(TIMER_A0,TIMER_CH1,overflowTime)
             TIMER_Capture_Clear(TIMER_A0,TIMER_CH1);  //����
             overflowTime=0;       //����
********************************************************************************/
void   TIMER_Capture_Clear     (TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_Clear",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
  
  TIMERX[timer]->CCR[ch] = 0u;
  TIMERX[timer]->R = 0u;
}
/*******************************************************************************
*  �������ƣ�TIMER_Capture_GetValue(TIMERn timer,TIMER_CHn ch)
*  ����˵������ȡ��������ʱ��ʱ����ͨ������ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
*  �������أ�
*  ʹ�����̣�
             uint16_t value = TIMER_Capture_GetValue(TIMER_A0,TIMER_CH1);  //��ȡ��������ʱ��ʱ��
********************************************************************************/
uint16_t TIMER_Capture_GetValue(TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_Capture_GetValue",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
  
  return TIMERX[timer]->CCR[ch];
}
/*******************************************************************************
*  �������ƣ�TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime)
*  ����˵������ȡ��������ʱ��ʱ����ͨ������ֵ
*  ����˵����uint16_t start_value  ��������ʼֵ
             uint16_t stop_value   ���������ֵ
             uint16_t overflowTime ���������������
*  �������أ�
*  ʹ�����̣�uint16 overflowTime;  //����������ڶ�ʱ������ж���Ը�ֵ��һ
             ......
             TIMER_Capture_CalTime_Us(0,TIMER_Capture_GetValue  (TIMER_A0,TIMER_CH1),overflowTime);  //��ȡ��������ʱ��ʱ��
             overflowTime=0;       //����
********************************************************************************/
uint32_t TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime)
{
  return (uint32_t)((((int32_t)stop_value - (int32_t)start_value) + (uint32_t)overflowTime*65536)/g_sClock.SMCLK.fMHZ);
}
/*******************************************************************************
*  �������ƣ�TIMER_Timer_Init      (TIMERn timer)
*  ����˵������ʱ���ܳ�ʼ��
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ����ɹ��򷵻�ERROR���ɹ��򷵻�SUCCESS
*  ʹ�����̣�TIMER_Timer_Init      (TIMER_A1);     //��ʱ��TA1��ʼ��Ϊ��ʱ��
********************************************************************************/
STATUS   TIMER_Timer_Init      (TIMERn timer)
{
#ifdef DELAY_TIMER
 ASSERT((timer != DELAY_TIMER),
         "TIMER_Timer_Init",
         "��ʱ���Ѿ�����ΪĬ����ʱ���ܣ���������Ϊ��������ʹ��");       //��������������»��궨��ö�ʱ����Ϊ��ʱ���õĶ�ʱ��������system.h���޸�DELAY_TIMER�궨��
#endif
  TIMERX[timer]->CLR = BIT_SET;
  TIMERX[timer]->SSEL = TIMER_SSEL_SMCLK;             //����ѡ����ϵͳʱ��
  TIMERX[timer]->ID = 0u; 
  TIMERX[timer]->MC = TIMER_MC_STOP;                  //��ֹͣ��������
  TIMERX[timer]->IE = RESET;                          //��ֹ����ж�
  
  return SUCCESS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Timer_Start(TIMERn timer)
*  ����˵������ʱ���ܿ�ʼ��ʱ
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ���
*  ʹ�����̣�TIMER_Timer_Start(TIMER_A1);      //��ʱ����ʼ��ʱ
********************************************************************************/
void   TIMER_Timer_Start(TIMERn timer)
{
  TIMERX[timer]->R = 0u;
  TIMERX[timer]->MC = TIMER_MC_CONTINUOUS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Timer_Stop (TIMERn timer)
*  ����˵������ʱ����ֹͣ��ʱ
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ����ص�ǰ����ֵ
*  ʹ�����̣�TIMER_Timer_Stop (TIMER_A1);   //��ʱ��ֹͣ��ʱ
********************************************************************************/
uint16_t   TIMER_Timer_Stop (TIMERn timer)
{
   TIMERX[timer]->MC = TIMER_MC_STOP;
   return TIMERX[timer]->R;
}
/*******************************************************************************
*  �������ƣ�TIMER_Timer_GetTime_Us(TIMERn timer,uint16_t overflowTime)
*  ����˵������ʱʱ���ȡ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint16_t overflowTime ���������������
*  �������أ����ؼ�ʱʱ�� ����λ��us��
*  ʹ�����̣�int16 overflowTime;  //����������ڶ�ʱ������ж���Ը�ֵ��һ
             ......
             TIMER_Timer_GetTime_Us(TIMAER_A1,overflowTime);  //��ȡ��ʱʱ��
             overflowTime=0;       //����
********************************************************************************/
uint32_t TIMER_Timer_GetTime_Us(TIMERn timer,uint16_t overflowTime)
{
  return (uint32_t)((TIMERX[timer]->R + overflowTime*65536u)/g_sClock.SMCLK.fMHZ);
}
/*******************************************************************************
*  �������ƣ�TIMER_Run (TIMERn timer)
*  ����˵������ʱ������
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ���
*  ʹ�����̣�TIMER_Clear_Counter (TIMAER_A1)��  //������ʱ��A1
********************************************************************************/
void   TIMER_Run (TIMERn timer)
{
  TIMERX[timer]->MC = TIMER_MC_CONTINUOUS;
}
/*******************************************************************************
*  �������ƣ�TIMER_Stop (TIMERn timer)
*  ����˵������ʱ������
*  ����˵����TIMERn timer       ����ʱ��ģ��
*  �������أ���
*  ʹ�����̣�TIMER_Stop(TIMAER_A1)��  //ֹͣ��ʱ��A1
********************************************************************************/
void   TIMER_Stop (TIMERn timer)
{
  TIMERX[timer]->MC = TIMER_MC_STOP;
}
/*******************************************************************************
*  �������ƣ�TIMER_GetCounterValue (TIMERn timer)
*  ����˵������ȡ��ʱ������ֵ
*  ����˵����TIMERn timer  ����ʱ��ģ��
*  �������أ�uint16_t      ������ֵ
*  ʹ�����̣�uint16_t value = TIMER_GetCounterValue(TIMAER_A1)��  //��ȡ��ʱ��A1����ֵ
********************************************************************************/
uint16_t   TIMER_GetCounterValue (TIMERn timer)
{
  return TIMERX[timer]->R;
}
/*******************************************************************************
*  �������ƣ�TIMER_SetCounterValue(TIMERn timer,uint16_t value)
*  ����˵�������ö�ʱ������ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             uint16_t value     ������ֵ
*  �������أ���
*  ʹ�����̣�TIMER_SetCounterValue(TIMAER_A1,0)��  //���ö�ʱ��A1����ֵΪ0
********************************************************************************/
void   TIMER_SetCounterValue (TIMERn timer,uint16_t value)
{
  TIMERX[timer]->R = value;
}
/*******************************************************************************
*  �������ƣ�TIMER_GetChannelCounterValue (TIMERn timer,TIMER_CHn ch)
*  ����˵������ȡ��ʱ��ͨ������ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��             
*  �������أ�uint16_t value     ����ͨ���ļ���ֵ
*  ʹ�����̣�uint16_t value = TIMER_GetChannelCounterValue(TIMAER_A1,TIMER_CH4)��  //��ȡ��ʱ��A1ͨ��4����ֵ
********************************************************************************/
uint16_t   TIMER_GetChannelCounterValue (TIMERn timer,TIMER_CHn ch)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_GetChannelCounterValue",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
  
  return TIMERX[timer]->CCR[ch];
}
/*******************************************************************************
*  �������ƣ�TIMER_SetChannelCounterValue(TIMERn timer,uint16_t value)
*  ����˵�������ö�ʱ������ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
             uint16_t value     ������ֵ
*  �������أ���
*  ʹ�����̣�TIMER_SetChannelCounterValue(TIMAER_A1,TIMER_CH4,0)��  //���ö�ʱ��A1ͨ��4����ֵΪ0
********************************************************************************/
void   TIMER_SetChannelCounterValue (TIMERn timer,TIMER_CHn ch,uint16_t value)
{
  ASSERT((timer==TIMER_A0&&ch<=TIMER_CH4)
       ||(timer==TIMER_A1&&ch<=TIMER_CH2)
       ||(timer==TIMER_A2&&ch<=TIMER_CH2)
       ||(timer==TIMER_B0&&ch<=TIMER_CH6),
       "TIMER_SetChannelCounterValue",
       "��ʱ��ͨ������������Χ��"); //���Լ���������
  
  TIMERX[timer]->CCR[ch] = value;
}
/*******************************************************************************
*  �������ƣ�TIMER_GetChannelPinInValue(TIMERn timer, TIMER_CHn ch)
*  ����˵������ȡͨ������ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
*  �������أ�STATUS : BIT_SET-����ߵ�ƽ RESET-����͵�ƽ
*  ʹ�����̣�STATUS status = TIMER_GetChannelInValue(TIMAER_A1, TIMER_CH1);  //��ȡ��ʱ��A1ͨ��1�����ƽ
********************************************************************************/
STATUS TIMER_GetChannelPinInValue (TIMERn timer, TIMER_CHn ch)
{
  return (TIMERX[timer]->CCTL[ch].CH_CCI) ? BIT_SET : RESET;
}
/*******************************************************************************
*  �������ƣ�TIMER_GetChannelPinOutValue(TIMERn timer, TIMER_CHn ch)
*  ����˵������ȡͨ�����ֵ
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_CHn ch       ����ʱ��ͨ��
*  �������أ�STATUS : BIT_SET-����ߵ�ƽ RESET-����͵�ƽ
*  ʹ�����̣�STATUS status = TIMER_GetChannelOutValue(TIMAER_A1, TIMER_CH1);  //��ȡ��ʱ��A1ͨ��1�����ƽ
********************************************************************************/
STATUS TIMER_GetChannelPinOutValue(TIMERn timer, TIMER_CHn ch)
{
  return (TIMERX[timer]->CCTL[ch].CH_OUT) ? BIT_SET : RESET;
}
/*******************************************************************************
*  �������ƣ�TIMER_ITConfig (TIMERn timer,TIMER_IRQn irqn,STATUS ITState)
*  ����˵���������Ƿ�ʹ��TIMER��ĳһ���ж�
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_IRQn irqn    ���ж��¼�
             STATUS ITState     ���Ƿ�ʹ���ж�
*  �������أ���
*  ʹ�����̣�TIMER_ITConfig (TIMAER_A1,TIMER_CCR0_IRQn,ENABLE); //ʹ�ܶ�ʱ��1ͨ��0�жϣ�����ʱ�ж�
********************************************************************************/
void  TIMER_ITConfig (TIMERn timer,TIMER_IRQn irqn,STATUS ITState)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    TIMERX[timer]->IE = ITState;
  }
  else
  {
    TIMERX[timer]->CCTL[irqn].IE = ITState;
  }
}
/*******************************************************************************
*  �������ƣ�TIMER_GetITStatus(TIMERn timer,TIMER_IRQn irqn)
*  ����˵������ȡTIMER��ĳһ���жϱ�־
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_IRQn irqn    ���ж��¼�
*  �������أ�STATUS : TRUE-�ж��¼����� FALSE-�ж��¼�δ����
*  ʹ�����̣�if(TRUE == TIMER_GetITStatus(TIMAER_A1,TIMER_CCR1_IRQn)){...} //�ж϶�ʱ��A1ͨ��1�ж��¼��Ƿ���
********************************************************************************/
STATUS TIMER_GetITStatus(TIMERn timer,TIMER_IRQn irqn)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    return ((TIMERX[timer]->IFG == BIT_SET) ? TRUE : FALSE);
  }
  else
  {
    return ((TIMERX[timer]->CCTL[irqn].IFG == BIT_SET) ? TRUE : FALSE);
  }
}
/*******************************************************************************
*  �������ƣ�TIMER_ClearITPendingBit(TIMERn timer,TIMER_IRQn irqn)
*  ����˵�������TIMER��ĳһ���жϱ�־
*  ����˵����TIMERn timer       ����ʱ��ģ��
             TIMER_IRQn irqn    ���ж��¼�
*  �������أ���
*  ʹ�����̣�TIMER_ClearITPendingBit(TIMAER_A1,TIMER_CCR1_IRQn);   //�����ʱ��ͨ��1�жϱ�־λ
********************************************************************************/
void  TIMER_ClearITPendingBit(TIMERn timer,TIMER_IRQn irqn)
{
  if(irqn == TIMER_OverFlow_IRQn)
  {
    TIMERX[timer]->IFG = RESET;
  }
  else
  {
    TIMERX[timer]->CCTL[irqn].IFG = RESET;
  }
}