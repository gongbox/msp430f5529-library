#ifndef TEMPATURE_H_
#define TEMPATURE_H_

#include "common.h" 

#define DS18B20_PORT   P6
#define DS18B20_PIN    5
#define DS18B20_OUT    PIN_OUT(P6,5)
#define DS18B20_IN     PIN_IN(P6,5)

void  B20_Init(void);                                                           //复位DS18B20
uint8_t B20_ReadByte(void);                                                       //读取一个字节的数据
void  B20_WriteByte(uint8_t data);                                              //写入一个字节的数据,data为DS18B20的ROM控制，用户不用管它，不用对它赋值
float B20_Read_temp(void);                                                      //读取温度值

#endif
