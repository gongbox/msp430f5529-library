#ifndef IIC_H_
#define IIC_H_

#include"common.h"

typedef enum
{                      //端口               SCL          SDA
    I2C0    = 2u,      //I2C_B0            P3.1         P3.0
    I2C1    = 3u,      //I2C_B1            P4.2         P4.1
} I2Cn;         //I2Cn模块
typedef enum
{
    I2C_RX_IRQn     = (0x0001u), /* USCI Receive Interrupt  */
    I2C_TX_IRQn     = (0x0002u), /* USCI Transmit Interrupt  */
    I2C_START_IRQn  = (0x0004u), /* START Condition interrupt  */
    I2C_STOP_IRQn   = (0x0008u), /* STOP Condition interrupt  */
    I2C_AL_IRQn     = (0x0010u), /* Arbitration Lost interrupt  */
    I2C_NACK_IRQn   = (0x0020u), /* NAK Condition interrupt  */
} I2C_IRQn;     //I2Cn中断类别

extern void    I2C_MasterInit (I2Cn, uint16_t SlaveID, uint32_t BaudRate);      //初始化I2C模块（主机模式）
extern void    I2C_SetSlaveID (I2Cn, uint16_t SlaveID);                         //设置从机地址

extern void    I2C_SendByte   (I2Cn i2cn,uint8_t data);                         //发送一字节数据
extern uint8_t I2C_ReadByte   (I2Cn i2cn);                                      //读取一字节数据

extern void    I2C_WriteReg   (I2Cn, uint8_t address, uint8_t data);            //往寄存器写入内容
extern uint8_t I2C_ReadReg    (I2Cn, uint8_t address);                          //读取寄存器的内容

extern void    I2C_ITConfig   (I2Cn, I2C_IRQn, STATUS ITState);                 //设置是否使能I2C的某一个中断
extern STATUS  I2C_GetITStatus(I2Cn, I2C_IRQn);                                 //获取I2C的某一个中断标志
extern void    I2C_ClearITPendingBit(I2Cn, I2C_IRQn);                           //清除I2C的某一个中断标志
#endif
