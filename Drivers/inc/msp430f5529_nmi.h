#ifndef NMI_H_
#define NMI_H_

#include "common.h"

//NMI 
typedef enum
{
  NMI_Rising             ,                 //������
  NMI_Falling            ,                 //�½���
}NMIIES;          //NMI�жϱ���ѡ��

extern void    NMI_Init     (NMIIES ies);                                       // ��ʼ��NMI���ܣ���ѡ���жϱ���
extern void    NMI_DeInit   (void);                                             // ����NMI����
extern void    NMI_ITConfig (STATUS ITState);                                   // �����Ƿ�ʹ��NMI�ж�
extern STATUS  NMI_GetITStatus(void);                                           // ��ȡNMI�жϱ�־
extern void    NMI_ClearITPendingBit(void);                                     // ���NMI�жϱ�־

#endif