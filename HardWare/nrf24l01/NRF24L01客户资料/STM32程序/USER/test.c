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
//Mini STM32开发板范例代码19
//无线通信实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
																		   
//收发都做在一个函数里面,通过按键来确定进入发送模式,还是接收模式		    									    		    	 		  
int main(void)
{	
	u8 key,mode;
	u16 t=0;			 
	u8 tmp_buf[33];

  	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,9600); //串口1初始化  	  
	LCD_Init();			//初始化液晶		  
	KEY_Init();	 	//按键初始化
	LED_Init();         //LED初始化
	NRF24L01_Init();    //初始化NRF24L01 
 	POINT_COLOR=RED;//设置字体为红色	   
	LCD_ShowString(60,50,"Mini STM32");	
	LCD_ShowString(60,70,"NRF24L01 TEST");	
	LCD_ShowString(60,90,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,"2011/1/1");				 
	while(NRF24L01_Check())//检测不到24L01
	{
		LCD_ShowString(60,130,"24L01 Check Failed!");
		delay_ms(500);
		LCD_ShowString(60,130,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	LCD_ShowString(60,130,"24L01 Ready!");
	LCD_ShowString(10,150,"KEY0:RX_Mode  KEY1:TX_Mode");
	while(1)//在该部分确定进入哪个模式!
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
		if(t==100) //闪烁显示提示信息
		{
			LCD_ShowString(10,150,"                          ");//清空显示  
		}	
		if(t==200)
		{
			t=0;
			LCD_ShowString(10,150,"KEY0:RX_Mode  KEY1:TX_Mode");
		}
		delay_ms(5);

	}
	LCD_Fill(10,150,240,166,WHITE);//清空上面的显示		  
 	POINT_COLOR=BLUE;//设置字体为蓝色	   
	if(mode==0)//RX模式
	{
		LCD_ShowString(60,150,"NRF24L01 RX_Mode");	
		LCD_ShowString(60,170,"Received DATA:");	
		RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
			{
				tmp_buf[32]=0;//加入字符串结束符
				LCD_ShowString(0,190,tmp_buf); 
			}else delay_us(100);	   
			t++;
			if(t==10000)//大约1s钟改变一次状态
			{
				t=0;
				LED0=!LED0;
			} 				    
		};	
	}else//TX模式
	{							    
		LCD_ShowString(60,150,"NRF24L01 TX_Mode");	
		TX_Mode();
		mode=' ';//从空格键开始  
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
				tmp_buf[32]=0;//加入结束符		   
			}else
			{										   	
 				LCD_ShowString(60,170,"Send Failed "); 
				LCD_Fill(0,188,240,218,WHITE);//清空上面的显示			   
			};
			LED0=!LED0;
			delay_ms(1500);				    
		};
	}     	  
}


				 






