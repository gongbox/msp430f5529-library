/***********************************************
创建人：陈志强
时间：2010.5.17
功能：MAX232头文件
************************************************/
#ifndef _MAX232_H_
#define _MAX232_H_
/*********************************************/
void Max232Int();
void Max232SendChar(unchar date);
void Max232SendAChar(unchar *string);
extern void MAX232SendDate();
/********************************************/
#endif