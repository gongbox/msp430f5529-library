/*****************************************
��������־ǿ
ʱ�䣺2010.6.12
���ܣ�NRF24L01��Ƶģ��C�ļ�(���䲿��)
���3��Ӧ���շ�ͨ��1��ַ
*****************************************/
#include"reg52.h"
#include"Allhead.h"
/*********************������***************************/
void main()
{
   unint ReadTempDate;//��ȡ�¶�
   char TxDate[4];
   NRF24L01Int();
   while(1)
    {
		 ReadTempDate=160;
		 TxDate[0]=ReadTempDate/100+'0';
		 TxDate[1]=ReadTempDate%100/10+'0';
		 TxDate[2]='.';
		 TxDate[3]=ReadTempDate%100%10+'0';	
		 NRFSetTxMode(TxDate);//�����¶�
		 while(CheckACK());	//����Ƿ������
	}

}