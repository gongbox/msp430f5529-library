/***********************************************
创建人：陈志强
时间：2010.5.17
功能：DS18B20头文件
************************************************/
#ifndef _DS18B20_H_
#define _DS18B20_H_
/*****************************************/
extern void Reset();
extern void WriteChar(unchar dat);
extern unint ReadChar();
extern unint ReadTemperature();
/*****************************************/
#endif