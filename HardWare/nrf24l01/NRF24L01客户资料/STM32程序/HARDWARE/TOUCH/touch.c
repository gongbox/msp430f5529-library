#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/6/13 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//����˵��
//V1.1 20110730
//1,��Pen_Holder����touchtype����,���ڱ�Ǵ�������.ʹ֮��֧���κδ���.
//2,����Get_Adjdata��SAVE_Adjdata��������.
//3,�����˴���У׼�������,�����жϴ����û�.							    
////////////////////////////////////////////////////////////////////////////////// 	  
Pen_Holder Pen_Point;//�����ʵ��
//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

//SPIд����
//��7843д��1byte����   
void ADS_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0;//��������Ч	   	 
		TCLK=1;      
	} 			    
} 		 
//SPI������ 
//��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ	   
u16 ADS_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;//������ʱ�� 	 
	TCS=0; //ѡ��ADS7843	 
	ADS_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TCLK=1;//��1��ʱ�ӣ����BUSY   	    
	TCLK=0; 	 
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		TCLK=0;//�½�����Ч  	    	   
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   //ֻ�и�12λ��Ч.
	TCS=1;//�ͷ�ADS7843	 
	return(Num);   
}
//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//���˲��������ȡ
//��Сֵ��������100.
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}	
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 50 //��Χ 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
} 
//��ȡһ������ֵ	
//������ȡһ��,֪��PEN�ɿ��ŷ���!					   
u8 Read_TP_Once(void)
{
	u8 t=0;	    
	Pen_Int_Set(0);//�ر��ж�
	Pen_Point.Key_Sta=Key_Up;
	Read_ADS2(&Pen_Point.X,&Pen_Point.Y);
	while(PEN==0&&t<=250)
	{
		t++;
		delay_ms(10);
	};
	Pen_Int_Set(1);//�����ж�		 
	if(t>=250)return 0;//����2.5s ��Ϊ��Ч
	else return 1;	
}

//////////////////////////////////////////////////
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
void Drow_Touch_Point(u8 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);//����
	LCD_DrawLine(x,y-12,x,y+13);//����
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);//������Ȧ
}	  
//��һ�����
//2*2�ĵ�			   
void Draw_Big_Point(u8 x,u16 y)
{	    
	LCD_DrawPoint(x,y);//���ĵ� 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}
//////////////////////////////////////////////////

//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
void Convert_Pos(void)
{		 	  
	if(Read_ADS2(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}	   
//�ж�,��⵽PEN�ŵ�һ���½���.
//��λPen_Point.Key_StaΪ����״̬
//�ж���0���ϵ��жϼ��
void EXTI1_IRQHandler(void)
{ 		   			 
	Pen_Point.Key_Sta=Key_Down;//��������  		  				 
	EXTI->PR=1<<1;  //���LINE1�ϵ��жϱ�־λ 
} 
//PEN�ж�����	 
void Pen_Int_Set(u8 en)
{
	if(en)EXTI->IMR|=1<<1;   //����line1�ϵ��ж�	  	
	else EXTI->IMR&=~(1<<1); //�ر�line1�ϵ��ж�	   
}	  
//////////////////////////////////////////////////////////////////////////
//�˲����漰��ʹ���ⲿEEPROM,���û���ⲿEEPROM,���δ˲��ּ���
#ifdef ADJ_SAVE_ENABLE
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//����У׼����										    
void Save_Adjdata(void)
{
	s32 temp;			 
	//����У�����!		   							  
	temp=Pen_Point.xfac*100000000;//����xУ������      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//����yУ������    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//����xƫ����
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//����yƫ����
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
	//���津������
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,Pen_Point.touchtype);	
	temp=0X0A;//���У׼����
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp);			 
}
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
u8 Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//��ȡ�����,���Ƿ�У׼���� 		 
	if(tempfac==0X0A)//�������Ѿ�У׼����			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//�õ�yУ׼����
	    //�õ�xƫ����
		Pen_Point.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	    //�õ�yƫ����
		Pen_Point.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
 		Pen_Point.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ�������ͱ��
		if(Pen_Point.touchtype)//X,Y��������Ļ�෴
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y��������Ļ��ͬ
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
}
#endif	
//
void ADJ_INFO_SHOW(u8*str)
{
	LCD_ShowString(40,40,"x1:       y1:       ");
	LCD_ShowString(40,60,"x2:       y2:       ");
	LCD_ShowString(40,80,"x3:       y3:       ");
	LCD_ShowString(40,100,"x4:       y4:       ");
 	LCD_ShowString(40,100,"x4:       y4:       ");
 	LCD_ShowString(40,120,str);					   
}
	 
