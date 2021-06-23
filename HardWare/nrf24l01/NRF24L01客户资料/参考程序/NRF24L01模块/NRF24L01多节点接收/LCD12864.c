/********************************************************************
�����ˣ���־ǿ
ʱ�䣺2010.5.17
���ܣ�LCD12864Һ������
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
/*************LCDƬѡ����**************************************/
void LcdCS(unchar num)
{
  switch(num)
   {
      case 0:CS1=1;CS2=1;break;//ȫ��
	  case 1:CS1=1;CS2=0;break;//����
	  case 2:CS1=0;CS2=1;break;//����
	  case 3:CS1=0;CS2=0;break;//����
	  default:break;
   }
}
/********LCDд�����*****************************************/
void LcdWriteCmd(unchar num,unchar cmd)
{

   RS=0;
   RW=0;
   LcdCS(num);	//ѡ��д������
   EN=1;
   DPOT=cmd;
   EN=0;
}
/*********LCDд���ݺ���****************************************/
void LcdWriteDate(unchar num,unchar date)
{
  RS=1;
  RW=0;
  LcdCS(num);//ѡ����Ļд������ 
  EN=1;
  DPOT=date;
  EN=0;
}												 
/**********LCD����Ļ����****************************************/
unchar LcdReadDate(unchar num)
{
  unchar date;
  DPOT=0xff;   //�����ݿ���1����ܶ��˿�
  RS=1;
  RW=1;
  LcdCS(num);//ѡ����Ļ��������
  EN=1;  
  date=DPOT;  
  EN=0;
  return(date);
}									  
/**********X���ú���*************************************************/
void PageSet(unchar num,unchar page)
{
   LcdWriteCmd(num,page|0xb8);
}
/**********Y���ú���************************************************/
void YSet(unchar num,unchar y)
{
   LcdWriteCmd(num,y|0x40);
}
/************ָ����λ��д������***********************************/
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
/************ָ��λ�ö�������****************************************/
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
/************�����������********************************************/
void LcdWriteDot(unchar x,unchar y,unchar attr)
{	
   	unchar yaddr,tempdate1,tempdate2;
	yaddr=6-(y>>3);	 //���������ҳ
	tempdate1=LcdReadFromSet(yaddr,x);//��ָ����x,y����һ���ֽڵ�����		
	tempdate2=0x80>>((y&0x07)-1);	 //�����ҳ�е�	
	if(attr)				
	   tempdate1|=tempdate2;   //����
	else
	   tempdate1&=~tempdate2;  //����
    LcdWriteToSet(yaddr,x,tempdate1);//ָ��λ�û���
}
/*******ָ��λ��д��12������*******************************************/
void LcdWriteCh12(unchar x,unchar y,unchar *string)
{
	  unchar i;
	  for(i=0;i<12;i++)
	    LcdWriteToSet(x,y++,string[i]);
		y-=12;
	  for(i=12;i<24;i++)
	    LcdWriteToSet(x+1,y++,string[i]);
} 
/**********ָ��λ��д��һ��5*8ASCII��ģ*******************/
/*void LcdWriteAsc58(unchar x,unchar y,unchar string)
{
   unchar i;
   for(i=0;i<5;i++)  //ASCII���ϲ���
      LcdWriteToSet(x,y++,nAsciiDot58[string-32][i]);
}  */
/**********ָ��λ��д��һ��8*16ASCII��ģ*******************/
void LcdWriteAsc816(unchar x,unchar y,unchar string)
{
   unchar i;
   for(i=0;i<8;i++)  //ASCII���ϲ���
      LcdWriteToSet(x,y++,nAsciiDot816[string-32][i]);
	  y-=8;//���س�ʼ��y��ַ
   for(i=8;i<16;i++)  //ASCII���²���
	  LcdWriteToSet(x+1,y++,nAsciiDot816[string-32][i]);
	
}
/*********ָ��λ��д��һ�����ֻ���ASCII��********************************/
//������������
void LcdWriteChStr(unchar x,unchar y,unchar *string)
{
  unchar ch1,ch2,i,j=0,length;
  for(length=0;string[length]!=0;length++);
  do
   {
      ch1=string[j];
      ch2=string[j+1];
	 if(ch1<0x80)//С��0x80ΪASCII��
	  {
		LcdWriteAsc816(x,y,ch1); 
		j++;
		y+=8;  //������������
	  }
	else
	{
      for(i=0;i<sizeof(GB_12)/sizeof(GB_12[0]);i++)	   //������������
	   {
	     if((ch1==GB_12[i].Index[0])&&(ch2==GB_12[i].Index[1]))
		  break;
	   }
	  LcdWriteCh12(x,y,GB_12[i].Msk);
	  j+=2;
	  y+=12;//������������
	}
   }while(j<length); 
}
/************ָ��λ��ͼƬ��ʾ����*******************************************/
/*void LcdShowPicture(unchar x,unchar y,unchar high,unchar wide,unchar *img)
{  //ҳ/Y��ַ/ͼƬ��/ͼƬ��/ͼƬ����
   //����ʾͼƬʱҪ�ر�С�ģ�ע��ͼƬ����ʱ���٣�
   //���õ�Y��page������Ҫ��������ʾ�⸱ͼƬ����߲����� 
   unchar i,j;
   for(i=0;i<(high>>3);i++)   //�ж���������ٸ�ҳ������8������Ϊһҳ
		for(j=0;j<(wide-y);j++)	 //����Y��ַʱҪ����ǰ��ո񣬾�������Y���һҳ��������ʾ���ٸ��ֽڣ�					 
		  LcdWriteToSet(x+i,y+j,img[i*wide+j]);

}
/*********************LCD����******************************************/
void LcdClear()
{
  unchar x,y;
  for(x=0;x<8;x++)
   {
	 for(y=0;y<128;y++)
	  LcdWriteToSet(x,y,0x00);
   }
}
/************LCD��ʼ������*********************************************/
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
