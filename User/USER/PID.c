#include"PID.h"
#include"math.h"

//��ʼ��PID�ṹ�����
void PID_Init(PID * s_PID,PID_VAR_TYPE set_point,PID_VAR_TYPE Proportion,PID_VAR_TYPE Integral, PID_VAR_TYPE Derivative)
{
   s_PID->SetPoint = set_point;
   s_PID->Proportion = Proportion;
   s_PID->Integral = Integral;
   s_PID->Derivative = Derivative;
   s_PID->Error = 0;
   s_PID->LastError = 0;
   s_PID->PrevError = 0;
   s_PID->SumError = 0;
   s_PID->LastResult = 0;
   s_PID->Result = 0;
   s_PID->OutMax = DEFAULT_PID_OUT_MAX;
   s_PID->OutMin = DEFAULT_PID_OUT_MIN;
   s_PID->IntegralMax = DEFAULT_PID_INTEGRAL_OUT_MAX;
   s_PID->IntegralMin = DEFAULT_PID_INTEGRAL_OUT_MIN;
}
//����Ŀ��ֵ
void  PID_SetPoint     (PID * s_PID, PID_VAR_TYPE set_point) 
{
   s_PID->SetPoint = set_point;
}      
//����PID�����Χ
void  PID_SetOutRange  (PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin)
{
  s_PID->OutMax = outMax;
  s_PID->OutMin = outMin;
}
//����PID���ַ�Χ
void  PID_SetIntegralOutRange(PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin)
{
  s_PID->IntegralMax = outMax;
  s_PID->IntegralMin = outMin;
}
//����ʽPID����
PID_VAR_TYPE Increment_PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{
    s_PID->LastResult = s_PID->Result;					       // �򵥸�ֵ����	
    //������   
    s_PID->Error = s_PID->SetPoint - now_point;
    //PID����   
    s_PID->Result = s_PID->LastResult 
                  + s_PID->Proportion  * (s_PID->Error - s_PID->LastError)                          // ������
                  + s_PID->Integral    *  s_PID->Error	                                       // ������ 
                  + s_PID->Derivative  * (s_PID->Error - 2*(s_PID->LastError) + s_PID->PrevError);  // ΢����
   
    s_PID->PrevError = s_PID->LastError;		               // �򵥸�ֵ����
    s_PID->LastError = s_PID->Error; 				       // �򵥸�ֵ����
    //����޷�
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;	
}
//λ��ʽPID����
PID_VAR_TYPE Position_PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{
    s_PID->LastResult = s_PID->Result;			       // �򵥸�ֵ����
    //������
    s_PID->Error = s_PID->SetPoint - now_point;
    s_PID->SumError += s_PID->Error;                            //��������ۼ�
    //�����޷�
    PID_VAR_TYPE IOutValue = s_PID->SumError * s_PID->Integral;
    if(IOutValue > s_PID->IntegralMax)IOutValue = s_PID->IntegralMax;
    else if(IOutValue < s_PID->IntegralMin)IOutValue = s_PID->IntegralMin;
    //PID����
    s_PID->Result =  s_PID->Proportion  *  s_PID->Error                          // ������
                   + IOutValue                                                     // ������ 
                   + s_PID->Derivative  * (s_PID->Error - s_PID->LastError);     // ΢����
    
    s_PID->PrevError = s_PID->LastError;		                       // �򵥸�ֵ����
    s_PID->LastError = s_PID->Error; 				       // �򵥸�ֵ����
 
    //����޷�
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;	
}
//��������ʽPID
PID_VAR_TYPE PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{  
    s_PID->LastResult = s_PID->Result;			       // �򵥸�ֵ����
    //������
    s_PID->Error = s_PID->SetPoint - now_point;
    s_PID->SumError += s_PID->Error;                            //��������ۼ�
    //�����޷�
    PID_VAR_TYPE IOutValue = s_PID->SumError * s_PID->Integral;
    if(IOutValue > s_PID->IntegralMax)IOutValue = s_PID->IntegralMax;
    else if(IOutValue < s_PID->IntegralMin)IOutValue = s_PID->IntegralMin;
    //PID����   
    s_PID->Result = s_PID->Proportion *
       (s_PID->Error + IOutValue + s_PID->Derivative * (s_PID->Error - s_PID->LastError) );
   
    s_PID->PrevError = s_PID->LastError;		                       // �򵥸�ֵ����
    s_PID->LastError = s_PID->Error; 				       // �򵥸�ֵ����
    //����޷�
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;
}