//������У׼����
//�õ��ĸ�У׼����
void Touch_Adjust(void)
{								 
	signed short pos_temp[4][2];//���껺��ֵ
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	float fac; 	   
	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//����   
	POINT_COLOR=RED;//��ɫ 
	LCD_Clear(WHITE);//���� 
	Drow_Touch_Point(20,20);//����1 
	Pen_Point.Key_Sta=Key_Up;//���������ź� 
	Pen_Point.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	 
	while(1)
	{
		if(Pen_Point.Key_Sta==Key_Down)//����������
		{
			if(Read_TP_Once())//�õ����ΰ���ֵ
			{  								   
				pos_temp[cnt][0]=Pen_Point.X;
				pos_temp[cnt][1]=Pen_Point.Y;
				cnt++;
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);//���� 
					Drow_Touch_Point(220,20);//����2
					break;
				case 2:
					LCD_Clear(WHITE);//���� 
					Drow_Touch_Point(20,300);//����3
					break;
				case 3:
					LCD_Clear(WHITE);//���� 
					Drow_Touch_Point(220,300);//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		    //�Ա����
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,2�ľ���
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�3,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("ver fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//��ʾ��ֵ
						//����100����ʾ
						LCD_ShowNum(40,140,fac*100,3,16);//��ʾ��ֵ,����ֵ������95~105��Χ֮��.
						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,3�ľ���
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("hor fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//��ʾ��ֵ
						//����100����ʾ
						LCD_ShowNum(40,140,fac*100,3,16);//��ʾ��ֵ,����ֵ������95~105��Χ֮��.
 						continue;
					}//��ȷ��
								   
					//�Խ������
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,4�ľ���
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,3�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(20,20);
						ADJ_INFO_SHOW("dia fac is:");   
						LCD_ShowNum(40+24,40,pos_temp[0][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,40,pos_temp[0][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,60,pos_temp[1][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,60,pos_temp[1][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,80,pos_temp[2][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,80,pos_temp[2][1],4,16);	//��ʾ��ֵ
						LCD_ShowNum(40+24,100,pos_temp[3][0],4,16);		//��ʾ��ֵ
						LCD_ShowNum(40+24+80,100,pos_temp[3][1],4,16);	//��ʾ��ֵ
						//����100����ʾ
						LCD_ShowNum(40,140,fac*100,3,16);//��ʾ��ֵ,����ֵ������95~105��Χ֮��.
						continue;
					}//��ȷ��
					//������
					Pen_Point.xfac=(float)200/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
						  
					Pen_Point.yfac=(float)280/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff  

					if(abs(Pen_Point.xfac)>2||abs(Pen_Point.yfac)>2)//������Ԥ����෴��.
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(20,20);
						LCD_ShowString(35,110,"TP Need readjust!");
						Pen_Point.touchtype=!Pen_Point.touchtype;//�޸Ĵ�������.
						if(Pen_Point.touchtype)//X,Y��������Ļ�෴
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y��������Ļ��ͬ
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}
						delay_ms(500);
						continue;
					}
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//����
					LCD_ShowString(35,110,"Touch Screen Adjust OK!");//У�����
					delay_ms(500);
					LCD_Clear(WHITE);//����   
					return;//У�����				 
			}
		}
	} 
}		    
//�ⲿ�жϳ�ʼ������
void Touch_Init(void)
{			    		   
	//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
	//��������֮ǰ,����ʹ��ʱ��.����ʵ���������������
	RCC->APB2ENR|=1<<4;    //PCʱ��ʹ��	   
	RCC->APB2ENR|=1<<0;    //��������ʱ��							  
	GPIOC->CRL&=0XFFFF0000;//PC0~3
	GPIOC->CRL|=0X00003883; 
	GPIOC->CRH&=0XFF0FFFFF;//PC13
	GPIOC->CRH|=0X00300000;//PC13������� 
	GPIOC->ODR|=0X200f;    //PC0~3 13 ȫ������	   
 	Read_ADS(&Pen_Point.X,&Pen_Point.Y);//��һ�ζ�ȡ��ʼ��			 
	MY_NVIC_Init(2,0,EXTI1_IRQChannel,2);	 
	RCC->APB2ENR|=0x01;    //ʹ��io����ʱ��	    
    AFIO->EXTICR[0]|=0X0020; //EXTI13ӳ�䵽PC1   
	EXTI->IMR|=1<<1;        //����line1�ϵ��ж�
	EXTI->EMR|=1<<1;        //������line1�ϵ��¼�
	EXTI->FTSR|=1<<1;       //line1���¼��½��ش���	
#ifdef ADJ_SAVE_ENABLE	  
	AT24CXX_Init();//��ʼ��24CXX
	if(Get_Adjdata())return;//�Ѿ�У׼
	else			   //δУ׼?
	{ 										    
		LCD_Clear(WHITE);//����
	    Touch_Adjust();  //��ĻУ׼ 
		Save_Adjdata();	 
	}			
	Get_Adjdata();
#else
	LCD_Clear(WHITE);//����
    Touch_Adjust();  //��ĻУ׼,���Զ�����			   
#endif												 
//	printf("Pen_Point.xfac:%f\n",Pen_Point.xfac);
//	printf("Pen_Point.yfac:%f\n",Pen_Point.yfac);
//	printf("Pen_Point.xoff:%d\n",Pen_Point.xoff);
//	printf("Pen_Point.yoff:%d\n",Pen_Point.yoff);
}

