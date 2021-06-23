//**#include <hidef.h>      /* common defines and macros */
//**#include "derivative.h"      /* derivative-specific definitions */
//**#include "Display.h" 
#include "GY521.h" 
//**#include "Keyboard.h"

 /*=============== GY521 ====================
||函数名：GY521                                                                        ||
||功能GY521初始化及IO口模拟IIC函数                                       ||
||备注XS128总线始终64MHZ时的初始化                                    ||
||           用户只需更改延时函数，精确的1us,程序中大多用              ||
||           到5us的延时，此程序中的delay521_nus(4) 就是5us。      ||
||           还需要更改IO口的输入输出引脚和状态，在头文件中声明    ||
||                                                                                               ||     
||日期  2013.8.2                                                                          ||
=========================================*/


uchar Ack_Count=0;                         //接收数据缓存区  
short    dis_data;

void delay521_nus(uint  n)       //N us延时函数
{
   //**uint i=0,j=0;
   //**for (i=0;i<n;i++)
   //**  for(j=0;j<2;j++);
  while(n--);
}    
void delay521_nms(uint  n)       //N ms延时函数
{
  uint i;
 //**  uint i=0,j=0;
 //**  for (i=0;i<n;i++)
  //**   for (j=0;j<5350;i++);
  while(n--)
    for(i=1000;i>0;i--);
}

/**************************************
起始信号
**************************************/
void IIC_Start()
{
   //** IIC_SDA_Out;
   P1DIR|=(1<<SDA); 
   //** IIC_SDA = 1;                    //拉高数据线
    IIC|=(1<<SDA);
    //*delay521_nus(4) ;
    delay521_nus(1) ;
    //**IIC_SCL = 1;                    //拉高时钟线
    IIC|=(1<<SCL);
    //**delay521_nus(4) ;                //延时
    delay521_nus(1) ;
   //** IIC_SDA = 0;                    //产生下降沿
    IIC&=~(1<<SDA);
   //** delay521_nus(4);                  //延时
    delay521_nus(1) ;
    //**IIC_SCL = 0;                    //拉低时钟线
    IIC&=~(1<<SCL);
   //** delay521_nus(4);
    delay521_nus(1) ;
}

