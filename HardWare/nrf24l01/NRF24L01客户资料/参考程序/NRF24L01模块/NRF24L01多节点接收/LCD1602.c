/*****************************************
创建：陈志强
时间：2010.6.12
功能：LCD1602液晶驱动
*****************************************/
#include"reg52.h"
#include"Allhead.h"
sbit RS=P0^0;
sbit RW=P0^1;
sbit EN=P0^2;
#define DBPort P2		
/***************************LCD写命令函数********************/
void LCDWriteCmd(unchar cmd)
{
   DBPort=cmd;
   RS=0;
   RW=0;
   LCDDelay(5);
   EN=1;
   EN=0;
}
/************************LCD写数据函数**************************/
void LCDWriteDate(unchar date)
{
  DBPort=date;
  RS=1;
  RW=0;
  LCDDelay(5);
  EN=1;
  EN=0;
}
/***************************定位函数************************/
void SetXY(unchar x,unchar y)
{
   if(x==0)
	  LCDWriteCmd(0x80+y);		//第一行
   else
      LCDWriteCmd(0x80+0x40+y); //第二行
}
/***********************显示函数****************************/
void LcdShow(unchar x,unchar y,unchar *str)
{
   SetXY(x,y);
   while(*str!='\0')
    {
	  LCDWriteDate(*str);
	  str++;
	}
} 
/**********************LCD初始化函数***********************/
void LCDInt()
{ 
   LCDWriteCmd(0x38);//设置模式
   LCDWriteCmd(0x0c);//不显示光标
   LCDWriteCmd(0x06);//设置指针自加
   LCDWriteCmd(0x01);//清屏
}