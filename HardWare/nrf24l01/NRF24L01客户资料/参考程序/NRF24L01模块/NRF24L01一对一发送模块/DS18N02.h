/***********************************************
�����ˣ���־ǿ
ʱ�䣺2010.5.17
���ܣ�DS18B20ͷ�ļ�
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