#ifndef __WIFI_H
#define __WIFI_H

#include "common.h"

//�궨��FIFO��С
#define FIFO_SIZE     256

extern void WIFI_Init();                                                        //ģ���ʼ��  
extern void WIFI_SendString(char * string);                                     //����һ�ֽ����ݵ�������
extern void WIFI_FIFO_Write(char data);                                         //FIFOд��һ�ֽ�
extern char WIFI_FIFO_Read();                                                   //FIFO��ȡһ�ֽ�
extern char * WIFI_FIFO_Read_String();                                          //һ���Զ�ȡ��������
extern uint16 WIFI_FIFO_Count();                                                //FIFOδ��ȡ����


#endif