/*****************************************
创建：陈志强
时间：2010.6.12
功能：工程主函数(主机接收部分)
*****************************************/
#include"reg52.h"
#include"Allhead.h"
/************************主函数********************/
void main()
{
   //LCD1602Int();
   *(RevTempDate0+4)='\0';
   *(RevTempDate1+4)='\0';
   LcdInt(); //12864液晶初始化
   LcdClear(); //12864液晶清屏 
   Max232Int();//串口初始化
   NRF24L01Int();
   LcdWriteChStr(1,10,"通道0数据:");
   LcdWriteChStr(3,10,"通道1数据:");
   while(1)
    {
		NRFSetRXMode();//设置为接收模式
		GetDate();//开始接受数
	}
}