#include "key.h"
#include "msp430f5529_gpio.h"
//按键处理阶段
typedef enum
{
  KEY_PRESSED           ,//按键按下
  KEY_REALSE            ,//按键松开
  KEY_REALSE_WOBBLE     ,//确认松开的消抖状态
  KEY_PRESS_WOBBLE      ,//确认按下的消抖状态
}KEY_STAGEn;
//端口配置参数
const struct
{
  PORTn         Port;             //GPIO端口
  uint16_t      Pin;              //引脚
  uint16_t      config;           //
  BOOL          PressedLevel;     //按下端口电平状态
} KEY_InitStruct[KEY_NUM] =
{
  {KEY1_GPIO_PORT,KEY1_GPIO_PIN,KEY1_GPIO_CONFIG,KEY1_PRESSED_LEVEL}, //按键1引脚配置参数
  {KEY2_GPIO_PORT,KEY2_GPIO_PIN,KEY2_GPIO_CONFIG,KEY2_PRESSED_LEVEL}, //按键2引脚配置参数
  //如需添加更多按键，请先对按键进行宏定义后，在参考上面修改
  //{KEY3_GPIO_PORT,KEY3_GPIO_PIN,KEY2_GPIO_CONFIG,KEY3_PRESSED_LEVEL}, //按键3引脚配置参数
};
//按键处理信息变量
volatile struct
{
  void (*ClickedCallBack)(KEY_EVENT);  //按键事件回调函数
  KEY_STAGEn    Stage;         //按键处理阶段
  BOOL          isClicked;     //按键状态是否发生改变                     
  BOOL          isLongClick;   //是否是长按
  uint16_t      ClickTimes;    //点击次数变量
  uint16_t      DealTime;      //按键处理时间
  uint16_t      PressedTime;   //按键按下时间
  uint8_t     	WobbleTime;    //消抖时间变量	
}KeyStruct[KEY_NUM];
/*******************************************************************************
*  函数名称：KEY_GPIO_Config(KEYn key)
*  功能说明：KEY端口初始化程序
*  参数说明：KEYn key: 按键
*  函数返回：无
*  使用示例：
********************************************************************************/
static void KEY_GPIO_Config(KEYn key)
{
  GPIO_Init(KEY_InitStruct[key].Port, KEY_InitStruct[key].Pin, KEY_InitStruct[key].config);
}
/*******************************************************************************
*  函数名称：KEY_Init(KEYn key,void (*ClickedCallBack)(KEY_EVENT))
*  功能说明：KEY初始化程序
*  参数说明：
             KEYn key: 要初始化的按键
             void (*ClickedCallBack)(KEY_EVENT): 按键事件调用的回掉函数
*  函数返回：无
*  使用示例：
********************************************************************************/
void KEY_Init(KEYn key,void (*ClickedCallBack)(KEY_EVENT))
{
  KEY_GPIO_Config(key);

  KeyStruct[key].ClickedCallBack = ClickedCallBack; //组册按键处理事件回调函数
  KeyStruct[key].ClickTimes = 0;        //点击次数清零
  KeyStruct[key].Stage = KEY_REALSE;    //按键处于松开状态
  KeyStruct[key].isClicked = FALSE;     //按键状态未改变
  KeyStruct[key].isLongClick = FALSE;   //不是长按
  KeyStruct[key].WobbleTime = 0;        //消抖时间初始化为0
}
/*******************************************************************************
*  函数名称：KEY_isPressed(KEYn key)
*  功能说明：判断按键是否按下
*  参数说明：
             KEYn key: 按键
*  函数返回：BOOL ：按键当前的状态
*  使用示例：
********************************************************************************/
BOOL KEY_isPressed(KEYn key)
{
  if(KEY_InitStruct[key].PressedLevel == GPIO_ReadBit(KEY_InitStruct[key].Port, KEY_InitStruct[key].Pin))
    return TRUE;
  else 
    return FALSE;
}
/*******************************************************************************
*  函数名称：KEY_20msTimingProcess(KEYn key)
*  功能说明：按键20ms周期处理函数
*  参数说明：
             KEYn key: 按键
*  函数返回：无
*  使用示例：
********************************************************************************/
void KEY_20msTimingProcess(KEYn key)
{
  switch(KeyStruct[key].Stage)
  {
  case KEY_REALSE:
    if(KEY_isPressed(key)) //如果按键按下
    {
      KeyStruct[key].Stage = KEY_PRESS_WOBBLE;    //进入按下消抖阶段
      KeyStruct[key].WobbleTime = 0;              //消抖时间清零
      KeyStruct[key].PressedTime = 0;             //按键按下时间清零
    }
    break;
  case KEY_PRESS_WOBBLE:
    KeyStruct[key].PressedTime += KEY_TIMING_PROCESS_TIME;								//按键按下时间开始计时
    KeyStruct[key].WobbleTime += KEY_TIMING_PROCESS_TIME; 								//消抖时间开始计时
    if(KEY_isPressed(key)) //如果按键按下
    {
      if(KeyStruct[key].WobbleTime >= KEY_PRESS_WOBBLE_TIME)  //消抖时间达到
      {
        KeyStruct[key].Stage = KEY_PRESSED;         //可以确认按下，进入已按下阶段
        KeyStruct[key].ClickTimes ++;             //按下次数加1
      }
    }
    else
    {
      KeyStruct[key].Stage = KEY_REALSE;          //如果消抖时间内松开则认为该次按键按下无效
    }
    break;
  case KEY_PRESSED:
    KeyStruct[key].PressedTime += KEY_TIMING_PROCESS_TIME;                //继续记录按键按下时间
    if( (KeyStruct[key].isLongClick == FALSE)
        &&(KeyStruct[key].PressedTime >= KEY_LONG_CLICK_PERIOD)) //如果长时间按下，则认为是长按状态
    {
      KeyStruct[key].ClickedCallBack(KEY_LONG_CLICK); //按键按下时间过长，则认为是长击
      KeyStruct[key].isLongClick = TRUE;          //标记为长按
    }
    else
    {
      if(KEY_isPressed(key) == FALSE) //如果按键松开
      {
        KeyStruct[key].Stage = KEY_REALSE_WOBBLE; //进入按键松开后消抖阶段
        KeyStruct[key].WobbleTime = 0;            //消抖时间清零，一边后续阶段消抖
      }
    }
    break;
  case KEY_REALSE_WOBBLE:
    KeyStruct[key].WobbleTime += KEY_TIMING_PROCESS_TIME;                 //按键松开消抖时间开始计时
    if(KEY_isPressed(key) == FALSE) //如果按键松开
    {
      if(KeyStruct[key].WobbleTime >= KEY_REALSE_WOBBLE_TIME)  //消抖时间达到
      {
        KeyStruct[key].Stage = KEY_REALSE;              //进入按键完全松开阶段
        KeyStruct[key].isClicked = TRUE;             //标记按键状态发生改变
      }
    }
    break;	
    default:break;
  }
  if(KeyStruct[key].isClicked == TRUE)  //如果按键状态改变了
  {
    //达到双击时间最大值或已经双击
    KeyStruct[key].DealTime += KEY_TIMING_PROCESS_TIME;
    if((KeyStruct[key].DealTime >= KEY_DOUBLE_CLICK_PERIOD)
      ||KeyStruct[key].ClickTimes >= 2)
    {
      if(KeyStruct[key].ClickedCallBack != NULL)
      {
        //如果按下时间很长
        if((KeyStruct[key].isLongClick == FALSE))
        {
          if(KeyStruct[key].ClickTimes >= 2)  //按下多次则认为是双击
            KeyStruct[key].ClickedCallBack(KEY_DOUBLE_CLICK);
          else if(KeyStruct[key].ClickTimes == 1)                               //否则认为是单击
            KeyStruct[key].ClickedCallBack(KEY_CLICK);
        }
      }
      KeyStruct[key].PressedTime = 0;
      KeyStruct[key].ClickTimes = 0;
      KeyStruct[key].DealTime = 0;
      KeyStruct[key].isClicked = FALSE;
      KeyStruct[key].isLongClick = FALSE;
    }
  }
}
