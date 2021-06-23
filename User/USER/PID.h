#ifndef __PID_H
#define __PID_H	  

//PID�����Χ�궨��
#define DEFAULT_PID_OUT_MAX               ( 10000)
#define DEFAULT_PID_OUT_MIN               (-10000)
//PID���ַ�Χ�궨��
#define DEFAULT_PID_INTEGRAL_OUT_MAX      ( 10000)
#define DEFAULT_PID_INTEGRAL_OUT_MIN      (-10000)

//PID�������ͺ궨��
#define PID_VAR_TYPE                            float     //int

//����ṹ��
typedef struct  
{  
    PID_VAR_TYPE SetPoint;      // �趨Ŀ�� 
    PID_VAR_TYPE Proportion;    // ��������
    PID_VAR_TYPE Integral;      // ���ֳ���
    PID_VAR_TYPE Derivative;    // ΢�ֳ��� 
    PID_VAR_TYPE SumError;      // ���ֺ�
    PID_VAR_TYPE Error;         // ���
    PID_VAR_TYPE LastError;     // �ϴ���� 
    PID_VAR_TYPE PrevError;     // ǰһ�����   
    PID_VAR_TYPE LastResult;    // �ϴμ�����
    PID_VAR_TYPE Result;        // ��ǰ������
    PID_VAR_TYPE OutMax;        // ����޷����ֵ
    PID_VAR_TYPE OutMin;        // ����޷���Сֵ
    PID_VAR_TYPE IntegralMax;   // �����޷����ֵ
    PID_VAR_TYPE IntegralMin;   // �����޷���Сֵ
}PID;

extern void PID_Init(PID * s_PID, PID_VAR_TYPE set_point,PID_VAR_TYPE Proportion, PID_VAR_TYPE Integral, PID_VAR_TYPE Derivative);  //PID��ʼ��

extern void  PID_SetOutRange  (PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin);         //����PID�����Χ
extern void  PID_SetIntegralOutRange(PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin);   //����PID���ַ�Χ
extern void  PID_SetPoint     (PID * s_PID, PID_VAR_TYPE set_point);            //����Ŀ��ֵ

extern PID_VAR_TYPE Increment_PID_Cal(PID * s_PID, PID_VAR_TYPE now_point);     //����ʽPID����
extern PID_VAR_TYPE Position_PID_Cal (PID * s_PID, PID_VAR_TYPE now_point);     //λ��ʽPID����
extern PID_VAR_TYPE PID_Cal          (PID * s_PID, PID_VAR_TYPE now_point);     //��������ʽPID
 
#endif 
