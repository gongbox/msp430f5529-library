  #ifndef  __12864SRR_H__
  #define  __12864SRR_H__

  #define uint unsigned int
  #define uchar unsigned char

 /******************在此修改端口***********************
  B14 为 传输 数据  输入
  B15 为 串行 时钟  输入
  注：在init_lcd(void); 还有涉及 I/O 的初始化程序
  ********************************************************/
  #define LCD_SID1   gpio_bit_set(P8,1,1)
  #define LCD_SID0   gpio_bit_set(P8,1,0)   		 //WR 做数据传输使用sid   
  #define LCD_SCLK1  gpio_bit_set(P8,2,1)
  #define LCD_SCLK0  gpio_bit_set(P8,2,0)   		 //EN 当做串行时钟使用sclk

  #define comm  0
  #define dat   1

  /******************12864使用的函数***********************
  从第 y 行的第 x 个开始显示
  X  为列 取值范围为 ：   0---7
  y  为行 取值范围为：    0---3
  ********************************************************/ 
  void LCD_GPIO_Config(void);                                             
  void wr_lcd(uchar dat_comm,uchar content);  	//写数据/地址或命令  dat_comm为数据或指令 content数据
  void init_lcd(void);                        	//初始化
  void clrram(void);    			//清屏
  void clr_lcd(uchar x,uchar y0,uchar length); 	//清除lcd任意位置字符 从y0行的第x个位置（1、16），清除length个字符
  void lcd_xy(uchar x,uchar y0);		//定位光标
  void wr_data(uchar x,uchar y0,uchar data );	//写入单个字符如                       wr_data(0,1,'d' );
  void wr_string(uchar x,uchar y0,uchar *p);	//写入字符串                           wr_string(0,0,"I love you");
  void wr_int(uchar x,uchar y0,uint num);       //写入整形数据,num范围：0-65535        wr_int(3,1,56); 
  void wr_float(uchar x,uchar y0,float num,int n);   //写入浮点数，n为显示的小数点位数 wr_float(0,2,3.7,2);
  
  #endif


