  #ifndef  __12864SRR_H__
  #define  __12864SRR_H__

  #define uint unsigned int
  #define uchar unsigned char

 /******************�ڴ��޸Ķ˿�***********************
  B14 Ϊ ���� ����  ����
  B15 Ϊ ���� ʱ��  ����
  ע����init_lcd(void); �����漰 I/O �ĳ�ʼ������
  ********************************************************/
  #define LCD_SID1   gpio_bit_set(P8,1,1)
  #define LCD_SID0   gpio_bit_set(P8,1,0)   		 //WR �����ݴ���ʹ��sid   
  #define LCD_SCLK1  gpio_bit_set(P8,2,1)
  #define LCD_SCLK0  gpio_bit_set(P8,2,0)   		 //EN ��������ʱ��ʹ��sclk

  #define comm  0
  #define dat   1

  /******************12864ʹ�õĺ���***********************
  �ӵ� y �еĵ� x ����ʼ��ʾ
  X  Ϊ�� ȡֵ��ΧΪ ��   0---7
  y  Ϊ�� ȡֵ��ΧΪ��    0---3
  ********************************************************/ 
  void LCD_GPIO_Config(void);                                             
  void wr_lcd(uchar dat_comm,uchar content);  	//д����/��ַ������  dat_commΪ���ݻ�ָ�� content����
  void init_lcd(void);                        	//��ʼ��
  void clrram(void);    			//����
  void clr_lcd(uchar x,uchar y0,uchar length); 	//���lcd����λ���ַ� ��y0�еĵ�x��λ�ã�1��16�������length���ַ�
  void lcd_xy(uchar x,uchar y0);		//��λ���
  void wr_data(uchar x,uchar y0,uchar data );	//д�뵥���ַ���                       wr_data(0,1,'d' );
  void wr_string(uchar x,uchar y0,uchar *p);	//д���ַ���                           wr_string(0,0,"I love you");
  void wr_int(uchar x,uchar y0,uint num);       //д����������,num��Χ��0-65535        wr_int(3,1,56); 
  void wr_float(uchar x,uchar y0,float num,int n);   //д�븡������nΪ��ʾ��С����λ�� wr_float(0,2,3.7,2);
  
  #endif


