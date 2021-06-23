#ifndef TEMPATURE_H_
#define TEMPATURE_H_

#include "common.h" 

#define DS18B20_PORT   P6
#define DS18B20_PIN    5
#define DS18B20_OUT    PIN_OUT(P6,5)
#define DS18B20_IN     PIN_IN(P6,5)

void  B20_Init(void);                                                           //��λDS18B20
uint8_t B20_ReadByte(void);                                                       //��ȡһ���ֽڵ�����
void  B20_WriteByte(uint8_t data);                                              //д��һ���ֽڵ�����,dataΪDS18B20��ROM���ƣ��û����ù��������ö�����ֵ
float B20_Read_temp(void);                                                      //��ȡ�¶�ֵ

#endif
