#include "msp430f5529_dma.h"

const DMA_MemMapPtr      DMAX = DMA_BASE_PTR;
/*******************************************************************************
*  �������ƣ�DMA_Init(DMA_CHn dma_ch, void *SADDR, void *DADDR, DMA_Trigger Trigger, uint16_t count, uint16_t config)
*  ����˵������ʼ��һ��DMAͨ��
*  ����˵����DMA_CHn dma_ch             :DMAͨ��
             void *SADDR                :DMA����Դ��ַ
             void *DADDR                :DMA����Ŀ�ĵ�ַ
             DMA_Trigger Trigger        :DMA���䴥������
             uint16_t count               :DMA�������
             uint16_t config              :DMA��������
*  �������أ���
*  ʹ��ʾ���� DMA_Init(DMA_CH2, &Sdata, &DData,DMATSEL_DMA_REQ,255,  DMA_SRCAddr_KeepOn|DMA_DSTAddr_KeepOn|DMA_WORD|DMA_Single|DMA_Edge);
                       //ͨ��2��Դ��ַ��Ŀ�ĵ�ַ������Դ������255���֣�Դ��ַ���䣬Ŀ�ĵ�ַ���䣬�ִ��䣬���δ��������ش���
********************************************************************************/
void DMA_Init(DMA_CHn dma_ch, void *SADDR, void *DADDR, DMA_Trigger Trigger, uint16_t count, uint16_t config)
{ 
  DMAX->CHX[dma_ch].EN = RESET;                          //�ر�DMAģ��
  DMAX->CHX[dma_ch].DA = (uint32_t)DADDR;                  //����Ŀ�ĵ�ַ
  DMAX->CHX[dma_ch].SA = (uint32_t)SADDR;                  //����Դ��ַ
  DMAX->CHX[dma_ch].SZ = count;                          //����д�����ݴ�С
  
  //DMAX->TSSEL[dma_ch] = Trigger;                         //����д�������⣬��֪��Ϊʲô��
  if((dma_ch%2) == 0)
  {
   *(&DMACTL0 + (dma_ch/2)) &= 0xFF00;      //���������Դ
   *(&DMACTL0 + (dma_ch/2)) |= Trigger;     //�����ô���Դ
  }
  else
  {
    *(&DMACTL0 + (dma_ch/2)) &= 0x00FF;
    *(&DMACTL0 + (dma_ch/2)) |= Trigger<<8;
  }
  DMAX->CHX[dma_ch].CTL = config;                       //����DMAģ��
  DMAX->CHX[dma_ch].EN = BIT_SET;                           //ʹ��DMAģ��
}
/*******************************************************************************
*  �������ƣ�DMA_Cmd  (DMA_CHn dma_ch,BOOL State)
*  ����˵����DMAʹ�ܻ��ֹ
*  ����˵����DMA_CHn dma_ch     :DMA����ͨ��
             STATUS State       :DMA�Ƿ�ʹ��
*  �������أ���
*  ʹ��ʾ����DMA_Cmd  (DMA_CH1,ENABLE);  //ʹ��DMAͨ��1
********************************************************************************/
void DMA_Cmd  (DMA_CHn dma_ch,STATUS State)
{
  DMAX->CHX[dma_ch].EN = State; 
}
/*******************************************************************************
*  �������ƣ�DMA_SoftwareTrigger(DMA_CHn dma_ch)
*  ����˵�����������һ��DMA����
*  ����˵����DMA_CHn dma_ch     :DMA����ͨ��
*  �������أ���
*  ʹ��ʾ����DMA_SoftwareTrigger(DMA_CH1);  //�������һ��DMAͨ��1����
********************************************************************************/
void DMA_SoftwareTrigger(DMA_CHn dma_ch)
{    
  DMAX->CHX[dma_ch].REQ = RESET; 
  DMAX->CHX[dma_ch].REQ = BIT_SET; 
}
/*******************************************************************************
*  �������ƣ�DMA_ITConfig (DMA_CHn dma_ch,STATUS ITState)
*  ����˵���������Ƿ�ʹ��DMAĳһͨ������ж�
*  ����˵����DMA_CHn dma_ch     :DMA����ͨ��
             BOOL ITState       :�Ƿ�ʹ��DMA��������ж�
*  �������أ���
*  ʹ��ʾ����DMA_ITConfig (DMA_CH0,ENABLE);  //ʹ��DMAͨ��1��������ж�
********************************************************************************/
void DMA_ITConfig (DMA_CHn dma_ch,STATUS ITState)
{
  DMAX->CHX[dma_ch].IFG = RESET;
  DMAX->CHX[dma_ch].IE = ITState; 
}
/*******************************************************************************
*  �������ƣ�DMA_GetITStatus(DMA_CHn dma_ch)
*  ����˵�����ж�ĳһ��ͨ���жϱ�־λ״̬
*  ����˵����DMA_CHn dma_ch     :DMA����ͨ��
*  �������أ�STATUS  :TRUE �ж��¼����� FALSE �ж��¼�δ����
*  ʹ��ʾ����if(TRUE == DMA_GetITStatus(DMA_CH0)){...}  //�ж�DMAͨ��1�Ƿ�����������ж�
********************************************************************************/
STATUS DMA_GetITStatus(DMA_CHn dma_ch)
{
  return ((DMAX->CHX[dma_ch].IFG) ? TRUE : FALSE);
}
/*******************************************************************************
*  �������ƣ�DMA_ClearITPendingBit(DMA_CHn dma_ch)
*  ����˵����DMA���ĳһͨ���жϱ�־
*  ����˵����DMA_CHn dma_ch     :DMA����ͨ��
*  �������أ���
*  ʹ��ʾ����DMA_ClearITPendingBit(DMA_CH0);  //���DMAͨ��1��������жϱ�־λ
********************************************************************************/
void DMA_ClearITPendingBit(DMA_CHn dma_ch)
{
  DMAX->CHX[dma_ch].IFG = RESET;
}
