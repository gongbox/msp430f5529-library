/********************************************************************
�����ˣ���־ǿ
ʱ�䣺2010.5.17
���ܣ�MAX232����
*********************************************************************/
#include"reg52.h"
#include"allhead.h"	
unchar RevFlags;
/*************************���ڳ�ʼ������**************/
void Max232Int()
{
	PCON=0x00;   //SMOD=0
	TMOD=0x20;  //����T1Ϊ������ʽ2
	TH1=0xfd;  //9600
	TL1=0xfd;
	SM0=0;
	SM1=1;//��ʽ1,8λ
	EA=1;
	ES=1;
	TR1=1;//��ʱ��1����
	REN=1;//�������
}
/*************************���ڷ���һ���ֽں���********************/
void Max232SendChar(unchar date)
{
   ES=0;
   SBUF=date;
   while(!TI);
     TI=0;
     ES=1;
}
/**********************����һ���ַ���******************************/
void Max232SendAChar(unchar *string)
{
  while(*string!='\0')
   {
      Max232SendChar(*string);
	  string++;
   }
}
/**********************�����жϽ��պ���****************************/
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
/*********************����NRF24L01�ɼ���������********************/
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
	//Max232SendChar('E');//���ͽ�����־
}