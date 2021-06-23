
#include "nokia_5110.h"
#include "english_6x8_pixel.h"
#include "write_chinese_string_pixel.h"
#include "picture.h"
#include "math.h"
#include "string.h"
/*-----------------------------------------------------------------------
LCD_init          : 5110LCD��ʼ��
-----------------------------------------------------------------------*/


void LCD_init(void)
  {
    
    //IO�ڳ�ʼ��
    P2SEL&=~(BIT6+BIT7);
    LCD_5110_DIR|=(0x01 << LCD_RST)+(0x01 << LCD_CE)+(0x01 << LCD_DC)+(0x01 << LCD_DIN)+(0x01<< LCD_CLK);
   
   
    //������λ�ź�
    LCD_5110_OUT &= ~(0x01 << LCD_RST);
    for(int i=0;i<200;i++);
    LCD_5110_OUT |= (0x01 << LCD_RST);
    
    //ʹ����չ��������LCDģʽ
    LCD_write_byte(0x21, 0);	
    LCD_write_byte(0xc0, 0);	// ����ƫ�õ�ѹ�Աȶ� 0xc8Ϊ��׼5V
    LCD_write_byte(0x06, 0);	// �¶�У��
    LCD_write_byte(0x13, 0);	// 1:48
    
    // ʹ�û�������
    LCD_write_byte(0x20, 0);	
    LCD_clear();	        // ����
    LCD_write_byte(0x0c, 0);	// �趨��ʾģʽ��������ʾ
    
      
  }

/*-----------------------------------------------------------------------
LCD_clear         : LCD��������
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);			
  }

/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��

���������X       ��0��83
          Y       ��0��5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// column
    LCD_write_byte(0x80 | X, 0);          	// row
  }

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�

���������c       ����ʾ���ַ���
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;

    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }

/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����

���������*s      ��Ӣ���ַ���ָ�룻
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
  }
/*-----------------------------------------------------------------------
LCD_write_chinese_string: ��LCD����ʾ����

���������X��Y    ����ʾ���ֵ���ʼX��Y���ꣻ
          ch_with �����ֵ���Ŀ��
          num     ����ʾ���ֵĸ�����  
          line    �����ֵ��������е���ʼ����
          row     ��������ʾ���м��
���ԣ�
	LCD_write_chi(0,0,12,7,0,0);
	LCD_write_chi(0,2,12,7,0,0);
	LCD_write_chi(0,4,12,7,0,0);	
-----------------------------------------------------------------------*/                        
void LCD_write_chinese_string(unsigned char X, unsigned char Y, 
                              unsigned char ch_with,unsigned char num,
                              unsigned char line,unsigned char row)
  {
    unsigned char i,n;
    
    LCD_set_XY(X,Y);                             //���ó�ʼλ��
    
    for (i=0;i<num;)
      {
      	for (n=0; n<ch_with*2; n++)              //дһ������
      	  { 
      	    if (n==ch_with)                      //д���ֵ��°벿��
      	      {
      	        if (i==0) LCD_set_XY(X,Y+1);
      	        else
      	           LCD_set_XY((X+(ch_with+row)*i),Y+1);
              }
      	    LCD_write_byte(write_chinese[line+i][n],1);
      	  }
      	i++;
      	LCD_set_XY((X+(ch_with+row)*i),Y);
      }
  }
/*-----------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD

���������data    ��д������ݣ�
          command ��д����/����ѡ��

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;
	LCD_5110_OUT &= ~(0x01 << LCD_CE);	// msp430
    
    if (command == 0)
     // PORTB &= ~LCD_DC ;	        // ��������
	//     LCD_DC = 0;
	  LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else
     // PORTB |= LCD_DC ;		        // ��������
    // LCD_DC = 1;
	 LCD_5110_OUT |= (0x01 << LCD_DC);
		for(i=0;i<8;i++)
		{
			if(dat&0x80)
				//SDIN = 1;
			   LCD_5110_OUT |= (0x01 << LCD_DIN);
			else
				//SDIN = 0;
			   LCD_5110_OUT &= ~(0x01 << LCD_DIN);
			//SCLK = 0;
			LCD_5110_OUT &= ~(0x01 << LCD_CLK);
			dat = dat << 1;
			//SCLK = 1;
			LCD_5110_OUT |= (0x01 << LCD_CLK);
		}
   // SPDR = data;			// �������ݵ�SPI�Ĵ���

    //while ((SPSR & 0x80) == 0);         // �ȴ����ݴ������
	
    //PORTB |= LCD_CE ;			// �ر�LCD
    // LCD_CE = 1;
	 LCD_5110_OUT |= (0x01 << LCD_CE);
  }

//��ʾ������
//x,y:�������  
//number:Ҫ��ʾ������
//decimal:Ҫ������С��λ
//��16����
static float _fround[6]={0.49,0.05,0.005,0.0005,0.00005,0.000005};

void ftoa(float n,unsigned char decimals,char *str)
{
	float scale;
	unsigned char i,d;
	if (n<0.0) {n=-n; *str++='-';};
	if (decimals>5) decimals=5;
	n=n+_fround[decimals];
	i=0;
	scale=1.0;
	while (n>=scale) {scale=scale*10.0; ++i;};
	if (i==0) *str++='0';
	else
	while (i--)
	{
    	scale=floor(0.5+scale/10.0);
      	d=(unsigned char) (n/scale);
      	*str++=d+'0';
      	n=n-scale*d;
    };
	if (decimals==0) {*str=0; return;};
	*str++='.';
	while (decimals--)
    {
    	n=n*10.0;
      	d=(unsigned char) n;
      	*str++=d+'0';
      	n=n-d;
	};
	*str=0;
}
void LCD_ShowFloat(unsigned char x,unsigned char y,float number,unsigned char decimal)
{
	char a[]="1234567890";
        ftoa(number,decimal,a);
	LCD_write_english_string(x,y,a);
        //LCD_ShowString(x,y,200,16,16,a);
}
/*-----------------------------------------------------------------------------
�������ƣ�LCD_draw_bmp_pixel
�������ܣ�λͼ���ƺ���
��ڲ�����X��Y    ��λͼ���Ƶ���ʼX��Y���ꣻ
          *map    ��λͼ�������ݣ�
          Pix_x   ��λͼ���أ�����
          Pix_y   ��λͼ���أ���
���ڲ������� 
�� ע�� 
------------------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y, const unsigned char *map, unsigned char Pix_x,unsigned char Pix_y)
{
    unsigned int i,n;
    unsigned char row;
    //����λͼ��ռ����						    
    if (Pix_y%8==0) 		//���Ϊλͼ��ռ����Ϊ����
        row=Pix_y/8;      
    else
        row=Pix_y/8+1;		//���Ϊλͼ��ռ������������
    
    LCD_set_XY(X,Y);
    for (n=0;n<row;n++)		//����
    {
        for(i=0;i<Pix_x;i++)
        {
          LCD_set_XY(X+i,Y+n);
          LCD_write_byte(map[i+n*Pix_x], 1);
         }                         
      }      
}


