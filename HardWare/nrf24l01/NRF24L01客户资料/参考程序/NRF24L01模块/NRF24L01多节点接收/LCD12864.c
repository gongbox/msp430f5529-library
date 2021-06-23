/********************************************************************
创建人：陈志强
时间：2010.5.17
功能：LCD12864液晶函数
******************************************************************/
#include"reg52.h"
#include"code.h"
#include"Allhead.h"
#include"math.h"
#define DPOT P2
sbit RS=P0^0;
sbit RW=P0^1;
sbit EN=P0^2;
sbit CS1=P0^3;
sbit CS2=P0^4;
sbit RES=P0^5;
/*************LCD片选函数**************************************/
void LcdCS(unchar num)
{
  switch(num)
   {
      case 0:CS1=1;CS2=1;break;//全屏
	  case 1:CS1=1;CS2=0;break;//左屏
	  case 2:CS1=0;CS2=1;break;//右屏
	  case 3:CS1=0;CS2=0;break;//关屏
	  default:break;
   }
}
/********LCD写命令函数*****************************************/
void LcdWriteCmd(unchar num,unchar cmd)
{

   RS=0;
   RW=0;
   LcdCS(num);	//选屏写入命令
   EN=1;
   DPOT=cmd;
   EN=0;
}
/*********LCD写数据函数****************************************/
void LcdWriteDate(unchar num,unchar date)
{
  RS=1;
  RW=0;
  LcdCS(num);//选择屏幕写入数据 
  EN=1;
  DPOT=date;
  EN=0;
}												 
/**********LCD读屏幕函数****************************************/
unchar LcdReadDate(unchar num)
{
  unchar date;
  DPOT=0xff;   //让数据口置1后才能读端口
  RS=1;
  RW=1;
  LcdCS(num);//选择屏幕读入数据
  EN=1;  
  date=DPOT;  
  EN=0;
  return(date);
}									  
/**********X设置函数*************************************************/
void PageSet(unchar num,unchar page)
{
   LcdWriteCmd(num,page|0xb8);
}
/**********Y设置函数************************************************/
void YSet(unchar num,unchar y)
{
   LcdWriteCmd(num,y|0x40);
}
/************指定的位置写入数据***********************************/
void LcdWriteToSet(unchar x,unchar y,unchar date)
{
   if(y<64)
    {
	   PageSet(1,x);
	   YSet(1,y);
	   LcdWriteDate(1,date);
	}
   else
    {
	   PageSet(2,x);
       YSet(2,y-64);
	   LcdWriteDate(2,date);
	}
}
/************指定位置读出数据****************************************/
unchar LcdReadFromSet(unchar x,unchar y)
{
  unchar date;
  if(y<64)
    {
	   PageSet(1,x);
	   YSet(1,y);
	   date=LcdReadDate(1);	  
	   date=LcdReadDate(1);
	   return(date);
	}
  else
    {
	  PageSet(2,x);
	  YSet(2,y-64);
	  date=LcdReadDate(2);
	  date=LcdReadDate(2);
	  return(date);
	}
}
/************画点基本函数********************************************/
void LcdWriteDot(unchar x,unchar y,unchar attr)
{	
   	unchar yaddr,tempdate1,tempdate2;
	yaddr=6-(y>>3);	 //计算出所在页
	tempdate1=LcdReadFromSet(yaddr,x);//在指定的x,y读出一个字节的数据		
	tempdate2=0x80>>((y&0x07)-1);	 //计算出页中点	
	if(attr)				
	   tempdate1|=tempdate2;   //画点
	else
	   tempdate1&=~tempdate2;  //消点
    LcdWriteToSet(yaddr,x,tempdate1);//指定位置画点
}
/*******指定位置写入12点阵汉字*******************************************/
void LcdWriteCh12(unchar x,unchar y,unchar *string)
{
	  unchar i;
	  for(i=0;i<12;i++)
	    LcdWriteToSet(x,y++,string[i]);
		y-=12;
	  for(i=12;i<24;i++)
	    LcdWriteToSet(x+1,y++,string[i]);
} 
/**********指定位置写入一个5*8ASCII字模*******************/
/*void LcdWriteAsc58(unchar x,unchar y,unchar string)
{
   unchar i;
   for(i=0;i<5;i++)  //ASCII码上部分
      LcdWriteToSet(x,y++,nAsciiDot58[string-32][i]);
}  */
/**********指定位置写入一个8*16ASCII字模*******************/
void LcdWriteAsc816(unchar x,unchar y,unchar string)
{
   unchar i;
   for(i=0;i<8;i++)  //ASCII码上部分
      LcdWriteToSet(x,y++,nAsciiDot816[string-32][i]);
	  y-=8;//返回初始的y地址
   for(i=8;i<16;i++)  //ASCII码下部分
	  LcdWriteToSet(x+1,y++,nAsciiDot816[string-32][i]);
	
}
/*********指定位置写入一串汉字或者ASCII码********************************/
//采用内码搜索
void LcdWriteChStr(unchar x,unchar y,unchar *string)
{
  unchar ch1,ch2,i,j=0,length;
  for(length=0;string[length]!=0;length++);
  do
   {
      ch1=string[j];
      ch2=string[j+1];
	 if(ch1<0x80)//小于0x80为ASCII码
	  {
		LcdWriteAsc816(x,y,ch1); 
		j++;
		y+=8;  //看字体来设置
	  }
	else
	{
      for(i=0;i<sizeof(GB_12)/sizeof(GB_12[0]);i++)	   //汉字内码搜索
	   {
	     if((ch1==GB_12[i].Index[0])&&(ch2==GB_12[i].Index[1]))
		  break;
	   }
	  LcdWriteCh12(x,y,GB_12[i].Msk);
	  j+=2;
	  y+=12;//看字体来设置
	}
   }while(j<length); 
}
/************指定位置图片显示函数*******************************************/
/*void LcdShowPicture(unchar x,unchar y,unchar high,unchar wide,unchar *img)
{  //页/Y地址/图片高/图片宽/图片编码
   //在显示图片时要特别小心，注意图片长宽时多少！
   //设置的Y和page参数不要超过能显示这副图片的最高参数！ 
   unchar i,j;
   for(i=0;i<(high>>3);i++)   //判断这幅画多少个页，竖向8个像素为一页
		for(j=0;j<(wide-y);j++)	 //设置Y地址时要减掉前面空格，就是设置Y后第一页还可以显示多少个字节！					 
		  LcdWriteToSet(x+i,y+j,img[i*wide+j]);

}
/*********************LCD清屏******************************************/
void LcdClear()
{
  unchar x,y;
  for(x=0;x<8;x++)
   {
	 for(y=0;y<128;y++)
	  LcdWriteToSet(x,y,0x00);
   }
}
/************LCD初始化函数*********************************************/
void LcdInt()
{
  RES=0;
  RES=1;
  RW=0;
  RS=0;
  EN=0;
  LcdCS(3);
  LcdWriteCmd(1,0x3f); 
  LcdWriteCmd(1,0xc0);
  LcdCS(3);
  LcdWriteCmd(2,0x3f);
  LcdWriteCmd(2,0xc0);
}  
