#ifndef VECTORS_H
#define VECTORS_H

#include "common.h"

// RAM����ַ
#define VECTOR_TOP_RAM       (0x43FF)
#define VECTOR_TOP_ROM       (0xFFFF)
//�ж���������
#define VECTOR_SIZE               64
//�ж���������ʼ��ַ
#define VECTOR_RAM_START_ADDR  (VECTOR_TOP_RAM + 1 - 2*VECTOR_SIZE)
#define VECTOR_ROM_START_ADDR  (VECTOR_TOP_ROM + 1 - 2*VECTOR_SIZE)

typedef uint16_t ISR_TYPE;
//typedef __interrupt void (*ISR_TYPE)();//ռ���ĸ��ֽڣ����Բ����ʣ�Ҫ��ֻ��ռ�������ֽ�
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
  /************************Ϊʹ�÷��㣬����Բ��ֳ��ù��ܽ��ж���**************************************************/
  //IO��
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
  //TIMER��ע�ⶨʱ���������ж�������CCR0����ռ��һ���ж������������ж�Դ��ͬʹ��һ���ж�����
  VECTOR_TIMER_A0      = VECTOR_TIMER0_A1, 
  VECTOR_TIMER_A0_CCR0 = VECTOR_TIMER0_A0,
  VECTOR_TIMER_A1      = VECTOR_TIMER1_A1,
  VECTOR_TIMER_A1_CCR0 = VECTOR_TIMER1_A0,
  VECTOR_TIMER_A2      = VECTOR_TIMER2_A1,
  VECTOR_TIMER_A2_CCR0 = VECTOR_TIMER2_A0,
  VECTOR_TIMER_B0      = VECTOR_TIMER0_B1,
  VECTOR_TIMER_B0_CCR0 = VECTOR_TIMER0_B0,
}VECTORn;         //�ж�����

extern void Set_Vector_Handler(VECTORn vector, __interrupt void pfunc_handler(void));  //�����ж�������ַ
extern void Set_VectorTable_To_RAM(void);                                       //�����ж�����ӳ�䵽RAM����
extern void Set_VectorTable_To_FLASH(void);                                     //�����ж�����ӳ�䵽FLASH����
extern void VectorTableCopyToRAM(void);                                         //��FLASH�и����ж�������RAM

#endif