/**********************************************************
�����ˣ���־ǿ
ʱ�䣺2009.9.10
���ܣ�DS18B20����							  
***********************************************************/
#include"reg52.h"
#include"allhead.h"
sbit DQ=P1^0;//���ݶ�
unint tempL,tempH;
/********************************************************* 
	DS18B20��λ����
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
	DS18B20дһ���ֽں���
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
	 DS18B20��һ���ֽں���
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
	DS18B20���¶ȳ���
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
	return(temperature); //���ص���0123��ʽ
}
