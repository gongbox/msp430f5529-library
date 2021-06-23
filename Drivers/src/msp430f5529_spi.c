#include "msp430f5529_spi.h"
#include "msp430f5529_clock.h"
#include "msp430f5529_gpio.h"

const USCI_MemMapPtr USCIX[]={USCI_A0_BASE_PTR,USCI_A1_BASE_PTR,USCI_B0_BASE_PTR,USCI_B1_BASE_PTR};

const struct
{
  GPIO_PIN STE_PIN ; //
  GPIO_PIN SIMO_PIN; //
  GPIO_PIN SOMI_PIN; //
  GPIO_PIN CLK_PIN ; //
}SPI_PIN[]=
{ //        STE             SIMO             SOMI             CLK
  /*SPI0*/{.STE_PIN={P3,2},.SIMO_PIN={P3,3},.SOMI_PIN={P3,4},.CLK_PIN={P2,7}},    
  /*SPI1*/{.STE_PIN={P4,3},.SIMO_PIN={P4,4},.SOMI_PIN={P4,5},.CLK_PIN={P4,0}},       
  /*SPI2*/{.STE_PIN={P2,1},.SIMO_PIN={P3,0},.SOMI_PIN={P3,1},.CLK_PIN={P3,2}},    
  /*SPI3*/{.STE_PIN={P4,0},.SIMO_PIN={P4,1},.SOMI_PIN={P4,2},.CLK_PIN={P4,3}},  
};//引脚初始化数组
/*******************************************************************************
*  函数名称：SPI_Master_Init (SPIn spin,uint32_t BaudRate,FirstBit firstbit, SPI_STE_Mode spistemode,SPI_CLK_Mode spiclkmode)
*  功能说明：SPI初始化
*  参数说明：SPIn spin                  :SPI模块号
             uint32_t BaudRate          :传输波特率
             SPI_FirstBit firstbit      :先发送高位或低位数据
             SPI_STE_Mode spistemode    :STE模式
             SPI_CLK_Mode spiclkmode    :SPI时钟模式
*  函数返回：无
*  使用例程：SPI_Master_Init(SPI2,10000000,MSB_First,SPI_STE_None,SPI_CLK_Mode_4); //初始化SPI模块，波特率为10000000，先发送高位数据，STE信号屏蔽，时钟线模式4
********************************************************************************/
void SPI_Master_Init (SPIn spin,uint32_t BaudRate,SPI_FirstBit firstbit, SPI_STE_Mode spistemode,SPI_CLK_Mode spiclkmode)
{
  WordType BR;
  BR.Word = g_sClock.SMCLK.nHZ/BaudRate;        //求取波特率所需的分频系数
  if(spistemode != SPI_STE_None)
  {
    GPIO_Init(SPI_PIN[spin].STE_PIN.Port,SPI_PIN[spin].STE_PIN.Pin,SEL);     //片选信号引脚设为第二功能
  }
  GPIO_Init(SPI_PIN[spin].SIMO_PIN.Port,SPI_PIN[spin].SIMO_PIN.Pin,SEL);     //主输出从输入信号引脚设为第二功能
  GPIO_Init(SPI_PIN[spin].SOMI_PIN.Port,SPI_PIN[spin].SOMI_PIN.Pin,SEL);     //主输入从输出信号引脚设为第二功能
  GPIO_Init(SPI_PIN[spin].CLK_PIN.Port,SPI_PIN[spin].CLK_PIN.Pin,SEL);       //时钟信号引脚设为第二功能
  
  USCIX[spin]->CTL1 = UCSWRST; 
  USCIX[spin]->SYNC = BIT_SET;                             //同步模式
  USCIX[spin]->MST = BIT_SET;                              //主机模式
  USCIX[spin]->MODE = spistemode;                      //片选信号设置
  USCIX[spin]->CKMODE = spiclkmode;                    //时钟线模式设置
  USCIX[spin]->MSB = firstbit;                         //发送首位设置
  
  USCIX[spin]->SSEL= USCI_SSEL__SMCLK;                 //时钟源设置
  
  USCIX[spin]->BRW = BR.Word;                          //BRW写1与写0发现波特率没有区别
  USCIX[spin]->CTL1 &=~ UCSWRST; 
  
  USCIX[spin]->IE = 0u;                                         //关闭所有中断
  USCIX[spin]->RXIFG = RESET;                                   //清除接收数据标志
  USCIX[spin]->TXIFG = BIT_SET;                                     //置位发送缓冲区为空标志
}
/*******************************************************************************
函数名称：SPI_SetBaudRate (SPIn spin, uint32_t BaudRate)     
功能说明：设置SPI波特率
参数说明：SPIn spin                  :SPI模块号
          uint32_t BaudRate            :传输波特率
函数返回：无
*******************************************************************************/
void  SPI_SetBaudRate (SPIn spin, uint32_t BaudRate)                  
{
  USCIX[spin]->CTL1 = UCSSEL__SMCLK + UCSWRST;                //设置时钟源
 
  USCIX[spin]->BRW =(uint16_t)g_sClock.SMCLK.nHZ/BaudRate;                                 //设置波特率

  USCIX[spin]->CTL1 &= ~UCSWRST;      
}
/*******************************************************************************
*  函数名称：SPI_SendReadByte(SPIn spin,uint8_t data)
*  功能说明：SPI发送并接收一字节数据，发送的同时一定会接收数据
*  参数说明：SPIn spin :SPI模块号
             uint8_t data:要发送的数据
*  函数返回：读取到的数据
*  使用示例：SPI_SendReadByte(SPIn spin,uint8_t data)
*  注意：不提供片选信号，需要自己使用IO口模拟CS片选信号
********************************************************************************/
uint8_t SPI_SendReadByte(SPIn spin,uint8_t data)
{
  while(USCIX[spin]->UC_BUSY == BIT_SET);                   //判断SPIn硬件是否忙
  USCIX[spin]->TXBUF = data;                            // 写入要发送的数据
  while(USCIX[spin]->UC_BUSY == BIT_SET);                   //判断SPIn硬件是否忙
  return USCIX[spin]->RXBUF;                            //返回接收到的数据,读取RXBUF后RXIFG位自动清除
}
/*******************************************************************************
*  函数名称：SPI_SendReadString(SPIn spin,char * SendData,char * ReceiveData,uint16_t Length)
*  功能说明：SPIn发送并接收多字节数据
*  参数说明：SPIn spin          :SPI模块号
             char * SendData    :发送数据地址
             char * ReceiveData :接收数据缓存地址
             uint16_t Length      :要发送数据字节数
*  函数返回：无
*  使用示例：char SendData[20] = "Hello!";
             char ReceiveData[20];
             SPI_SendReadString(SPI0,SendData,ReceiveData,strlen("Hello!")); //SPI0发送字符串
*  注意：不提供片选信号，需要自己使用IO口模拟CS片选信号
********************************************************************************/
void SPI_SendReadString(SPIn spin,char * SendData,char * ReceiveData,uint16_t Length)
{
  while(Length--)
  {
    *ReceiveData++=SPI_SendReadByte(spin, *SendData++);
  }
}
/*******************************************************************************
*  函数名称：SPI_ITConfig    (SPIn spin,SPI_IRQn irqn,STATUS ITState)
*  功能说明：设置SPI某一中断是否使能
*  参数说明：SPIn spin          :SPI模块号
             SPI_IRQn irqn      :中断事件选择
             STATUS ITState     :中断是否使能
*  函数返回：无
*  使用示例：SPI_ITConfig    (SPI1,SPI_RX_IRQn,ENABLE);  //使能SPI1的接收中断
********************************************************************************/
void  SPI_ITConfig    (SPIn spin,SPI_IRQn irqn,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    USCIX[spin]->IE |= irqn;
  }
  else
  {
    USCIX[spin]->IE &=~irqn;
  }
}
/*******************************************************************************
*  函数名称：SPI_GetITStatus(SPIn spin,SPI_IRQn irqn)
*  功能说明：获取SPI中断标志位状态
*  参数说明：SPIn spin          :SPI模块号
             SPI_IRQn irqn      :中断事件选择
*  函数返回：STATUS :TRUE-中断事件发生 FALSE-中断事件未发生
*  使用示例：if(TRUE == SPI_GetITStatus(SPI1,SPI_RX_IRQn)){...}  //判断SPI1接收中断事件是否发生
********************************************************************************/
STATUS  SPI_GetITStatus(SPIn spin,SPI_IRQn irqn)
{
  return ((USCIX[spin]->IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  函数名称：SPI_ClearITPendingBit(SPIn spin,SPI_IRQn irqn)
*  功能说明：SPIn清除中断标志位状态
*  参数说明：SPIn spin          :SPI模块号
             SPI_IRQn irqn      :中断事件选择
*  函数返回：无
*  使用示例：SPI_ClearITPendingBit(SPI1,SPI_RX_IRQn);  //清除SPI1接收中断标志
********************************************************************************/
void   SPI_ClearITPendingBit(SPIn spin,SPI_IRQn irqn)
{
   USCIX[spin]->IFG &=~ irqn;
}