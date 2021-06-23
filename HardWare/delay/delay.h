#ifndef DELAY_H_
#define DELAY_H_

#include "common.h"

#define CPU_FRE_MHZ     g_sClock.MCLK.fMHZ

extern void delay_us(uint32_t us);                    //US—” ±
#define     delay_ms(ms)   delay_us((uint32_t)ms*1000)//MS—” ±

#endif