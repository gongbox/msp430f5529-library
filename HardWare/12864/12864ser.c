#include "12864ser.h"
//#include "delay.h"
#include "gpio.h"
void delay_us(int us)
{
  us*=25;
  while(us--);
}
void delay_ms(int ms)
{
  ms*=25;
  while(ms--)
  {
      for(int i=200;i>0;i--);
  }
}
 void LCD_GPIO_Config(void)
{
	/*GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);	*/
  
        gpio_init(P8,1,GPO);
        gpio_init(P8,2,GPO);
}

/**************************LCD��ʼ��***************************/
void init_lcd(void)
{
   //	���� SID->B4   SCLK->B5
	LCD_GPIO_Config();
    
    clrram();         //�����ʾ ȫ�����
    delay_ms(1);
    wr_lcd(comm,0x06);//������ƣ�AC�Զ���һ��������ʾ���ƶ�
    wr_lcd(comm,0x0c);//������ʾON,�����ʾOFF,���λ�ò�������˸
    wr_lcd(comm,0x01);//�����ʾ
}

/*************************clrram()******************************/
void clrram()
{
    wr_lcd(comm,0x30);//8-BIT���ƽӿڣ�����ָ�����
    wr_lcd(comm,0x01);//�����ʾ
    delay_us(180);
}

/*-----------���lcd����λ���ַ�------------------*/
/*--------positionΪ��һλ�õ���ʼ��ַ------------*/
/*--lengthΪ���ȣ�ȡֵΪ1-64---/��ֻ��һ�����Ϊ16*/
void clr_lcd(uchar x,uchar y0,uchar length)
{
    uint i;
    lcd_xy(x,y0);          //��λ���
    for(i=0;i<length;i++)
    {
        wr_lcd (dat,0x20);//���ո�
    }
}

/*********************д���������*********************
   dat_comm Ϊ comm �� 0 ʱд����
   dat_comm Ϊ dat  �� 1 ʱд����
******************************************************/
void wr_lcd(uchar dat_comm,uchar content)
{			
    uchar a,i,j;
    delay_us(50);
    a=content;             //Ҫд������
    LCD_SCLK0;             //en=0;
    LCD_SID1;              //wr=1
    for(i=0;i<5;i++)       //����ʱ��
    {
       LCD_SCLK1;
       LCD_SCLK0;
    }
    LCD_SID0;              //wr=0
    LCD_SCLK1;             //en=1
    LCD_SCLK0;             //en=0
    if(dat_comm) LCD_SID1;
    else         LCD_SID0;

    LCD_SCLK1;
    LCD_SCLK0;
    LCD_SID0;
    LCD_SCLK1;
    LCD_SCLK0;

    for(j=0;j<2;j++)       //8��ѭ��ддһ�ֽ�����
    {
       for(i=0;i<4;i++)
       {
          if(a&0x80)
              LCD_SID1;
          else
              LCD_SID0;
          a=a<<1;
          LCD_SCLK1;
          LCD_SCLK0;
       }
       LCD_SID0;
       for(i=0;i<4;i++)    //ʱ��
       {
          LCD_SCLK1;
          LCD_SCLK0;
       }
    }
}

/********************�����м���ã�д��ַ***********************/
void lcd_xy(uchar x,uchar y0)
{
    uchar pos=0;
    switch(y0)             //y0Ϊ�к�
    {
        case 0: pos=0x80+x;break;
        case 1: pos=0x90+x;break;
        case 2: pos=0x88+x;break;
        case 3: pos=0x98+x;break;
        default:break;
    }
    wr_lcd(comm,pos);
}

/***********************��ʾ�����ַ�***************************/
void wr_data(uchar x,uchar y0,uchar data )
{
    lcd_xy(x,y0);
    wr_lcd(dat,data);      //д���������  
}

/************************д�ַ���******************************/
//Xȡֵ0-7��Yȡֵ0-3
void wr_string(uchar x,uchar y0,uchar *p) 
{
     uchar cnt=0;          //���ݹ�������ָ�� ��cntΪƫ������Ȼ��һ���ַ�һ���ַ���д�롣
     lcd_xy(x,y0);
     while(*(p+cnt)!='\0')
     {
          wr_lcd(dat,*(p+cnt));//д����
          cnt++;
     }
}
/***********************������������ʾ************************
numΪ������С����nΪ��ʾ��С����λ��
�������������㷨
��n=0ʱ������ʾС����
��С����������ʾ10λ
****************************************************************/
void wr_float(uchar x,uchar y0,float num,int n)
{
    uint i=1,j=0,k=0;
    uchar array[10]={'0'};
    float m=1;
    unsigned long int t;
    for(j=0;j<n;j++)                     
       {m*=10.0;}
    t=(unsigned long int)(num*m);              //��С��numת��Ϊ����         
    while(num>=10.0)                  //������λ��   
    {   
        i++;   
        num/=10.0;   
    }
    for(j=i+n+1;j>0;j--)              //�������ֵ������
    { 
      if(j!=i+1)
      {
          array[j-1]=t%10+48;
          t/=10;
      }
      else if(n==0)                   //��n=0ʱ������ʾС����
             array[i]='\0';
           else   array[i]=46;        //��n!=0ʱ��array[i]���С����  С�����ASCII��Ϊ46
    }
    array[i+n+1]='\0';                //���һλ���������־
    wr_string(x,y0,array);
    for(k=0;k<10;k++)                 //�����������
        array[k]=48;
}

/************************����������ʾ*************************/
void wr_int(uchar x,uchar y0,uint num) //num��Χ��0-65535
{
    uint i=1,j=0,k=0;
    float temp;
    uchar array[5]={'0'};
    temp=(float)num;
    while(temp>=10.0)          //�ж���������λ��        
    {   
        i++;                   //iΪ��������λ��
        temp/=10.0;   
    } 
    array[i]='\0';
    for(j=i;j>0;j--)           //����ȡֵ�������� 
    {   
       array[j-1]=num%10+48;   //ȡ���λ��ֵ       
       num/=10;                 
    }
    wr_string(x,y0,array);
    for(k=0;k<5;k++)           //�����������
        array[k]='0';
}




