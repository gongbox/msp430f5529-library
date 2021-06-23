#ifndef __KEY_H__
#define __KEY_H__

#include "common.h"

/*****************************************用户定义声明部分************************************************/
//用户定义变量

#define KEY1_PRESSED_LEVEL        RESET          //KEY1按下的时候是高电平
#define KEY2_PRESSED_LEVEL        RESET          //KEY2按下的时候是高电平

//定义两个按键的引脚
#define KEY1_GPIO_PIN             1                 
#define KEY1_GPIO_PORT            P2                      
#define KEY1_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY2_GPIO_PIN             1                 
#define KEY2_GPIO_PORT            P1                      
#define KEY2_GPIO_CONFIG          (GPI|PULL_UP)

#define KEY_PRESS_WOBBLE_TIME	  10      //按键按下抖动时间。也就是消抖时间,单位ms
#define KEY_REALSE_WOBBLE_TIME    10      //按键释放抖动时间。也就是消抖时间,单位ms
#define KEY_LONG_CLICK_PERIOD     500     //按键长按时间最小值。单位ms
#define KEY_DOUBLE_CLICK_PERIOD   200     //按键双击时间最大值,单位ms 

#define KEY_TIMING_PROCESS_TIME   20      //按键处理周期,单位ms 

/************以下内容均不需要更改，直接使用即可*******************************/

typedef enum
{ 
  KEY1		,
  KEY2		,
  //KEY3	,
  KEY_NUM       ,
}KEYn;  

typedef enum
{
  KEY_NONE_CLICK    ,
  KEY_CLICK	        ,
  KEY_DOUBLE_CLICK  ,
  KEY_LONG_CLICK	,		
}KEY_EVENT;

void KEY_Init(KEYn,void (*ClickedCallBack)(KEY_EVENT)); 	                //按键初始化函数
BOOL KEY_isPressed(KEYn);						        //判断按键是否按下														//按键状态获取
void KEY_20msTimingProcess(KEYn);					        //案件周期调用函数											//按键周期性处理函数调用

#endif
