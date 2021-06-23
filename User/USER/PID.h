#ifndef __PID_H
#define __PID_H	  

//PID输出范围宏定义
#define DEFAULT_PID_OUT_MAX               ( 10000)
#define DEFAULT_PID_OUT_MIN               (-10000)
//PID积分范围宏定义
#define DEFAULT_PID_INTEGRAL_OUT_MAX      ( 10000)
#define DEFAULT_PID_INTEGRAL_OUT_MIN      (-10000)

//PID变量类型宏定义
#define PID_VAR_TYPE                            float     //int

//定义结构体
typedef struct  
{  
    PID_VAR_TYPE SetPoint;      // 设定目标 
    PID_VAR_TYPE Proportion;    // 比例常数
    PID_VAR_TYPE Integral;      // 积分常数
    PID_VAR_TYPE Derivative;    // 微分常数 
    PID_VAR_TYPE SumError;      // 积分和
    PID_VAR_TYPE Error;         // 误差
    PID_VAR_TYPE LastError;     // 上次误差 
    PID_VAR_TYPE PrevError;     // 前一次误差   
    PID_VAR_TYPE LastResult;    // 上次计算结果
    PID_VAR_TYPE Result;        // 当前计算结果
    PID_VAR_TYPE OutMax;        // 输出限幅最大值
    PID_VAR_TYPE OutMin;        // 输出限幅最小值
    PID_VAR_TYPE IntegralMax;   // 积分限幅最大值
    PID_VAR_TYPE IntegralMin;   // 积分限幅最小值
}PID;

extern void PID_Init(PID * s_PID, PID_VAR_TYPE set_point,PID_VAR_TYPE Proportion, PID_VAR_TYPE Integral, PID_VAR_TYPE Derivative);  //PID初始化

extern void  PID_SetOutRange  (PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin);         //设置PID输出范围
extern void  PID_SetIntegralOutRange(PID * s_PID, PID_VAR_TYPE outMax,PID_VAR_TYPE outMin);   //设置PID积分范围
extern void  PID_SetPoint     (PID * s_PID, PID_VAR_TYPE set_point);            //设置目标值

extern PID_VAR_TYPE Increment_PID_Cal(PID * s_PID, PID_VAR_TYPE now_point);     //增量式PID计算
extern PID_VAR_TYPE Position_PID_Cal (PID * s_PID, PID_VAR_TYPE now_point);     //位置式PID计算
extern PID_VAR_TYPE PID_Cal          (PID * s_PID, PID_VAR_TYPE now_point);     //比例外置式PID
 
#endif 
