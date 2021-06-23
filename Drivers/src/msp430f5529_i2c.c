#include "msp430f5529_i2c.h"
#include "msp430f5529_clock.h" 
#include "msp430f5529_system.h"
#include "msp430f5529_gpio.h"

const struct{
  GPIO_PIN SCL;
  GPIO_PIN SDA;
}I2C_PIN[]=
{ //        SCL         SDA
  /*I2C0*/{.SCL={P3,1},.SDA={P3,0}},           
  /*I2C1*/{.SCL={P4,2},.SDA={P4,1}}          
};
static inline void  I2C_Start     (I2Cn i2cn);          //发送一个起始信号
static inline void  I2C_Stop      (I2Cn i2cn);          //发送一个停止信号
static inline void  I2C_WaitBusy  (I2Cn i2cn);          //等待空闲
static inline void  I2C_EnterSend (I2Cn i2cn);          //进入发送模式
static inline void  I2C_EnterRead (I2Cn i2cn);          //进入接收模式
/*******************************************************************************
*  函数名称：I2C_MasterInit(I2Cn i2cn,uint16_t SlaveID,uint32_t BaudRate) 
*  功能说明：初始化一个模块为I2C主机模块，并设置从机地址及传输波特率
*  参数说明：I2Cn i2cn      :模块号
             uint8_t SlaveID  :从机地址
             uint32_t BaudRate:传输波特率
*  函数返回：无
*  使用示例：I2C_MasterInit   (I2C0, 0x68,100000);  //初始化I2C0模块为主机，从机地址为0x68,设置传输波特率为100000
********************************************************************************/
void  I2C_MasterInit(I2Cn i2cn,uint16_t SlaveID,uint32_t BaudRate)                                       
{
  WordType BR;
  BR.Word=g_sClock.SMCLK.nHZ/BaudRate;        //求取波特率所需的分频系数
  GPIO_Init(I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,GPO);
  //输出9个时钟以恢复I2Cn总线状态  
  for(uint8_t i=0;i<9;i++)
  {
    GPIO_WriteBit  (I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,  BIT_SET);
    DELAY_US(5);
    GPIO_WriteBit  (I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,RESET);
    DELAY_US(5);
  }
  //初始化引脚
  GPIO_Init(I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,SEL);
  GPIO_Init(I2C_PIN[i2cn-I2C0].SDA.Port,I2C_PIN[i2cn-I2C0].SDA.Pin,SEL);
  //初始化寄存器
  USCIX[i2cn]->CTL1 = UCSWRST;                       // 软件复位使能，保持复位状态
  USCIX[i2cn]->CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C主机，同步模式
  if(SlaveID > 0x7F)
  {
    USCIX[i2cn]->SLA10 = BIT_SET;
  }
  else
  {
    USCIX[i2cn]->SLA10 = RESET;
  }
  USCIX[i2cn]->I2CSA = SlaveID;
  USCIX[i2cn]->CTL1 |= UCSSEL__SMCLK;                 //使用SMCLK作为时钟源
  USCIX[i2cn]->BR0 = BR.Byte[0];
  USCIX[i2cn]->BR1 = BR.Byte[1];                        
  USCIX[i2cn]->CTL1 &=~ UCSWRST;                      //清除软件复位，正常操作
  
  USCIX[i2cn]->IE = 0u;                                         //关闭所有中断
  USCIX[i2cn]->RXIFG = RESET;                                   //清除接收数据标志
  USCIX[i2cn]->TXIFG = BIT_SET;                                     //置位发送缓冲区为空标志 
}
/*******************************************************************************
*  函数名称：I2C_SetSlaveID(I2Cn i2cn,uint16_t SlaveID)
*  功能说明：设置从机地址
*  参数说明： I2Cn i2cn      :模块号
              uint8_t SlaveID  :从机地址
*  函数返回：无
*  使用示例：I2C_SetSlaveID(I2C1,0x68);   //设置I2C1模块的从机地址为0x68
********************************************************************************/
void I2C_SetSlaveID(I2Cn i2cn,uint16_t SlaveID)
{
  if(SlaveID > 0x7F)
  {
    USCIX[i2cn]->SLA10 = BIT_SET;
  }
  else
  {
    USCIX[i2cn]->SLA10 = RESET;
  }
  USCIX[i2cn]->I2CSA = SlaveID;
}
/*******************************************************************************
*  函数名称：I2C_Start(I2Cn i2cn)
*  功能说明：发送一个起始信号
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：无
*  使用示例：I2C_Start (I2C1);  //I2C1模块发送一个起始信号
********************************************************************************/
inline void I2C_Start(I2Cn i2cn)
{
  while((USCIX[i2cn]->UC_BUSY==BIT_SET) || (USCIX[i2cn]->TXSTP==BIT_SET));//确保总线空闲
  USCIX[i2cn]->TXSTT = BIT_SET;
}
/*******************************************************************************
*  函数名称：I2C_Stop(I2Cn i2cn)
*  功能说明：发送一个停止信号
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：无
*  使用示例：I2C_Stop (I2C1);  //I2C1模块发送一个停止信号
********************************************************************************/
inline void I2C_Stop(I2Cn i2cn)
{
  while(USCIX[i2cn]->UC_BUSY == BIT_SET);                 //等待空闲
  USCIX[i2cn]->TXSTP =BIT_SET;                            //发送停止位
  while(USCIX[i2cn]->TXSTP == BIT_SET);                   //等待停止位发送完成
}
/*******************************************************************************
*  函数名称：I2C_EnterSend (I2Cn i2cn)
*  功能说明：I2C进入发送模式
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：无
*  使用示例：I2C_EnterSend (I2C1);  //I2C1模块进入写模式
********************************************************************************/
inline void I2C_EnterSend (I2Cn i2cn)
{
  USCIX[i2cn]->TR = BIT_SET;                              //进入发送模式
}
/*******************************************************************************
*  函数名称：I2C_EnterRead (I2Cn i2cn)
*  功能说明：I2C进入接收模式
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：无
*  使用示例：I2C_EnterRead (I2C1);  //I2C1模块进入读模式
********************************************************************************/
inline void I2C_EnterRead (I2Cn i2cn)
{
  USCIX[i2cn]->TR = RESET;                             //进入接收模式
}
/*******************************************************************************
*  函数名称：I2C_SendByte (I2Cn i2cn,uint8_t data)
*  功能说明：I2C发送一字节数据
*  参数说明：I2Cn i2cn      :模块号
             uint8_t data     :要发送的数据
*  函数返回：无
*  使用示例：I2C_SendByte (I2C1,0x01);  //I2C1模块发送一字节数据0x01
********************************************************************************/
inline void I2C_SendByte (I2Cn i2cn,uint8_t data)
{
  while(USCIX[i2cn]->TXIFG == RESET);                   //等待
  USCIX[i2cn]->TXBUF = data;                            //发送要写入的数据
}
/*******************************************************************************
*  函数名称：I2C_ReadByte  (I2Cn i2cn)
*  功能说明：IIC读取一个字节数据
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：读取到的数据
*  使用示例：uint8 data = I2C_ReadByte  (I2C1);   //读取数据
********************************************************************************/
inline uint8_t I2C_ReadByte  (I2Cn i2cn)
{
  while(USCIX[i2cn]->RXIFG == RESET);                 //等待接收到数据
  return USCIX[i2cn]->RXBUF;
}
/*******************************************************************************
*  函数名称：I2C_WaitBusy(I2Cn i2cn)
*  功能说明：I2C等待空闲
*  参数说明：I2Cn i2cn      :模块号
*  函数返回：无
*  使用示例：I2C_WaitBusy(I2C1);  //等待I2C1模块不忙
********************************************************************************/
inline void I2C_WaitBusy(I2Cn i2cn)
{
  while(USCIX[i2cn]->UC_BUSY == BIT_SET);                    //等待发送或接收完成
}
/*******************************************************************************
*  函数名称： I2C_WriteReg(I2Cn i2cn, uint8_t address, char data)
*  功能说明： 往某一个地址写入一字节数据
*  参数说明： I2Cn i2cn      :模块号
              uint8_t address :寄存器地址
              uint8_t data    :对该地址要写入的数据内容
*  函数返回：无
*  使用示例：I2C_WriteReg(I2C1, 0x20, 0x12);   //对地址为0x20处写入内容0x12
********************************************************************************/
void  I2C_WriteReg(I2Cn i2cn, uint8_t address, uint8_t data)     
{
  I2C_EnterSend(i2cn);
  I2C_Start    (i2cn);                        //发送一个起始信号
  I2C_SendByte (i2cn,address);                //发送要写入的地址
  I2C_SendByte (i2cn,data);                   //发送要写入的数据
  I2C_Stop     (i2cn);                        //发送停止位
  I2C_WaitBusy (i2cn);
}
/*******************************************************************************
*  函数名称： I2C_ReadReg(I2Cn i2cn, uint8_t address) 
*  功能说明： 对外部芯片读取某一地址的内容
*  参数说明： I2Cn i2cn      :模块号
              uint8_t address :寄存器地址
*  函数返回： 读取到的内容
*  使用示例： uint8_t data = I2C_ReadReg(I2C1, 0x20);  //读取寄存器地址为0x20处的内容
********************************************************************************/
uint8_t    I2C_ReadReg(I2Cn i2cn, uint8_t address)             
{
  I2C_EnterSend (i2cn);                       //进入发送模式
  I2C_Start     (i2cn);                       //发送一个起始信号
  I2C_SendByte  (i2cn,address);               //发送一字节数据
  I2C_WaitBusy  (i2cn);                       //等待传输完毕
  I2C_EnterRead (i2cn);                       //进入接收模式
  I2C_Start     (i2cn);                       //发送一个起始信号
  I2C_WaitBusy  (i2cn);                       //等待空闲
  I2C_Stop      (i2cn);                       //发送一个停止信号,读的话要先发送停止位
  return I2C_ReadByte(i2cn);                  //读取数据
}
/*******************************************************************************
*  函数名称： I2C_ITConfig (I2Cn i2cn,I2C_IRQn irqn,STATUS ITState)
*  功能说明： 设置使能或禁止I2Cn的某一个中断
*  参数说明： I2Cn i2cn      :模块号
              I2C_IRQn irqn :中断类型
*  函数返回：无
*  使用示例：I2C_ITConfig (I2C1,I2C_RX_IRQn,ENABLE);  //使能I2C1的接收中断
********************************************************************************/
void  I2C_ITConfig (I2Cn i2cn,I2C_IRQn irqn,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    USCIX[i2cn]->IE |= irqn;
  }
  else
  {
    USCIX[i2cn]->IE &=~irqn;
  }
}
/*******************************************************************************
*  函数名称： I2C_GetITStatus(I2Cn i2cn,I2C_IRQn irqn)
*  功能说明： 获取I2C的某一个中断标志
*  参数说明： I2Cn i2cn      :模块号
              I2C_IRQn irqn :中断类型
*  函数返回： STATUS : TRUE 中断事件发生，FALSE 中断事件未发生
*  使用示例： if(TRUE == I2C_GetITStatus(I2C0,I2C_RX_IRQn)){...} //判断I2C0模块是否接收完成事件中断发生
********************************************************************************/
STATUS I2C_GetITStatus(I2Cn i2cn,I2C_IRQn irqn)
{
  return ((USCIX[i2cn]->IFG & irqn) ? TRUE :FALSE);
}
/*******************************************************************************
*  函数名称： I2C_ClearITPendingBit(I2Cn i2cn,I2C_IRQn irqn)
*  功能说明： 清除I2Cn的某一个中断标志
*  参数说明： I2Cn i2cn      :模块号
              I2C_IRQn irqn :中断类型
*  函数返回： 无
*  使用示例： I2C_ClearITPendingBit(I2C0,I2C_RX_IRQn);   //清除I2C1模块接收中断标志位
********************************************************************************/
void  I2C_ClearITPendingBit(I2Cn i2cn,I2C_IRQn irqn)
{
  USCIX[i2cn]->IFG &=~ irqn;
}