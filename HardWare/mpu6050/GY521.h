#ifndef _GY521_H_
#define _GY521_H_
//**#include <hidef.h>      /* common defines and macros */
//**#include <MC9S12XS128.h>     /* derivative information */
#include"msp430g2553.h"
typedef unsigned char uchar;
typedef unsigned int uint;
extern short     dis_data;
//**#define	  IIC_SCL  PORTA_PA0      //IIC时钟引脚定义
//**#define	  IIC_SDA  PORTA_PA1      //IIC数据引脚定义
//**#define	  IIC_SDA_Out  DDRA_DDRA1=0xff         //IIC数据引脚输出定义
//**#define	  IIC_SDA_In     DDRA_DDRA1=0x00         //IIC数据引脚输入定义
//**#define	  IIC_SCL_Out   DDRA_DDRA0=1      //IIC时钟引脚定义
#define IIC_SDA_Out P1DIR|=BIT6
#define IIC_SDA_In  P1DIR&=~BIT6
#define IIC_SCL_Out P1DIR|=BIT7;
#define SDA 6
#define SCL 7
#define IIC P1OUT
//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x68//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取
//****************************************


void InitMPU6050();                 //初始化MPU6050
void display_GY521();
uchar Single_ReadIIC(uchar REG_Address);
void Single_WriteIIC(uchar REG_Address,uchar REG_data);
uchar IIC_RecvByte();
void IIC_SendByte(uchar dat);
void IIC_RecvACK();
void IIC_SendACK(uchar ack);
void IIC_Stop() ;
void IIC_Start();
int GetData(uchar REG_Address);
#endif