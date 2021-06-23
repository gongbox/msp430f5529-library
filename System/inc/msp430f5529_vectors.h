#ifndef VECTORS_H
#define VECTORS_H

#include "common.h"

// RAM最大地址
#define VECTOR_TOP_RAM       (0x43FF)
#define VECTOR_TOP_ROM       (0xFFFF)
//中断向量数量
#define VECTOR_SIZE               64
//中断向量表起始地址
#define VECTOR_RAM_START_ADDR  (VECTOR_TOP_RAM + 1 - 2*VECTOR_SIZE)
#define VECTOR_ROM_START_ADDR  (VECTOR_TOP_ROM + 1 - 2*VECTOR_SIZE)

typedef uint16_t ISR_TYPE;
//typedef __interrupt void (*ISR_TYPE)();//占用四个字节，所以不合适，要求只能占用两个字节
//typedef void * ISR_TYPE;

typedef enum
{
  VECTOR_RTC      =41u, /*  RTC */
  VECTOR_PORT2        , /*  Port 2 */
  VECTOR_TIMER2_A1    , /*  Timer0_A5 CC1-4, TA */
  VECTOR_TIMER2_A0    , /*  Timer0_A5 CC0 */
  VECTOR_USCI_B1      , /*  USCI B1 Receive/Transmit */
  VECTOR_USCI_A1      , /*  USCI A1 Receive/Transmit */
  VECTOR_PORT1        , /*  Port 1 */
  VECTOR_TIMER1_A1    , /*  Timer1_A3 CC1-2, TA1 */
  VECTOR_TIMER1_A0    , /*  Timer1_A3 CC0 */
  VECTOR_DMA          , /*  DMA */
  VECTOR_USB_UBM      , /*  USB Timer / cable event / USB reset */
  VECTOR_TIMER0_A1    , /*  Timer0_A5 CC1-4, TA */
  VECTOR_TIMER0_A0    , /*  Timer0_A5 CC0 */
  VECTOR_ADC12        , /*  ADC */
  VECTOR_USCI_B0      , /*  USCI B0 Receive/Transmit */
  VECTOR_USCI_A0      , /*  USCI A0 Receive/Transmit */
  VECTOR_WDT          , /*  Watchdog Timer */
  VECTOR_TIMER0_B1    , /*  Timer0_B7 CC1-6, TB */
  VECTOR_TIMER0_B0    , /*  Timer0_B7 CC0 */
  VECTOR_COMP_B       , /*  Comparator B */
  VECTOR_UNMI         , /*  User Non-maskable */
  VECTOR_SYSNMI       , /*  System Non-maskable */
  VECTOR_RESET        , /*  Reset [Highest Priority] */
  /************************为使用方便，下面对部分常用功能进行定义**************************************************/
  //IO口
  VECTOR_P1            = VECTOR_PORT1,
  VECTOR_P2            = VECTOR_PORT2, 
  //SPI
  VECTOR_SPI0          = VECTOR_USCI_A0,
  VECTOR_SPI1          = VECTOR_USCI_A1,
  VECTOR_SPI2          = VECTOR_USCI_B0,
  VECTOR_SPI3          = VECTOR_USCI_B1,
  //I2C
  VECTOR_I2C0          = VECTOR_USCI_B0,
  VECTOR_I2C1          = VECTOR_USCI_B1,
  //UART
  VECTOR_UART0         = VECTOR_USCI_A0,
  VECTOR_UART1         = VECTOR_USCI_A1,
  //TIMER，注意定时器有两个中断向量，CCR0单独占用一个中断向量，其他中断源共同使用一个中断向量
  VECTOR_TIMER_A0      = VECTOR_TIMER0_A1, 
  VECTOR_TIMER_A0_CCR0 = VECTOR_TIMER0_A0,
  VECTOR_TIMER_A1      = VECTOR_TIMER1_A1,
  VECTOR_TIMER_A1_CCR0 = VECTOR_TIMER1_A0,
  VECTOR_TIMER_A2      = VECTOR_TIMER2_A1,
  VECTOR_TIMER_A2_CCR0 = VECTOR_TIMER2_A0,
  VECTOR_TIMER_B0      = VECTOR_TIMER0_B1,
  VECTOR_TIMER_B0_CCR0 = VECTOR_TIMER0_B0,
}VECTORn;         //中断向量

extern void Set_Vector_Handler(VECTORn vector, __interrupt void pfunc_handler(void));  //设置中断向量地址
extern void Set_VectorTable_To_RAM(void);                                       //设置中断向量映射到RAM区域
extern void Set_VectorTable_To_FLASH(void);                                     //设置中断向量映射到FLASH区域
extern void VectorTableCopyToRAM(void);                                         //从FLASH中复制中断向量表到RAM

#endif