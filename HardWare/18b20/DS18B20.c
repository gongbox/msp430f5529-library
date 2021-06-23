#include "tempature.h"
#include "gpio.h"

volatile float g_fTempatureValue = 0.0; 
/******************************************* 
��������: B20_Init 
��    ��: ��λDS18B20 
��    ��: �� 
����ֵ  : �� 
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
 
  while(DS18B20_IN); //�ȴ�DS18B20���� 
  while(!DS18B20_IN); //�ȴ�DS18B20�ͷ����� 
} 
/******************************************* 
��������: B20_ReadByte 
��    ��: ��ȡһ���ֽڵ����� 
��    ��: �� 
����ֵ  : data--���ص�һ���ֽ����� 
********************************************/ 
uint8_t B20_ReadByte(void) 
{ 
 uint8_t i,data=0; 
 for(i=0;i < 8;i++)     //λ����ֵ 
 { 
  data>>=1;      //���ƣ�׼�������µ�����λ

  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPO);

  DS18B20_OUT = 0;//���ͣ�����������λ 
  DS18B20_OUT = 1;  //�ͷ����� 
  DELAY_US(5);      //�ȴ�5΢�� 
  // PIN_DIR(DS18B20_PORT,DS18B20_PIN) = 0; //����Ϊ���룬��ʼ��ȡ����λ
  GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPI);	 
  if(DS18B20_IN)//��λ�Ƿ�Ϊ�� 
  { 
   data|=0x80;     //�Ǿͽ���λ�ø� 
  } 
  DELAY_US(50);     //�ȴ�50΢�� 
 } 
 return data;     //��������һ���ֽڷ��� 
} 
/******************************************* 
��������: B20_WriteByte 
��    ��: д��һ���ֽڵ����� 
��    ��: data--Ҫд������� 
����ֵ  : �� 
********************************************/ 
void B20_WriteByte(uint8_t data) 
{ 
 uint8_t i; 
	
 for(i=0;i<8;i++)            //λ����ֵ 
 { 
   GPIO_Init(DS18B20_PORT,DS18B20_PIN,GPO);
   //PIN_DIR(DS18B20_PORT,DS18B20_PIN) = 1; //����Ϊ��� 
  DS18B20_OUT = 0;//���ͣ�����д����λ 
  DELAY_US(1);      //�ȴ�1΢�� 
  if(data&0x01)     //��λ�����Ƿ�Ϊ�� 
  { 
   DS18B20_OUT = 1;//�Ǹ��򽫵��������� 
  } 
  else 
  { 
   DS18B20_OUT = 0;//�ǵ��򽫵��������� 
  } 
  DELAY_US(50);      //�ȴ�50΢�� 
  DS18B20_OUT = 1;//�ͷ����� 
  data>>=1;      //���ƣ�Ϊд���µ�����λ��׼�� 
 } 
 DELAY_US(50);      //�ȴ�50΢�� 
} 
/******************************************* 
��������: B20_Read_temp 
��    ��: ��ȡ�¶�ֵ 
��    ��: �� 
����ֵ  : �¶�ֵ(��λ�����϶�) 
********************************************/ 
float B20_Read_temp(void) 
	
{ 
 uint8_t templ,temph; 
 uint16_t temp; 
 float tempature;
 B20_Init();       //��ʼ����ÿ��д����ӳ�ʼ����ʼ 
 B20_WriteByte(0xcc);    //����ROM 
 B20_WriteByte(0x44);    //�����¶�ת�� 
 B20_Init();       //��ʼ����ÿ��д����ӳ�ʼ����ʼ 
 B20_WriteByte(0xcc);    //����ROM 
 B20_WriteByte(0xbe);    //���Ĵ��� 
 templ=B20_ReadByte();   //���¶ȵ��ֽ� 
 temph=B20_ReadByte();   //���¶ȸ��ֽ� 
 if(((uint32_t)temph*256+templ) > 63488)//(1111 1xxx xxxx xxxx)>63488������63488�Ķ��Ǹ���
 { 
   temph |= (1<<4);        //���¶ȱ�־λ
   temp =~(temph*256+templ)+1;//ȡ����1
 }  
 else
 { 
   temph &=~(1<<4);       //���¶ȱ�־λ
   temp = (temph*256+templ);
 }
 temp *= 0.625; 
 tempature = temp*0.1; //���������¶�ת��
 return ((temp==0x0352) ? 0.0f : tempature);   //��������ʱΪ85���Ը�Ϊ0,�����¶�ֵ
 
}

