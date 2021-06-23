/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                              (c) Copyright 2002, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                                TI MSP430
*********************************************************************************************************
*/
#include "include.h"
#include "includes.h"

#define START_TASK_PRIO  10                    //任务优先级为10
#define START_STK_SIZE   128                   //设置任务堆栈大小
OS_STK  START_TASK_STK[START_STK_SIZE];       //任务堆栈
void Start_Task(void * pdata);                //任务函数
//KEY任务
#define KEY_TASK_PRIO    3
#define KEY_STK_SIZE     64
OS_STK  KEY_TASK_STK[KEY_STK_SIZE];
void KEY_Task(void * pdata);
//LED任务
#define LED_TASK_PRIO    5                   //任务优先级为5
#define LED_STK_SIZE     64                 //设置任务堆栈大小
OS_STK  LED_TASK_STK[LED_STK_SIZE];          //任务堆栈
void LED_Task(void * pdata);                 //任务函数
//LCD任务
#define LCD_TASK_PRIO    8
#define LCD_STK_SIZE     256
OS_STK  LCD_TASK_STK[LCD_STK_SIZE];
void LCD_Task(void * pdata);
//Beep任务
#define Beep_TASK_PRIO   6
#define Beep_STK_SIZE    64
OS_STK  Beep_TASK_STK[Beep_STK_SIZE];
void Beep_Task(void * pdata);

OS_EVENT * msg_key;                  //按键邮箱事件块指针
OS_EVENT * sem_led;                  //按键信号量指针
/*
OS_TMR   * tmr1;
//软件定时器1的回调函数
void tmr1_callback(OS_TMR * ptmr,void * p_arg)
{
  
}*/
void Start_Task(void * pdata)
{
  OS_CPU_SR cpu_sr;
  uint8 err;
  pdata=pdata;
  msg_key=OSMboxCreate((void *)0);   //创建消息邮箱
  sem_led=OSSemCreate(1);            //创建信号量
  OSStatInit();                      //初始化统计任务，这里会延时1s左右
  OS_ENTER_CRITICAL();                //进入临界区，禁止中断打断
    OSTaskCreate(KEY_Task,(void *)0,(OS_STK *)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
    OSTaskCreate(LED_Task,(void *)0,(OS_STK *)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(LCD_Task,(void *)0,(OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE-1],LCD_TASK_PRIO);
    OSTaskCreate(Beep_Task,(void *)0,(OS_STK *)&Beep_TASK_STK[Beep_STK_SIZE-1],Beep_TASK_PRIO);
    /*tmr1=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,
                     (OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);
    */
  OSTaskSuspend(START_TASK_PRIO);   //挂起起始任务
  OS_EXIT_CRITICAL();
}
void KEY_Task(void * pdata)
{
  uint8 key,err;
  Key_Init(KEY_ALL,GPI|PULL_UP);               //按键初始化
  while(1)
  {
    key= !Key_Status_Get(KEY0);                           //按键状态获取
    OSMboxPost(msg_key,(void *)key);
    if(key)
      OSSemPost(sem_led);                                 //释放信号量
    //else
      //OSSemPend(sem_led,2,&err);          //请求信号量，无限等待时间
    OSTimeDly(5);
  }
}
void LCD_Task(void * pdata)
{
  OS_CPU_SR cpu_sr;
  LCD_INIT();
  LCD_Display_Full(WHITE);
  uint8 err;
  int i=0; 
  uint8 key;
  while(1)
  {
    //OS_ENTER_CRITICAL();
    uint8 key=(uint32)OSMboxPend(msg_key,10,&err);
    if(key)
    {
      LCD_Printf_8x16(0,20,"What do you want to do?");
    }
    else
    {
      LCD_Printf_8x16(0,20,"                       ");
    }
    LCD_Printf_8x16(0,0,"CPU Usage=%f",(float)OSCPUUsage);
    //OS_EXIT_CRITICAL();
    OSTimeDly(50);
  }
}
void LED_Task(void * pdata)
{
  LED_Init(LED0);              //LED灯初始化
  uint8 err;
  while(1)
  {
    uint8 key=(uint32)OSMboxPend(msg_key,0,&err);
    LED_SetStatus(LED0,key);  //设置LED状态
    OSTimeDly(10);
  }
}
void Beep_Task(void * pdata)
{
  LED_Init(LED1);              //LED灯初始化
  uint8 err;
  while(1)
  {
    OSSemPend(sem_led,0,&err);          //请求信号量，无限等待时间
    LED_Turn(LED1);               //指示灯闪烁
    OSTimeDly(500);
    LED_Turn(LED1);               //指示灯闪烁
    OSTimeDly(500);
  }
}
#if 0
extern __interrupt void TICKS_ISR(void);

void main()
{
  DisableWatchDog();            //关闭看门狗
  DisableInterrupts();           //禁止总中断
  DCO_Set_Frequency (DCO_25MHZ);     //设置DCO频率为16MHZ
  Clock_Set_Source (DCO,XT2,XT1);    //设置时钟源
  LED_Init(LED_ALL);              //LED灯初始化
  EnableInterrupts();
  
  //Set_Vector_Handler(VECTOR_WDT,TICKS_ISR);    //设置中断向量
  //WatchDog_Init(5,IntervalTimer); //使用看门狗提供定时中断，时间很不可靠，建议换为TA或TB
  
  Set_Vector_Handler(VECTOR_TIMER0_B0,TICKS_ISR);    //设置中断向量
  TIMER_MsInterval_Init(TIMER_B0,1000/OS_TICKS_PER_SEC);                    //初始化一个ms级的定时中断
  TIMER_IRQ_Enable (TIMER_B0,TIMER_CCR0_Interrupt);              //使能TIMER的某一个中断
  
  OSInit();                                              /* 初始化uCOS-II */
    OSTaskCreate(Start_Task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
  OSStart();                                             /* 开始任务调度  */
}
#endif