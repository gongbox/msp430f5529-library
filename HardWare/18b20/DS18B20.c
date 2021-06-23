#include "tempature.h"
#include "gpio.h"

volatile float g_fTempatureValue = 0.0; 
/******************************************* 
函数名称: B20_Init 
功    能: 复位DS18B20 
参    数: 无 
返回值  : 无 
********************************************/ 
void B20_Init(void) 
{ 
  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPO);
  
  DS18B20_OUT = 1;
  DELAY_US(10);
  DS18B20_OUT = 0;
  DELAY_US(750);
  DS18B20_OUT = 1; 
  DELAY_US(60);
 
  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPI);
 
  while(DS18B20_IN); //等待DS18B20拉低 
  while(!DS18B20_IN); //等待DS18B20释放总线 
} 
/******************************************* 
函数名称: B20_ReadByte 
功    能: 读取一个字节的数据 
参    数: 无 
返回值  : data--返回的一个字节数据 
********************************************/ 
uint8_t B20_ReadByte(void) 
{ 
 uint8_t i,data=0; 
 for(i=0;i < 8;i++)     //位计数值 
 { 
  data>>=1;      //右移，准备接受新的数据位

  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPO);

  DS18B20_OUT = 0;//拉低，启动读数据位 
  DS18B20_OUT = 1;  //释放总线 
  DELAY_US(5);      //等待5微秒 
  // PIN_DIR(DS18B20_PORT,DS18B20_PIN) = 0; //配置为输入，开始读取数据位
  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPI);	 
  if(DS18B20_IN)//该位是否为高 
  { 
   data|=0x80;     //是就将此位置高 
  } 
  DELAY_US(50);     //等待50微秒 
 } 
 return data;     //将读到的一个字节返回 
} 
/******************************************* 
函数名称: B20_WriteByte 
功    能: 写入一个字节的数据 
参    数: data--要写入的数据 
返回值  : 无 
********************************************/ 
void B20_WriteByte(uint8_t data) 
{ 
 uint8_t i; 
	
 for(i=0;i<8;i++)            //位计数值 
 { 
   GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPO);
   //PIN_DIR(DS18B20_PORT,DS18B20_PIN) = 1; //配置为输出 
  DS18B20_OUT = 0;//拉低，启动写数据位 
  DELAY_US(1);      //等待1微秒 
  if(data&0x01)     //此位数据是否为高 
  { 
   DS18B20_OUT = 1;//是高则将单总线拉高 
  } 
  else 
  { 
   DS18B20_OUT = 0;//是低则将单总线拉低 
  } 
  DELAY_US(50);      //等待50微秒 
  DS18B20_OUT = 1;//释放总线 
  data>>=1;      //右移，为写入新的数据位做准备 
 } 
 DELAY_US(50);      //等待50微秒 
} 
/******************************************* 
函数名称: B20_Read_temp 
功    能: 读取温度值 
参    数: 无 
返回值  : 温度值(单位：摄氏度) 
********************************************/ 
float B20_Read_temp(void) 
	
{ 
 uint8_t templ,temph; 
 uint16_t temp; 
 float tempature;
 B20_Init();       //初始化，每次写命令都从初始化开始 
 B20_WriteByte(0xcc);    //跳过ROM 
 B20_WriteByte(0x44);    //启动温度转换 
 B20_Init();       //初始化，每次写命令都从初始化开始 
 B20_WriteByte(0xcc);    //跳过ROM 
 B20_WriteByte(0xbe);    //读寄存器 
 templ=B20_ReadByte();   //读温度低字节 
 temph=B20_ReadByte();   //读温度高字节 
 if(((uint32_t)temph*256+templ) > 63488)//(1111 1xxx xxxx xxxx)>63488，大于63488的都是负数
 { 
   temph |= (1<<4);        //负温度标志位
   temp =~(temph*256+templ)+1;//取反加1
 }  
 else
 { 
   temph &=~(1<<4);       //正温度标志位
   temp = (temph*256+templ);
 }
 temp *= 0.625; 
 tempature = temp*0.1; //数字量与温度转化
 return ((temp==0x0352) ? 0.0f : tempature);   //开机启动时为85所以改为0,返回温度值
 
}

