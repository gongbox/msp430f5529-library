/*****************************************
��������־ǿ
ʱ�䣺2010.6.12
���ܣ�NRF24L01��Ƶģ��C�ļ�(���䲿��)
*****************************************/
#include"reg52.h"
#include"Allhead.h"
/*********************������***************************/
sbit Key=P1^0;
void main()
{
   unint ReadTempDate;//��ȡ�¶�
   char TxDate[4];
   NRF24L01Int();
   while(1)
    {
		 ReadTempDate=169;
		 TxDate[0]=ReadTempDate/100+'0';
		 TxDate[1]=ReadTempDate%100/10+'0';
		 TxDate[2]='.';
		 TxDate[3]=ReadTempDate%100%10+'0';	
		 NRFSetTxMode(TxDate);//�����¶�
		 while(CheckACK());	//����Ƿ������
	}

}