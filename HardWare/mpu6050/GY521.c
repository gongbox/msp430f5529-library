//**#include <hidef.h>      /* common defines and macros */
//**#include "derivative.h"      /* derivative-specific definitions */
//**#include "Display.h" 
#include "GY521.h" 
//**#include "Keyboard.h"

 /*=============== GY521 ====================
||��������GY521                                                                        ||
||���ܣPGY521��ʼ����IO��ģ��IIC����                                       ||
||��ע�zXS128����ʼ��64MHZʱ�ĳ�ʼ��                                    ||
||           �û�ֻ�������ʱ��������ȷ��1us,�����д����              ||
||           ��5us����ʱ���˳����е�delay521_nus(4) ����5us��      ||
||           ����Ҫ����IO�ڵ�����������ź�״̬����ͷ�ļ�������    ||
||                                                                                               ||     
||����  2013.8.2                                                                          ||
=========================================*/


uchar Ack_Count=0;                         //�������ݻ�����  
short    dis_data;

void delay521_nus(uint  n)       //N us��ʱ����
{
   //**uint i=0,j=0;
   //**for (i=0;i<n;i++)
   //**  for(j=0;j<2;j++);
  while(n--);
}    
void delay521_nms(uint  n)       //N ms��ʱ����
{
  uint i;
 //**  uint i=0,j=0;
 //**  for (i=0;i<n;i++)
  //**   for (j=0;j<5350;i++);
  while(n--)
    for(i=1000;i>0;i--);
}

/**************************************
��ʼ�ź�
**************************************/
void IIC_Start()
{
   //** IIC_SDA_Out;
   P1DIR|=(1<<SDA); 
   //** IIC_SDA = 1;                    //����������
    IIC|=(1<<SDA);
    //*delay521_nus(4) ;
    delay521_nus(1) ;
    //**IIC_SCL = 1;                    //����ʱ����
    IIC|=(1<<SCL);
    //**delay521_nus(4) ;                //��ʱ
    delay521_nus(1) ;
   //** IIC_SDA = 0;                    //�����½���
    IIC&=~(1<<SDA);
   //** delay521_nus(4);                  //��ʱ
    delay521_nus(1) ;
    //**IIC_SCL = 0;                    //����ʱ����
    IIC&=~(1<<SCL);
   //** delay521_nus(4);
    delay521_nus(1) ;
}

