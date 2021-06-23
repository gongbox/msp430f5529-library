/********************************************************************
创建人：陈志强
时间：2010.5.17
功能：MAX232驱动
*********************************************************************/
#include"reg52.h"
#include"allhead.h"	
unchar RevFlags;
/*************************串口初始化函数**************/
void Max232Int()
{
	PCON=0x00;   //SMOD=0
	TMOD=0x20;  //设置T1为工作方式2
	TH1=0xfd;  //9600
	TL1=0xfd;
	SM0=0;
	SM1=1;//方式1,8位
	EA=1;
	ES=1;
	TR1=1;//定时器1允许
	REN=1;//允许接收
}
/*************************串口发送一个字节函数********************/
void Max232SendChar(unchar date)
{
   ES=0;
   SBUF=date;
   while(!TI);
     TI=0;
     ES=1;
}
/**********************发送一个字符串******************************/
void Max232SendAChar(unchar *string)
{
  while(*string!='\0')
   {
      Max232SendChar(*string);
	  string++;
   }
}
/**********************串口中断接收函数****************************/
void ExInt() interrupt 4 
{
  ES=0;
  if(RI)
    {
	  RI=0;
	  RevFlags=SBUF;
	}
  ES=1;
} 
/*********************发送NRF24L01采集到的数据********************/
void MAX232SendDate()
{
	if(RevFlags=='S')
	{
		 RevFlags=0;
		 Max232SendAChar(RevTempDate0);
		 Max232SendAChar("12.3");
		 Max232SendAChar("32.6");
		 Max232SendAChar("56.7");
		 Max232SendAChar("36.9");
		 Max232SendAChar("89.4");
	}
	//Max232SendChar('E');//发送结束标志
}