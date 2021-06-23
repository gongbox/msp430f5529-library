#include"PID.h"
#include"math.h"

//初始化PID结构体参数
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
//设置目标值
void  PID_SetPoint     (PID * s_PID, PID_VAR_TYPE set_point) 
{
   s_PID->SetPoint = set_point;
}      
//设置PID输出范围
void  PID_SetOutRange  (PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin)
{
  s_PID->OutMax = outMax;
  s_PID->OutMin = outMin;
}
//设置PID积分范围
void  PID_SetIntegralOutRange(PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin)
{
  s_PID->IntegralMax = outMax;
  s_PID->IntegralMin = outMin;
}
//增量式PID计算
PID_VAR_TYPE Increment_PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{
    s_PID->LastResult = s_PID->Result;					       // 简单赋值运算	
    //误差计算   
    s_PID->Error = s_PID->SetPoint - now_point;
    //PID计算   
    s_PID->Result = s_PID->LastResult 
                  + s_PID->Proportion  * (s_PID->Error - s_PID->LastError)                          // 比例项
                  + s_PID->Integral    *  s_PID->Error	                                       // 积分项 
                  + s_PID->Derivative  * (s_PID->Error - 2*(s_PID->LastError) + s_PID->PrevError);  // 微分项
   
    s_PID->PrevError = s_PID->LastError;		               // 简单赋值运算
    s_PID->LastError = s_PID->Error; 				       // 简单赋值运算
    //输出限幅
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;	
}
//位置式PID计算
PID_VAR_TYPE Position_PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{
    s_PID->LastResult = s_PID->Result;			       // 简单赋值运算
    //误差计算
    s_PID->Error = s_PID->SetPoint - now_point;
    s_PID->SumError += s_PID->Error;                            //积分误差累加
    //积分限幅
    PID_VAR_TYPE IOutValue = s_PID->SumError * s_PID->Integral;
    if(IOutValue > s_PID->IntegralMax)IOutValue = s_PID->IntegralMax;
    else if(IOutValue < s_PID->IntegralMin)IOutValue = s_PID->IntegralMin;
    //PID计算
    s_PID->Result =  s_PID->Proportion  *  s_PID->Error                          // 比例项
                   + IOutValue                                                     // 积分项 
                   + s_PID->Derivative  * (s_PID->Error - s_PID->LastError);     // 微分项
    
    s_PID->PrevError = s_PID->LastError;		                       // 简单赋值运算
    s_PID->LastError = s_PID->Error; 				       // 简单赋值运算
 
    //输出限幅
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;	
}
//比例外置式PID
PID_VAR_TYPE PID_Cal(PID * s_PID,PID_VAR_TYPE now_point)
{  
    s_PID->LastResult = s_PID->Result;			       // 简单赋值运算
    //误差计算
    s_PID->Error = s_PID->SetPoint - now_point;
    s_PID->SumError += s_PID->Error;                            //积分误差累加
    //积分限幅
    PID_VAR_TYPE IOutValue = s_PID->SumError * s_PID->Integral;
    if(IOutValue > s_PID->IntegralMax)IOutValue = s_PID->IntegralMax;
    else if(IOutValue < s_PID->IntegralMin)IOutValue = s_PID->IntegralMin;
    //PID计算   
    s_PID->Result = s_PID->Proportion *
       (s_PID->Error + IOutValue + s_PID->Derivative * (s_PID->Error - s_PID->LastError) );
   
    s_PID->PrevError = s_PID->LastError;		                       // 简单赋值运算
    s_PID->LastError = s_PID->Error; 				       // 简单赋值运算
    //输出限幅
    if(s_PID->Result > s_PID->OutMax)s_PID->Result = s_PID->OutMax;
    else if(s_PID->Result < s_PID->OutMin)s_PID->Result = s_PID->OutMin;
    
    return s_PID->Result;
}