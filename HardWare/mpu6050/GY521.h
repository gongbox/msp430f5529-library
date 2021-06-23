#ifndef _GY521_H_
#define _GY521_H_
//**#include <hidef.h>      /* common defines and macros */
//**#include <MC9S12XS128.h>     /* derivative information */
#include"msp430g2553.h"
typedef unsigned char uchar;
typedef unsigned int uint;
extern short     dis_data;
//**#define	  IIC_SCL  PORTA_PA0      //IICʱ�����Ŷ���
//**#define	  IIC_SDA  PORTA_PA1      //IIC�������Ŷ���
//**#define	  IIC_SDA_Out  DDRA_DDRA1=0xff         //IIC���������������
//**#define	  IIC_SDA_In     DDRA_DDRA1=0x00         //IIC�����������붨��
//**#define	  IIC_SCL_Out   DDRA_DDRA0=1      //IICʱ�����Ŷ���
#define IIC_SDA_Out P1DIR|=BIT6
#define IIC_SDA_In  P1DIR&=~BIT6
#define IIC_SCL_Out P1DIR|=BIT7;
#define SDA 6
#define SCL 7
#define IIC P1OUT
//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x68//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
//****************************************


void InitMPU6050();                 //��ʼ��MPU6050
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