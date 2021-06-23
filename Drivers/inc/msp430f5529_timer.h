#ifndef TIMER_H_
#define TIMER_H_

#include"common.h"

typedef enum 
{
  TIMER_A0              ,
  TIMER_A1              ,
  TIMER_A2              ,
  TIMER_B0              ,
  TIMER_NUM             ,
}TIMERn;             //��ʱ��ģ��
typedef enum 
{
  TIMER_CH0             ,  
  TIMER_CH1             ,  
  TIMER_CH2             ,
  TIMER_CH3             , 
  TIMER_CH4             , 
  TIMER_CH5             ,
  TIMER_CH6             , 
  TIMER_CH_NUM          ,
}TIMER_CHn;           //��ʱ��ͨ��
/*********************************PWM����**************************************/
typedef enum
{
  TIMER_PWM_OUTMOD0     ,
  TIMER_PWM_OUTMOD1     ,
  TIMER_PWM_OUTMOD2     ,
  TIMER_PWM_OUTMOD3     ,
  TIMER_PWM_OUTMOD4     ,
  TIMER_PWM_OUTMOD5     ,
  TIMER_PWM_OUTMOD6     ,
  TIMER_PWM_OUTMOD7     ,
}TIMER_PWM_OUTMODn;       //PWM���������
//���PWM������
#define DEFAULT_PWM_OUTMOD      TIMER_PWM_OUTMOD7  //�궨��PWM�����Ĭ�ϲ���
//����ռ�ձ����ֵ
#define TIMER_PWM_A0_PRECISON       1000u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/TIMER_A0_PRECISON
#define TIMER_PWM_A1_PRECISON       1000u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/TIMER_A1_PRECISON
#define TIMER_PWM_A2_PRECISON       1000u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/TIMER_A2_PRECISON
#define TIMER_PWM_B0_PRECISON       20000u     //����ռ�ձȾ��ȣ�100������Ϊ1%��1000u�򾫶�Ϊ0.1%������ռ�ձ� duty �βδ��룬��ռ�ձ�Ϊ duty/TIMER_B0_PRECISON   
extern STATUS   TIMER_PWM_MultiChannel_Init(TIMERn, uint32_t fre, uint8_t ChannelNum,...);//��ʱ��PWM���ܳ�ʼ����ʹ�ÿɱ���������Ը�����Ҫ��ʼ�����ͨ��
//ʹ������ĺ궨����Բ��������ʼ��ͨ���������磺TIMER_PWM_Init(TIMER_A0,1000,TIMER_CH1,TIMER_CH3); //��ʼ��TIMER_CH1,TIMER_CH3����ͨ��
#define         TIMER_PWM_Init(timer,fre,...)       TIMER_PWM_MultiChannel_Init(timer,fre,ARG_NUM(__VA_ARGS__),##__VA_ARGS__)
extern void     TIMER_PWM_SetChannelOutmod(TIMERn, TIMER_CHn, TIMER_PWM_OUTMODn);//����PWM�������
extern STATUS   TIMER_PWM_SetFrequency    (TIMERn, uint32_t fre);               //����ĳһ����ʱ��PWM��Ƶ��,ע�⣺ռ�ձȻ�ı�
extern void     TIMER_PWM_SetChannelDuty  (TIMERn, TIMER_CHn, uint32_t duty);   //����ĳһ����ʱ��ĳһ��ͨ����ռ�ձ�
/*******************************��ʱ�жϹ���************************************/
extern STATUS   TIMER_Interval_Us   (TIMERn, uint32_t us);                      //��ʼ��һ��us���Ķ�ʱ�ж�
#define         TIMER_Interval_Ms(timer,ms)  TIMER_Interval_Us(timer,(uint32_t)ms*1000)
/*******************************��ʱ�жϹ���************************************/
extern STATUS   TIMER_Delay_Us(TIMERn, uint32_t us);                            //us����ʱ
#define         TIMER_Delay_Ms(timer,ms)     TIMER_Delay_Us(timer,(uint32_t)ms*1000)
/*******************************�����жϹ���************************************/
typedef enum
{
  CAP_None              ,  //��ֹ����
  CAP_Rising            ,  //�����ز���
  CAP_Falling           ,  //�½��ز���
  CAP_RisingAndFalling  ,  //�������½��ض�����
}TIMER_CAPTUREn;      //����ģʽ
extern STATUS   TIMER_Capture_Init      (TIMERn, TIMER_CHn, TIMER_CAPTUREn);    //��ʼ��һ��ͨ��Ϊ����ģʽ�� 
extern void     TIMER_Capture_SetMode   (TIMERn, TIMER_CHn, TIMER_CAPTUREn);    //���ò������
extern uint16_t TIMER_Capture_GetValue  (TIMERn, TIMER_CHn);                    //��ȡ�����ò�������ʱ�ļ���ֵ
extern uint32_t TIMER_Capture_CalTime_Us(uint16_t start_value,uint16_t stop_value,uint16_t overflowTime);  //�������β����ʱ��ע�⻹Ҫ���Ƿ������β���֮��Ķ�ʱ���������
extern void     TIMER_Capture_Clear     (TIMERn, TIMER_CHn);                    //ĳһͨ����ȡ��������
/*******************************�����������************************************/
extern STATUS   TIMER_Pluse_Init    (TIMERn);                                   //��ʼ��һ����ʱ�������������
extern uint32_t TIMER_Pluse_GetValue(TIMERn, uint16_t overflowTime);            //��ȡĳһ����ʱ���������ֵ
extern void     TIMER_Pluse_Clear   (TIMERn);                                   //����һ����ʱ�����������ֵ
/*******************************��ʱ����************************************/
extern STATUS   TIMER_Timer_Init    (TIMERn);                                   //��ʱ���ܳ�ʼ��
extern void     TIMER_Timer_Start   (TIMERn);                                   //��ʱ��������
extern uint16_t TIMER_Timer_Stop    (TIMERn);                                   //��ʱ����ֹͣ�����ؼ���ֵ
extern uint32_t TIMER_Timer_GetTime_Us(TIMERn, uint16_t overflowTime);          //��ȡ��ʱʱ��(��λ��US)
/*******************************�ж�����************************************/
typedef enum
{
  TIMER_CCR0_IRQn          ,    /* CCR0_Interrupt *///ע������жϱ�־�����жϺ���Զ�����������жϺ�Ҫ���ж������־�Ƿ���λ
  TIMER_CCR1_IRQn          ,    /* CCR1_Interrupt */
  TIMER_CCR2_IRQn          ,    /* CCR2_Interrupt */
  TIMER_CCR3_IRQn          ,    /* CCR3_Interrupt */
  TIMER_CCR4_IRQn          ,    /* CCR4_Interrupt */
  TIMER_CCR5_IRQn          ,    /* CCR5_Interrupt */
  TIMER_CCR6_IRQn          ,    /* CCR6_Interrupt */
  TIMER_OverFlow_IRQn           /* IFG */ //��������������񣬼�ʱ���������ʱʹ�ܣ���������²�ʹ��
}TIMER_IRQn;
extern void     TIMER_ITConfig   (TIMERn, TIMER_IRQn, STATUS ITState);          //�����Ƿ�ʹ��TIMER��ĳһ���ж�
extern STATUS   TIMER_GetITStatus(TIMERn, TIMER_IRQn);                          //��ȡ��ʱ����ĳһλ�жϱ�־
extern void     TIMER_ClearITPendingBit(TIMERn, TIMER_IRQn);                    //���TIMER��ĳһ���жϱ�־
/*******************************����***********************************/
//��ʱ�����У�ֹͣ
extern void     TIMER_Run        (TIMERn);                                      //������ʱ������
extern void     TIMER_Stop       (TIMERn);                                      //ֹͣ��ʱ��
//��ȡ�����ö�ʱ����ǰ����ֵ
extern uint16_t TIMER_GetCounterValue(TIMERn);                                  //��ȡ��ʱ��������ֵ
extern void     TIMER_SetCounterValue(TIMERn, uint16_t value);                  //���ö�ʱ��������ֵ
//��ȡ�����ö�ʱ��ĳһͨ����ǰ����ֵ
extern uint16_t TIMER_GetChannelCounterValue(TIMERn,TIMER_CHn);                 //��ȡ��ʱ��ͨ��������ֵ
extern void     TIMER_SetChannelCounterValue(TIMERn,TIMER_CHn,uint16_t value);  //���ö�ʱ��ͨ��������ֵ
//��ȡͨ�����ŵ�ƽ״̬
extern STATUS   TIMER_GetChannelPinOutValue(TIMERn, TIMER_CHn);                    //��ȡͨ�����ֵ
extern STATUS   TIMER_GetChannelPinInValue (TIMERn, TIMER_CHn);                    //��ȡͨ������ֵ,�����ڲ���ʱ�ж��������ػ����½���
#endif