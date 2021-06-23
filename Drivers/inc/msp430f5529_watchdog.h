#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include"common.h"

extern void  DisableWatchDog(void);                                             //看门狗功能关闭
//注意定时时间只是比较接近设置时间的一个时间，并不精准
extern void  WatchDog_Init(uint32_t ms);                                        //看门狗功能配置
extern void  WatchDog_Timer_Init(uint32_t ms);                                  //看门狗定时器功能初始化
extern void  WatchDog_Delay_Ms  (uint32_t ms);                                  //看门狗延时使用

extern void  WatchDog_Feed(void);                                               //喂狗
extern void  WatchDog_ITConfig(STATUS ITState);                                 //设置看门狗中断是否使能
extern void  SoftWare_RESET(void);                                              //软件触发一次复位

#endif