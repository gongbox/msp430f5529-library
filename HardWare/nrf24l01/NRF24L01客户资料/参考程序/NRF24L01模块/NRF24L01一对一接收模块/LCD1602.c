/*****************************************
��������־ǿ
ʱ�䣺2010.6.12
���ܣ�LCD1602Һ������
*****************************************/
#include"reg52.h"
#include"Allhead.h"
sbit RS=P0^0;
sbit RW=P0^1;
sbit EN=P0^2;
#define DBPort P2		
/***************************LCDд�����********************/
void LCDWriteCmd(unchar cmd)
{
   DBPort=cmd;
   RS=0;
   RW=0;
   LCDDelay(5);
   EN=1;
   EN=0;
}
/************************LCDд���ݺ���**************************/
void LCDWriteDate(unchar date)
{
  DBPort=date;
  RS=1;
  RW=0;
  LCDDelay(5);
  EN=1;
  EN=0;
}
/***************************��λ����************************/
void SetXY(unchar x,unchar y)
{
   if(x==0)
	  LCDWriteCmd(0x80+y);		//��һ��
   else
      LCDWriteCmd(0x80+0x40+y); //�ڶ���
}
/***********************��ʾ����****************************/
void LcdShow(unchar x,unchar y,unchar *str)
{
   SetXY(x,y);
   while(*str!='\0')
    {
	  LCDWriteDate(*str);
	  str++;
	}
} 
/**********************LCD��ʼ������***********************/
void LCDInt()
{ 
   LCDWriteCmd(0x38);//����ģʽ
   LCDWriteCmd(0x0c);//����ʾ���
   LCDWriteCmd(0x06);//����ָ���Լ�
   LCDWriteCmd(0x01);//����
}