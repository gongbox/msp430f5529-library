/*****************************************
创建：陈志强
时间：2010.6.12
功能：LCD1602头文件
*****************************************/
#ifndef _LCD1602_H_
#define _LCD1602_H_
/*********************************************/
extern void LCDWriteCmd(unchar cmd);
void LCDWriteDate(unchar date);
void LcdShow(unchar x,unchar y,unchar *str);
void SetXY(unchar x,unchar y);
extern void LCDInt();
/********************************************/
#endif