/**********************************************************
创建人：陈志强
时间：2009.9.10
功能：DS18B20函数							  
***********************************************************/
#include"reg52.h"
#include"allhead.h"
sbit DQ=P1^0;//数据端
unint tempL,tempH;
/********************************************************* 
	DS18B20复位函数
**********************************************************/
void Reset() 
{   
	DQ=1;
	DDelay(8); 
	DQ=0; 
	DDelay(85); 
	DQ=1;
	DDelay(14);

} 
/*********************************************************** 
	DS18B20写一个字节函数
*********************************************************/
void WriteChar(unchar dat) 
{ 
	  unint i; 
	  for(i=8;i>0;i--) 
	  { 
		DQ=0;   
		DQ=dat&0x01;
		DDelay(5);
		DQ=1;
		dat>>=1;
	   } 
	 DDelay(4);
} 
/**************************************************** 
	 DS18B20读一个字节函数
****************************************************/
unint ReadChar() 
{			 
	unchar i,dat=0;
	for (i=8;i>0;i--) 
	{ 
		DQ=1; 
		DDelay(1);
		DQ=0;  
		dat>>=1;
		DQ=1; 
		if(DQ)
		  dat=dat|0x80;
		  DDelay(4);
     } 
return(dat);
} 
/*************************************************** 
	DS18B20读温度程序
****************************************************/ 
unint ReadTemperature() 
{ 
    unint temperature;
	Reset(); 
	WriteChar(0xcc); 
	WriteChar(0x44); 
	DDelay(125); 
	Reset();
	WriteChar(0xcc);
	WriteChar(0xbe);
	tempL=ReadChar(); 
	tempH=ReadChar();
	temperature=((tempH*256)+tempL)*0.0625*10; 	
	DDelay(200);
	return(temperature); //返回的是0123格式
}
