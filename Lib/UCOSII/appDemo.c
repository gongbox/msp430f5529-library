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

#define START_TASK_PRIO  10                    //�������ȼ�Ϊ10
#define START_STK_SIZE   128                   //���������ջ��С
OS_STK  START_TASK_STK[START_STK_SIZE];       //�����ջ
void Start_Task(void * pdata);                //������
//KEY����
#define KEY_TASK_PRIO    3
#define KEY_STK_SIZE     64
OS_STK  KEY_TASK_STK[KEY_STK_SIZE];
void KEY_Task(void * pdata);
//LED����
#define LED_TASK_PRIO    5                   //�������ȼ�Ϊ5
#define LED_STK_SIZE     64                 //���������ջ��С
OS_STK  LED_TASK_STK[LED_STK_SIZE];          //�����ջ
void LED_Task(void * pdata);                 //������
//LCD����
#define LCD_TASK_PRIO    8
#define LCD_STK_SIZE     256
OS_STK  LCD_TASK_STK[LCD_STK_SIZE];
void LCD_Task(void * pdata);
//Beep����
#define Beep_TASK_PRIO   6
#define Beep_STK_SIZE    64
OS_STK  Beep_TASK_STK[Beep_STK_SIZE];
void Beep_Task(void * pdata);

OS_EVENT * msg_key;                  //���������¼���ָ��
OS_EVENT * sem_led;                  //�����ź���ָ��
/*
OS_TMR   * tmr1;
//�����ʱ��1�Ļص�����
void tmr1_callback(OS_TMR * ptmr,void * p_arg)
{
  
}*/
void Start_Task(void * pdata)
{
  OS_CPU_SR cpu_sr;
  uint8 err;
  pdata=pdata;
  msg_key=OSMboxCreate((void *)0);   //������Ϣ����
  sem_led=OSSemCreate(1);            //�����ź���
  OSStatInit();                      //��ʼ��ͳ�������������ʱ1s����
  OS_ENTER_CRITICAL();                //�����ٽ�������ֹ�жϴ��
    OSTaskCreate(KEY_Task,(void *)0,(OS_STK *)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);
    OSTaskCreate(LED_Task,(void *)0,(OS_STK *)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO);
    OSTaskCreate(LCD_Task,(void *)0,(OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE-1],LCD_TASK_PRIO);
    OSTaskCreate(Beep_Task,(void *)0,(OS_STK *)&Beep_TASK_STK[Beep_STK_SIZE-1],Beep_TASK_PRIO);
    /*tmr1=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,
                     (OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);
    */
  OSTaskSuspend(START_TASK_PRIO);   //������ʼ����
  OS_EXIT_CRITICAL();
}
void KEY_Task(void * pdata)
{
  uint8 key,err;
  Key_Init(KEY_ALL,GPI|PULL_UP);               //������ʼ��
  while(1)
  {
    key= !Key_Status_Get(KEY0);                           //����״̬��ȡ
    OSMboxPost(msg_key,(void *)key);
    if(key)
      OSSemPost(sem_led);                                 //�ͷ��ź���
    //else
      //OSSemPend(sem_led,2,&err);          //�����ź��������޵ȴ�ʱ��
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
  LED_Init(LED0);              //LED�Ƴ�ʼ��
  uint8 err;
  while(1)
  {
    uint8 key=(uint32)OSMboxPend(msg_key,0,&err);
    LED_SetStatus(LED0,key);  //����LED״̬
    OSTimeDly(10);
  }
}
void Beep_Task(void * pdata)
{
  LED_Init(LED1);              //LED�Ƴ�ʼ��
  uint8 err;
  while(1)
  {
    OSSemPend(sem_led,0,&err);          //�����ź��������޵ȴ�ʱ��
    LED_Turn(LED1);               //ָʾ����˸
    OSTimeDly(500);
    LED_Turn(LED1);               //ָʾ����˸
    OSTimeDly(500);
  }
}
#if 0
extern __interrupt void TICKS_ISR(void);

void main()
{
  DisableWatchDog();            //�رտ��Ź�
  DisableInterrupts();           //��ֹ���ж�
  DCO_Set_Frequency (DCO_25MHZ);     //����DCOƵ��Ϊ16MHZ
  Clock_Set_Source (DCO,XT2,XT1);    //����ʱ��Դ
  LED_Init(LED_ALL);              //LED�Ƴ�ʼ��
  EnableInterrupts();
  
  //Set_Vector_Handler(VECTOR_WDT,TICKS_ISR);    //�����ж�����
  //WatchDog_Init(5,IntervalTimer); //ʹ�ÿ��Ź��ṩ��ʱ�жϣ�ʱ��ܲ��ɿ������黻ΪTA��TB
  
  Set_Vector_Handler(VECTOR_TIMER0_B0,TICKS_ISR);    //�����ж�����
  TIMER_MsInterval_Init(TIMER_B0,1000/OS_TICKS_PER_SEC);                    //��ʼ��һ��ms���Ķ�ʱ�ж�
  TIMER_IRQ_Enable (TIMER_B0,TIMER_CCR0_Interrupt);              //ʹ��TIMER��ĳһ���ж�
  
  OSInit();                                              /* ��ʼ��uCOS-II */
    OSTaskCreate(Start_Task, (void *)0, (OS_STK *)&START_TASK_STK[START_STK_SIZE - 1], START_TASK_PRIO);
  OSStart();                                             /* ��ʼ�������  */
}
#endif