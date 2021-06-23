#include <stm32f10x_lib.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"
#include "lcd.h"	   
#include "rtc.h"
#include "wkup.h"
#include "adc.h"
#include "dma.h"
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "24l01.h"
//Mini STM32�����巶������19
//����ͨ��ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
																		   
//�շ�������һ����������,ͨ��������ȷ�����뷢��ģʽ,���ǽ���ģʽ		    									    		    	 		  
int main(void)
{	
	u8 key,mode;
	u16 t=0;			 
	u8 tmp_buf[33];

  	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);		//��ʱ��ʼ��
	uart_init(72,9600); //����1��ʼ��  	  
	LCD_Init();			//��ʼ��Һ��		  
	KEY_Init();	 	//������ʼ��
	LED_Init();         //LED��ʼ��
	NRF24L01_Init();    //��ʼ��NRF24L01 
 	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	LCD_ShowString(60,50,"Mini STM32");	
	LCD_ShowString(60,70,"NRF24L01 TEST");	
	LCD_ShowString(60,90,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,"2011/1/1");				 
	while(NRF24L01_Check())//��ⲻ��24L01
	{
		LCD_ShowString(60,130,"24L01 Check Failed!");
		delay_ms(500);
		LCD_ShowString(60,130,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	LCD_ShowString(60,130,"24L01 Ready!");
	LCD_ShowString(10,150,"KEY0:RX_Mode  KEY1:TX_Mode");
	while(1)//�ڸò���ȷ�������ĸ�ģʽ!
	{
		key=KEY_Scan();
		if(key==1)
		{
			mode=0;   
			break;
		}else if(key==2)
		{
			mode=1;
			break;
		}
		t++;
		if(t==100) //��˸��ʾ��ʾ��Ϣ
		{
			LCD_ShowString(10,150,"                          ");//�����ʾ  
		}	
		if(t==200)
		{
			t=0;
			LCD_ShowString(10,150,"KEY0:RX_Mode  KEY1:TX_Mode");
		}
		delay_ms(5);

	}
	LCD_Fill(10,150,240,166,WHITE);//����������ʾ		  
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
	if(mode==0)//RXģʽ
	{
		LCD_ShowString(60,150,"NRF24L01 RX_Mode");	
		LCD_ShowString(60,170,"Received DATA:");	
		RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
			{
				tmp_buf[32]=0;//�����ַ���������
				LCD_ShowString(0,190,tmp_buf); 
			}else delay_us(100);	   
			t++;
			if(t==10000)//��Լ1s�Ӹı�һ��״̬
			{
				t=0;
				LED0=!LED0;
			} 				    
		};	
	}else//TXģʽ
	{							    
		LCD_ShowString(60,150,"NRF24L01 TX_Mode");	
		TX_Mode();
		mode=' ';//�ӿո����ʼ  
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				LCD_ShowString(60,170,"Sended DATA:");	
				LCD_ShowString(0,190,tmp_buf); 
				key=mode;
				for(t=0;t<32;t++)
				{
					key++;
					if(key>('~'))key=' ';
					tmp_buf[t]=key;	
				}
				mode++; 
				if(mode>'~')mode=' ';  	  
				tmp_buf[32]=0;//���������		   
			}else
			{										   	
 				LCD_ShowString(60,170,"Send Failed "); 
				LCD_Fill(0,188,240,218,WHITE);//����������ʾ			   
			};
			LED0=!LED0;
			delay_ms(1500);				    
		};
	}     	  
}


				 






