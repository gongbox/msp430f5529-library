#ifndef __WIFI_H
#define __WIFI_H

#include "common.h"

//宏定义FIFO大小
#define FIFO_SIZE     256

extern void WIFI_Init();                                                        //模块初始化  
extern void WIFI_SendString(char * string);                                     //发送一字节数据到服务器
extern void WIFI_FIFO_Write(char data);                                         //FIFO写入一字节
extern char WIFI_FIFO_Read();                                                   //FIFO读取一字节
extern char * WIFI_FIFO_Read_String();                                          //一次性读取所有数据
extern uint16 WIFI_FIFO_Count();                                                //FIFO未读取数量


#endif