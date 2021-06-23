#ifndef DMA_H_
#define DMA_H_

#include"common.h"

typedef enum 
{
  DMA_CH0       ,
  DMA_CH1       ,
  DMA_CH2       ,
}DMA_CHn;    //DMAͨ��
typedef enum
{
  DMATSEL_DMA_REQ    =0 ,  // 0:  DMA_REQ (sw) 
  DMATSEL_TA0CCR0       ,  // 1:  Timer0_A (TA0CCR0.IFG) //���жϴ�������ر��ж�ʹ��λ�������IFG��־λ�Զ�����
  DMATSEL_TA0CCR2       ,  // 2:  Timer0_A (TA0CCR2.IFG) 
  DMATSEL_TA1CCR0       ,  // 3:  Timer1_A (TA1CCR0.IFG) 
  DMATSEL_TA1CCR2       ,  // 4:  Timer1_A (TA1CCR2.IFG) 
  DMATSEL_TA2CCR0       ,  // 5:  Timer2_A (TA2CCR0.IFG) 
  DMATSEL_TA2CCR2       ,  // 6:  Timer2_A (TA2CCR2.IFG) 
  DMATSEL_TB0CCR0       ,  // 7:  TimerB (TB0CCR0.IFG) 
  DMATSEL_TB0CCR2       ,  // 8:  TimerB (TB0CCR2.IFG) 
  DMATSEL_USCIA0RX   =16,  // 16: USCIA0 receive 
  DMATSEL_USCIA0TX      ,  // 17: USCIA0 transmit 
  DMATSEL_USCIB0RX      ,  // 18: USCIB0 receive 
  DMATSEL_USCIB0TX      ,  // 19: USCIB0 transmit 
  DMATSEL_USCIA1RX      ,  // 20: USCIA1 receive 
  DMATSEL_USCIA1TX      ,  // 21: USCIA1 transmit 
  DMATSEL_USCIB1RX      ,  // 22: USCIB1 receive 
  DMATSEL_USCIB1TX      ,  // 23: USCIB1 transmit 
  DMATSEL_ADC12IFG      ,  // 24: ADC12IFGx 
  DMATSEL_USB_FNRXD  =27,  // 27: USB FNRXD 
  DMATSEL_USB_READY     ,  // 28: USB ready 
  DMATSEL_MPY           ,  // 29: Multiplier ready 
  DMATSEL_DMAxIFG       ,  // 30: previous DMA channel DMAxIFG 
  DMATSEL_DMAE0         ,  // 31: ext. Trigger (DMAE0) 
}DMA_Trigger;         //DMA����Դ
typedef enum
{
  DMA_SRCAddr_KeepOn      =DMASRCINCR_0,     //Դ��ַ����(Ĭ��)
  DMA_SRCAddr_Decrease    =DMASRCINCR_2,     //Դ��ַ��С
  DMA_SRCAddr_Increase    =DMASRCINCR_3,     //Դ��ַ����
}DMA_SRC_ADDR_Config;      //DMA��ַ�仯ģʽ
typedef enum
{
  DMA_DSTAddr_KeepOn      =DMADSTINCR_0,     //Ŀ�ĵ�ַ����(Ĭ��)
  DMA_DSTAddr_Decrease    =DMADSTINCR_2,     //Ŀ�ĵ�ַ��С
  DMA_DSTAddr_Increase    =DMADSTINCR_3,     //Ŀ�ĵ�ַ����
}DMA_DST_ADDR_Config;      //DMA��ַ�仯ģʽ
typedef enum
{
  DMA_WORD                 = 0,              //���ִ���(Ĭ��)
  DMA_BYTE         = DMASRCBYTE|DMADSTBYTE,  //���ֽ�Ϊ��λ����
}DMA_DataSize;             //DMA���䵥λ
typedef enum
{
  DMA_Single               =DMADT_0,         //���δ���(Ĭ��)
  DMA_Block                =DMADT_1,         //�鴫��
  DMA_Burst_Block          =DMADT_2,         //ͻ���鴫��
  DMA_Repeated_Single      =DMADT_4,         //�ظ����δ���
  DMA_Repeated_Block       =DMADT_5,         //�ظ��鴫��
  DMA_Repeated_Burst_Block =DMADT_6,         //�ظ�ͻ���鴫��
}DMA_TransmitMode;          //DMA����ģʽ
typedef enum
{
  DMA_Edge                 =0,               //���ش���(Ĭ��)
  DMA_Level                =DMALEVEL         //�ߵ�ƽ����     
}DMA_TriggerMode;           //DMA������ʽ

extern void   DMA_Init(DMA_CHn, void *SADDR, void *DADDR, DMA_Trigger, uint16_t count, uint16_t config);  //DMA��ʼ������
extern void   DMA_Cmd (DMA_CHn, STATUS State);                                  //DMAʹ�ܻ��ֹ
extern void   DMA_SoftwareTrigger(DMA_CHn);                                    //DMA�������һ�δ���

extern void   DMA_ITConfig (DMA_CHn, STATUS ITState);                           //����ʹ�ܻ��ֹDMA�ж�
extern STATUS DMA_GetITStatus(DMA_CHn);                                         //�ж�ĳһ��ͨ���жϱ�־λ 
extern void   DMA_ClearITPendingBit(DMA_CHn);                                   //DMA���ͨ���жϱ�־

#endif