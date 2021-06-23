#include "msp430f5529_gpio.h"

//port，pin,BITs获取宏定义,由此可知，当使用16位IO定义时由于计算port,pin值会降低执行效率
#ifdef GPIO_PORT_SIZE_WORD
#define GET_PORT(portn)        ((((portn) >= PORTA)&&((portn) <= PORTD)) ? ((PORTn)((portn) - PORTA)) : ((PORTn)((portn - PORT1) / 2)))
#define GET_PIN(portn,pin)     ((((portn) >= PORTA)&&((portn) <= PORTD)) ? (pin)  : ((((portn - PORT1)%2) == 0) ? (pin)  : ((pin)+8u)))
#define GET_BITs(portn,BITs)   ((((portn) >= PORTA)&&((portn) <= PORTD)) ? (BITs) : ((((portn - PORT1)%2) == 0) ? (BITs) : (((BITs)<<8u))))
#else
#define GET_PORT(portn)        (portn)
#define GET_PIN(portn,pin)     (pin)
#define GET_BITs(portn,BITs)   (BITs)
#endif

#ifdef GPIO_PORT_SIZE_WORD
const GPIO_MemMapPtr GPIOX[] ={PA_BASE_PTR,PB_BASE_PTR,PC_BASE_PTR,PD_BASE_PTR}; 
#else
const GPIO_MemMapPtr GPIOX[] ={P1_BASE_PTR,P2_BASE_PTR,P3_BASE_PTR,P4_BASE_PTR,P5_BASE_PTR,P6_BASE_PTR,P7_BASE_PTR,P8_BASE_PTR}; 
#endif
/*********************************************************************************
函数名：IO口初始化
入口参数：PORTn portn    对应IO端口
          uint16_t pin   对应IO引脚位置
          uint16_t config     
          对应IO口配置 {
                            GPI                   ,// 输入方向
                            GPO                   ,// 输出方向
                            LDS                   ,// 低驱动能力
                            HDS                   ,// 高驱动能力
                            PULL_DOWN             ,// 下拉
                            PULL_UP               ,// 上拉
                            IRQ_RISING            ,// 上升沿触发中断
                            IRQ_FALLING           ,// 下降沿触发中断
                            SEL                   ,// 第二复用功能
                        }  
使用例程:GPIO_MultiBits_Init(P2,BIT3,GPO|HDS);   //初始化P2.3为输出放向，高驱动能力
**********************************************************************************/
void GPIO_MultiBits_Init(PORTn portn,uint16_t BITs,uint16_t config)
{
  //获取port,pin
  PORTn    port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  
  //配置IO口
  if((config & SEL) == SEL)
  {
    GPIOX[port]->SEL |= BITs;                    //选择为第二功能
  }
  else
  {
    GPIOX[port]->SEL &=~BITs;                    //选择为IO口功能
  }

  if((config & GPO) == GPO)              //判断方向
  {
    GPIOX[port]->PDIR |= BITs;       //设置为输出方向
    
    if((config&HDS) == HDS)            //判断驱动能力
    {
      GPIOX[port]->DS |= BITs;          //设为高驱动能力
    }
    else
    {
      GPIOX[port]->DS &=~BITs;          //设为低驱动能力
    }
  }
  else   //如果是输入方向
  {
    GPIOX[port]->PDIR &=~BITs;      //设置为输入方向
    
    if((config & PULL_DOWN) == PULL_DOWN) //判断是否开启下拉
    {
      GPIOX[port]->REN  |= BITs;             //开启上\下拉
      GPIOX[port]->POUT &=~BITs;             //设置为下拉
    }
    else if((config & PULL_UP) == PULL_UP)//判断是否开启上拉
    {
      GPIOX[port]->REN  |= BITs;             //开启上\下拉
      GPIOX[port]->POUT |= BITs;             //设置为上拉
    }
    else
    {
      GPIOX[port]->REN &=~BITs;              //关闭上\下拉
    }
    
    if((portn == PORT1)||(portn == PORT2)
#ifdef  GPIO_PORT_SIZE_WORD
       ||(portn == PORTA)
#endif       
       );                 //小于等于P2则可能会配置IO口中断
    {
      GPIOX[port]->IE &=~BITs;                  //关闭中断
      if((config & IRQ_RISING) == IRQ_RISING)       //如果有上升沿
      {
        GPIOX[port]->IES |= BITs;                  //设为上升沿
      }
      else 
      {
        GPIOX[port]->IES &=~BITs ;                 //设为下降沿
      } 
    }
  }
}
/*******************************************************************************
*  函数名称： GPIO_SetBits  (PORTn portn, uint16_t BITs)
*  功能说明：设置IO口输出为高电平
*  参数说明： PORTn portn     对应IO端口
              uint16_t BITs   对应IO引脚位置
*  函数返回：无
*  使用示例： GPIO_SetBits  (PORT1, GPIO_Pin_0|GPIO_Pin_1); //P1.0,P1.1输出高电平
********************************************************************************/
void     GPIO_SetBits  (PORTn portn, uint16_t BITs)
{
  if((portn <= PORT8)) 
    BITs = BITs & 0xFF;
  //获取port,pin
  PORTn port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  //IO口操作
  GPIOX[port]->POUT |= BITs;            //
}
/*******************************************************************************
*  函数名称： GPIO_ResetBits(PORTn portn, uint16_t BITs)
*  功能说明：设置IO口输出为低电平
*  参数说明： PORTn portn     对应IO端口
              uint16_t BITs   对应IO引脚位置
*  函数返回：无
*  使用示例：GPIO_ResetBits  (PORT1, GPIO_Pin_0|GPIO_Pin_1); //P1.0,P1.1输出低电平
********************************************************************************/
void     GPIO_ResetBits(PORTn portn, uint16_t BITs)
{
  //获取port,pin
  PORTn port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  //IO口操作
  GPIOX[port]->POUT &=~BITs;    
}
/*******************************************************************************
函数名称：IO口一位输出设置
入口参数：PORTn portn    对应IO端口
          uint8_t pin     对应IO引脚位置
          STATUS level    设置输出电平（SET ：高电平  RESET：低电平）
*  使用示例：
********************************************************************************/
void GPIO_WriteBit(PORTn portn,uint8_t pin,STATUS level)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO口操作
  if(level != RESET)
  {
    GPIOX[port]->POUT |= BIT;            //
  }
  else 
  {
    GPIOX[port]->POUT &=~BIT;            //
  }
}
/*******************************************************************************
函数名称：IO口一位输入读取
入口参数：PORTn portn    对应IO端口
          uint8_t pin    对应IO引脚位置
返回参数：STATUS         输入电平（SET ：高电平  RESET：低电平）
*  使用示例：
********************************************************************************/
STATUS GPIO_ReadBit(PORTn portn,uint8_t pin)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO口操作
  return ((GPIOX[port]->IN & BIT) ? BIT_SET : RESET);
}
/*******************************************************************************
函数名称：IO口一位输出翻转
入口参数：PORTn portn    对应IO端口
          uint8_t pin     对应IO引脚位置
*  使用示例：
********************************************************************************/
void GPIO_TurnBit  (PORTn portn,uint8_t pin)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO口操作
  GPIOX[port]->POUT ^= BIT;
}
/*******************************************************************************
函数名称：IO口8位输出设置
入口参数：PORTn portn    对应IO端口,必须取PORT1-PORT8
          uint8_t level   设置8位输出电平
*  使用示例：
********************************************************************************/
void GPIO_WriteByte(PORTn portn,uint8_t value)
{
#ifdef GPIO_PORT_SIZE_WORD
  
  ASSERT((portn <= PORT8),
         "GPIO_WriteByte",
         "端口设置不合理！"); //断言检测输入参数
  
  PORTn port = GET_PORT(portn);
  
  if((portn%2) == 0) //如果portn是基数时
  {
    GPIOX[port]->POUT = GPIOX[port]->POUT & 0xFF00;
    GPIOX[port]->POUT |= value;
  }
  else
  {
    GPIOX[port]->POUT = GPIOX[port]->POUT & 0x00FF;
    GPIOX[port]->POUT |= ((uint16_t)value<<8u);
  }
#else
  GPIOX[portn]->POUT = value;            //
#endif
}
/*******************************************************************************
函数名称：IO口8位输入读取
入口参数：PORTn portn    对应IO端口,必须取PORT1-PORT8
返回参数：uint8 8位输入电平
*  使用示例：
********************************************************************************/
uint8_t GPIO_ReadByte(PORTn portn)
{
#ifdef GPIO_PORT_SIZE_WORD
  //如果输入参数时P1-P8时
  ASSERT((portn <= PORT8),
         "GPIO_ReadByte",
         "端口设置不合理！"); //断言检测输入参数
  
  PORTn port = GET_PORT(portn);
  
  if((portn%2) == 0) //如果portn是基数时
  {
    return ((GPIOX[port]->IN) & 0x00FF);
  }
  else
  {
    return ((GPIOX[port]->IN) >> 8u);
  }
#else
  return (GPIOX[portn]->IN);            //
#endif
}
#ifdef GPIO_PORT_SIZE_WORD
/*******************************************************************************
函数名称：IO口16位输出设置
入口参数：PORTn portn     对应IO端口,必须取PORTA-PORTD
          uint16_t level   设置16位输出电平
*  使用示例：
********************************************************************************/
void    GPIO_WriteWord(PORTn portn, uint16_t value) 
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  
  GPIOX[port]->POUT = value;            //
}
/*******************************************************************************
函数名称：IO口16位输入读取
入口参数：PORTn portn    对应IO端口,必须取PORTA-PORTD
返回参数：uint16 16位输入电平
*  使用示例：
********************************************************************************/
uint16_t GPIO_ReadWord(PORTn portn)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  
  return (GPIOX[port]->IN);
}
#endif
/*******************************************************************************
函数名称：设置是否允许IO口中断
入口参数：PORTn portn    对应IO端口
          uint8_t pin     对应IO引脚位置
*  使用示例：
********************************************************************************/
void GPIO_ITConfig(PORTn portn,uint8_t pin,STATUS ITState)
{
  ASSERT(
#ifdef  GPIO_PORT_SIZE_WORD
         (portn == PORTA)||
#endif
         (portn == PORT1)||(portn == PORT2),
         "GPIO_ITConfig",
         "端口设置不合理！");                 //断言检测输入参数

  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO口操作
  
  if(ITState != DISABLE)
  {
    GPIOX[port]->IFG &=~BIT;                  //清中断标志
    GPIOX[port]->IE |= BIT;
  }
  else
  {
    GPIOX[port]->IE &=~BIT;
  }
}
/*******************************************************************************
函数名称：获取某一位端口中断标志
入口参数：PORTn portn    对应IO端口
          uint8_t pin     对应IO引脚位置
*  使用示例：
********************************************************************************/
STATUS GPIO_GetITStatus(PORTn portn,uint8_t pin)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO口操作
  return ((GPIOX[port]->IFG & BIT) ? TRUE : FALSE);
}
/*******************************************************************************
函数名称：清除8位中断标志
入口参数：PORTn portn    对应IO端口
          uint8_t pin     对应IO引脚位置
*  使用示例：
********************************************************************************/
void  GPIO_ClearITPendingBit(PORTn portn,uint8_t pin)
{
  //获取port,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO口操作
  GPIOX[port]->IFG &=~BIT;
}