/**************************************
 ֹͣ�ź�
**************************************/
void IIC_Stop()
{
    //**IIC_SDA_Out;
    P1DIR|=(1<<SDA);
    //**IIC_SCL = 0;   
    IIC&=~(1<<SCL);
   //** IIC_SDA = 0;                    //����������
    IIC&=~(1<<SDA);
    //** delay521_nus(4);                   //��ʱ
    delay521_nus(1) ;
    //**IIC_SCL = 1;                    //����ʱ����
     IIC|=(1<<SCL);
    //** delay521_nus(4);                  //��ʱ
     delay521_nus(1) ;
   //** IIC_SDA = 1;                    //����������
     IIC|=(1<<SDA);
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(uchar ack)
{
    //**IIC_SDA_Out;
    P1DIR|=(1<<SDA);
   //** IIC_SCL = 0;                    //����ʱ����
    IIC&=~(1<<SCL);
   //** delay521_nus(4);
    delay521_nus(1) ;
   //** IIC_SDA = ack;                  //дӦ���ź�
    if(ack)
      IIC|=(1<<SDA);
    else 
      IIC&=~(1<<SDA);
   // delay521_nus(4);
    delay521_nus(1) ;
   //** IIC_SCL = 1;                    //����ʱ����
    IIC|=(1<<SCL);
  //  delay521_nus(4);                //��ʱ
    delay521_nus(1) ;
    //**IIC_SCL = 0;                    //����ʱ����
    IIC&=~(1<<SCL);
   //delay521_nus(4);                 //��ʱ
    delay521_nus(1) ;
}

/**************************************
����Ӧ���ź�
**************************************/
void IIC_RecvACK()
{
   //**IIC_SDA_In;
   P1DIR&=~(1<<SDA);
   //delay521_nus(4);
  //** IIC_SCL = 1;                  //����ʱ����
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
   //** IIC_SCL = 0;                 //����ʱ����
   IIC&=~(1<<SCL);
   //** delay521_nus(4);                //��ʱ
   delay521_nus(1) ;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void IIC_SendByte(uchar dat)
{
    uchar i;
       
    // IIC_SDA_Out;
    P1DIR|=(1<<SDA);
      i=8;  
   //**  IIC_SCL = 0;                //����ʱ����
      IIC&=~(1<<SCL);
   //**  delay521_nus(4);
   delay521_nus(1) ;
   while(i--)         //8λ������
    {   
     //**  IIC_SDA=(dat&0x80)>>7;
    if(((dat&0x80)>>7))
     IIC|=(1<<SDA);
   else 
     IIC&=~(1<<SDA); 
        dat<<=1;              //�����ݿ�
	 //**   delay521_nus(4);
        delay521_nus(1) ;
    //**    IIC_SCL = 1;                //����ʱ����
            IIC|=(1<<SCL);
       //** delay521_nus(4);             //��ʱ
            delay521_nus(1) ;
      //**  IIC_SCL = 0;                //����ʱ����
        IIC&=~(1<<SCL);
        //**delay521_nus(4);              //��ʱ 
        delay521_nus(1) ;
    }  
    // IIC_SDA=0;
   IIC|=(1<<SDA);
     //delay521_nus(2);
    IIC_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
uchar IIC_RecvByte()
{
    uchar i;
    uchar dat = 0;
    // **IIC_SDA_In;
    P1DIR&=~(1<<SDA);
     i=8;  
  
   while(i--)         //8λ������     
    {   
       //** IIC_SCL = 0;                //����ʱ����
      IIC&=~(1<<SCL);
      //**  delay521_nus(4);    
      delay521_nus(1) ;
    //**    IIC_SCL = 1;                //����ʱ����
        IIC|=(1<<SCL);
        dat <<= 1;
      //**  dat |= IIC_SDA;             //������   
        if(P1IN&BIT6)
          dat|=1;
        else 
          dat|=0;
    //**    delay521_nus(4);             //��ʱ
        delay521_nus(1) ;
    }
    return dat;
}

//���ֽ�д��*******************************************

void Single_WriteIIC(uchar REG_Address,uchar REG_data)
{
    IIC_Start();                  //��ʼ�ź�
      
    IIC_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    IIC_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��    
    IIC_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
   
    IIC_Stop();                   //����ֹͣ�ź�
}

//���ֽڶ�ȡ*****************************************
uchar Single_ReadIIC(uchar REG_Address)
{  
    uchar REG_data;
   
    IIC_Start();                          //��ʼ�ź�
    IIC_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    IIC_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    IIC_Start();                          //��ʼ�ź�
    IIC_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=IIC_RecvByte();              //�����Ĵ�������
	IIC_SendACK(1);   
	IIC_Stop();                           //ֹͣ�ź�
    return REG_data; 
}

//��ʼ��MPU6050��������Ҫ��ο�pdf�����޸�************************
void InitMPU6050()
{    
   Single_WriteIIC(PWR_MGMT_1, 0x00);	   //�������״̬    
   Single_WriteIIC(SMPLRT_DIV, 0x07);      //�����ǲ����ʣ�����ֵ��0x07(125Hz)
   Single_WriteIIC(CONFIG, 0x06);          //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
   Single_WriteIIC(GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
   Single_WriteIIC(ACCEL_CONFIG, 0x01);    //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

}
//***********************************************************************

int GetData(uchar REG_Address)   //��������Ҫ�����ļĴ�����ַ��ͷ�ļ��궨���ѯ
{
	char H,L;
	H=Single_ReadIIC(REG_Address);
	L=Single_ReadIIC(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
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