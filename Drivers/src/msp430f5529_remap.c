#include "msp430f5529_remap.h"

/*******************************************************************************
*  函数名称：GPIO_PinRemapConfig(PORTn portn,uint8_t pin,GPIO_Remap remap)
*  功能说明：GPIO重映射，可以使任一GPIO口（目前只有P4端口（P4.0-P4.7）支持）
             实现可供选择的任一复用功能（参见.h文件的枚举选项）
*  注意    ：某些功能还是需要设置硬件输入输出方向及第二功能
*  参数说明：PORTn portn        :GPIO重映射端口
             uint8_t pin          :GPIO重映射引脚
             GPIO_Remap remap   :GOIO重映射功能
*  函数返回：无
*  使用示例：GPIO_PinRemapConfig(P4,7,GPIO_Remap_MCLK)； //P4.7重映射为MCLK主时钟输出
********************************************************************************/
void  GPIO_PinRemapConfig(PORTn portn,uint8_t pin,GPIO_Remap remap)
{
#ifdef  GPIO_PORT_SIZE_WORD
  ASSERT(((portn == (PORTB)) && (pin>=8))||((portn == (PORT4)) && (pin<=7)),
         "GPIO_PinRemapConfig",
         "portn,pin参数不合理！");              //断言检测输入参数
  if(portn == PORTB)pin = pin - 8;
#else
  ASSERT(portn == PORT4,"GPIO_PinRemapConfig","portn参数不合理！");                             //断言检测输入参数
#endif 
  PMAPPWD = PMAPKEY;                                            //访问钥匙
  *(&P4MAP0 + pin) = remap;                                     //端口重映射配置 
  PMAPCTL |= PMAPRECFG;                                         //允许重新配置的端口映射
  
  if(((remap>=GPIO_Remap_TB0CCR0A) && (remap<=GPIO_Remap_TB0CCR6A)) 
      || (remap==GPIO_Remap_MCLK))      //如果是这些复用功能则需要设为输出方向，也可能不是，其他的也不一定不需要设为输出方向
  {
    GPIO_Init(PORT4,pin,GPO | SEL);
  }
  else
  {
    GPIO_Init(PORT4,pin,SEL);
  }
  PMAPPWD = 0;                                                 //禁止访问
}