/**************************************
 停止信号
**************************************/
void IIC_Stop()
{
    //**IIC_SDA_Out;
    P1DIR|=(1<<SDA);
    //**IIC_SCL = 0;   
    IIC&=~(1<<SCL);
   //** IIC_SDA = 0;                    //拉低数据线
    IIC&=~(1<<SDA);
    //** delay521_nus(4);                   //延时
    delay521_nus(1) ;
    //**IIC_SCL = 1;                    //拉高时钟线
     IIC|=(1<<SCL);
    //** delay521_nus(4);                  //延时
     delay521_nus(1) ;
   //** IIC_SDA = 1;                    //产生上升沿
     IIC|=(1<<SDA);
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(uchar ack)
{
    //**IIC_SDA_Out;
    P1DIR|=(1<<SDA);
   //** IIC_SCL = 0;                    //拉低时钟线
    IIC&=~(1<<SCL);
   //** delay521_nus(4);
    delay521_nus(1) ;
   //** IIC_SDA = ack;                  //写应答信号
    if(ack)
      IIC|=(1<<SDA);
    else 
      IIC&=~(1<<SDA);
   // delay521_nus(4);
    delay521_nus(1) ;
   //** IIC_SCL = 1;                    //拉高时钟线
    IIC|=(1<<SCL);
  //  delay521_nus(4);                //延时
    delay521_nus(1) ;
    //**IIC_SCL = 0;                    //拉低时钟线
    IIC&=~(1<<SCL);
   //delay521_nus(4);                 //延时
    delay521_nus(1) ;
}

/**************************************
接收应答信号
**************************************/
void IIC_RecvACK()
{
   //**IIC_SDA_In;
   P1DIR&=~(1<<SDA);
   //delay521_nus(4);
  //** IIC_SCL = 1;                  //拉高时钟线
   IIC|=(1<<SCL);
   //** while(IIC_SDA)
   while((P1IN&(1<<SDA)))
    {
        Ack_Count++;
        if(Ack_Count>250)
         {
            IIC_Stop() ;
            Ack_Count=0;
            break;
         }
    }
    //dis_data++;
   //** IIC_SCL = 0;                 //拉低时钟线
   IIC&=~(1<<SCL);
   //** delay521_nus(4);                //延时
   delay521_nus(1) ;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void IIC_SendByte(uchar dat)
{
    uchar i;
       
    // IIC_SDA_Out;
    P1DIR|=(1<<SDA);
      i=8;  
   //**  IIC_SCL = 0;                //拉低时钟线
      IIC&=~(1<<SCL);
   //**  delay521_nus(4);
   delay521_nus(1) ;
   while(i--)         //8位计数器
    {   
     //**  IIC_SDA=(dat&0x80)>>7;
    if(((dat&0x80)>>7))
     IIC|=(1<<SDA);
   else 
     IIC&=~(1<<SDA); 
        dat<<=1;              //送数据口
	 //**   delay521_nus(4);
        delay521_nus(1) ;
    //**    IIC_SCL = 1;                //拉高时钟线
            IIC|=(1<<SCL);
       //** delay521_nus(4);             //延时
            delay521_nus(1) ;
      //**  IIC_SCL = 0;                //拉低时钟线
        IIC&=~(1<<SCL);
        //**delay521_nus(4);              //延时 
        delay521_nus(1) ;
    }  
    // IIC_SDA=0;
   IIC|=(1<<SDA);
     //delay521_nus(2);
    IIC_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
uchar IIC_RecvByte()
{
    uchar i;
    uchar dat = 0;
    // **IIC_SDA_In;
    P1DIR&=~(1<<SDA);
     i=8;  
  
   while(i--)         //8位计数器     
    {   
       //** IIC_SCL = 0;                //拉低时钟线
      IIC&=~(1<<SCL);
      //**  delay521_nus(4);    
      delay521_nus(1) ;
    //**    IIC_SCL = 1;                //拉高时钟线
        IIC|=(1<<SCL);
        dat <<= 1;
      //**  dat |= IIC_SDA;             //读数据   
        if(P1IN&BIT6)
          dat|=1;
        else 
          dat|=0;
    //**    delay521_nus(4);             //延时
        delay521_nus(1) ;
    }
    return dat;
}

//单字节写入*******************************************

void Single_WriteIIC(uchar REG_Address,uchar REG_data)
{
    IIC_Start();                  //起始信号
      
    IIC_SendByte(SlaveAddress);   //发送设备地址+写信号
    IIC_SendByte(REG_Address);    //内部寄存器地址，    
    IIC_SendByte(REG_data);       //内部寄存器数据，
   
    IIC_Stop();                   //发送停止信号
}

//单字节读取*****************************************
uchar Single_ReadIIC(uchar REG_Address)
{  
    uchar REG_data;
   
    IIC_Start();                          //起始信号
    IIC_SendByte(SlaveAddress);           //发送设备地址+写信号
    IIC_SendByte(REG_Address);            //发送存储单元地址，从0开始	
    IIC_Start();                          //起始信号
    IIC_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=IIC_RecvByte();              //读出寄存器数据
	IIC_SendACK(1);   
	IIC_Stop();                           //停止信号
    return REG_data; 
}

//初始化MPU6050，根据需要请参考pdf进行修改************************
void InitMPU6050()
{    
   Single_WriteIIC(PWR_MGMT_1, 0x00);	   //解除休眠状态    
   Single_WriteIIC(SMPLRT_DIV, 0x07);      //陀螺仪采样率，典型值：0x07(125Hz)
   Single_WriteIIC(CONFIG, 0x06);          //低通滤波频率，典型值：0x06(5Hz)
   Single_WriteIIC(GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
   Single_WriteIIC(ACCEL_CONFIG, 0x01);    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

}
//***********************************************************************

int GetData(uchar REG_Address)   //输入你想要参数的寄存器地址，头文件宏定义查询
{
	char H,L;
	H=Single_ReadIIC(REG_Address);
	L=Single_ReadIIC(REG_Address+1);
	return (H<<8)+L;   //合成数据
}


void display_GY521()
{  
    uchar i;
    
    dis_data=GetData(	ACCEL_XOUT_H);
    delay_nms(20);
    
    dis_data=GetData(	GYRO_XOUT_H	);
    delay_nms(20);
    
    dis_data=GetData(	ACCEL_YOUT_H);
    delay_nms(20);
    
    dis_data=GetData(	GYRO_YOUT_H	);
    delay_nms(20);
    
    dis_data=GetData(	ACCEL_ZOUT_H);
    delay_nms(20);
    
    dis_data=GetData(	GYRO_ZOUT_H	);
    delay_nms(20);
    
    
}