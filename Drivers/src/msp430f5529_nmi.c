#include "msp430f5529_nmi.h"

const SFR_MemMapPtr SFRX = SFR_BASE_PTR;
/*************************************************************************
*  �������ƣ�NMI_Init(NMIIES ies)
*  ����˵��������NMI
*  ����˵����NMIIES ies : �����NMI�жϴ�����
*  �������أ���
*  ʹ��ʾ����NMI_Init(NMI_Rising); //����NMIΪ�����ش����ж�
*************************************************************************/  
void  NMI_Init(NMIIES ies)
{
  SFRX->SYS.NMI = ENABLE;                     //ʹ��NMI����
  SFRX->IE.NMI = RESET;                       //��ֹNMI�ж�
  if(ies == NMI_Rising)
  {
    SFRX->SYS.NMIIES = BIT_SET;                   //����NMI�ж���
  }
  else
  {
    SFRX->SYS.NMIIES = RESET;
  }
  SFRX->IFG.NMI = RESET;                      //���NMI�жϱ�־
}
/*************************************************************************
*  �������ƣ�NMI_DeInit()
*  ����˵��������NMI����
*  ����˵������
*  �������أ���
*  ʹ��ʾ����NMI_DeInit(); //ȡ��NMI����
*************************************************************************/  
void    NMI_DeInit   (void)
{
  SFRX->SYS.NMI = DISABLE;                    //��ֹNMI����
}
/*************************************************************************
*  �������ƣ�NMI_ITConfig (STATUS ITState)
*  ����˵���������Ƿ�ʹ��NMI�ж�
*  ����˵����BOOL ITState :�Ƿ�ʹ��NMI�ж�
*  �������أ���
*  ʹ��ʾ����NMI_ITConfig (ENABLE);  //ʹ��NMI�ж�
*************************************************************************/  
void  NMI_ITConfig (STATUS ITState)
{
  if(ITState != DISABLE)
  {
    SFRX->IE.NMI = ENABLE;                    //����ʹ��NMI�ж�
  }
  else
  {
    SFRX->IE.NMI = DISABLE;
  }
}
/*************************************************************************
*  �������ƣ�NMI_GetITStatus(void)
*  ����˵������ȡNMI�жϱ�־
*  ����˵������
*  �������أ�STATUS : NMI�ж��¼��Ƿ���
*  ʹ��ʾ����if(TRUE == NMI_GetITStatus()){...} //�ж�NMI�ж��¼��Ƿ���
*************************************************************************/  
STATUS  NMI_GetITStatus(void)               
{
  return ((SFRX->IFG.NMI) ? TRUE : FALSE);       //�����ж��¼���־
}
/*************************************************************************
*  �������ƣ�NMI_ClearITPendingBit(void)
*  ����˵�������NMI�жϱ�־
*  ����˵������
*  �������أ���
*  ʹ��ʾ����NMI_ClearITPendingBit();  //���NMI�жϱ�־
*************************************************************************/  
void  NMI_ClearITPendingBit(void)
{
   SFRX->IFG.NMI = RESET;                       //����ж��¼���־
}