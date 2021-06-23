#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include"common.h"

extern void  DisableWatchDog(void);                                             //���Ź����ܹر�
//ע�ⶨʱʱ��ֻ�ǱȽϽӽ�����ʱ���һ��ʱ�䣬������׼
extern void  WatchDog_Init(uint32_t ms);                                        //���Ź���������
extern void  WatchDog_Timer_Init(uint32_t ms);                                  //���Ź���ʱ�����ܳ�ʼ��
extern void  WatchDog_Delay_Ms  (uint32_t ms);                                  //���Ź���ʱʹ��

extern void  WatchDog_Feed(void);                                               //ι��
extern void  WatchDog_ITConfig(STATUS ITState);                                 //���ÿ��Ź��ж��Ƿ�ʹ��
extern void  SoftWare_RESET(void);                                              //�������һ�θ�λ

#endif