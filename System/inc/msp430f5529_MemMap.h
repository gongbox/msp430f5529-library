#ifndef MEMMAP_H_
#define MEMMAP_H_

#include "common.h"
#include "msp430f5529_gpio.h"

typedef struct ADC_MemMap
{
  union
  {
    uint16_t CTL0;
    struct
    {
      uint16_t SC             :1;  /* ADC12 Start Conversion */
      uint16_t ENC            :1;  /* ADC12 Enable Conversion */
      uint16_t TOVIE          :1;  /* ADC12 Timer Overflow interrupt enable */
      uint16_t OVIE           :1;  /* ADC12 Overflow interrupt enable */
      uint16_t ON             :1;  /* ADC12 On/enable */
      uint16_t ADCREFON       :1;  /* ADC12 Reference on */
      uint16_t REF2_5V        :1;  /* ADC12 Ref 0:1.5V / 1:2.5V */
      uint16_t MSC            :1;  /* ADC12 Multiple SampleConversion */
      uint16_t SHT0           :4;  /* ADC12 Sample Hold 0 Select  */
      uint16_t SHT1           :4;  /* ADC12 Sample Hold 1 Select  */
    };
  };
  union
  {
    uint16_t CTL1;
    struct
    {
      uint16_t ADBUSY         :1;    /* ADC12 Busy */
      uint16_t CONSEQ         :2;    /* ADC12 Conversion Sequence Select  */
      uint16_t SSEL           :2;    /* ADC12 Clock Source Select  */
      uint16_t DIV            :3;    /* ADC12 Clock Divider Select  */
      uint16_t ISSH           :1;    /* ADC12 Invert Sample Hold Signal */
      uint16_t SHP            :1;    /* ADC12 Sample/Hold Pulse Mode */
      uint16_t SHS            :2;    /* ADC12 Sample/Hold Source  */
      uint16_t CSTARTADD      :4;    /* ADC12 Conversion Start Address  */
    };
  };
  union
  {
    uint16_t CTL2;
    struct
    {
      uint16_t REFBURST       :1;    /* ADC12+ Reference Burst */
      uint16_t ADCREFOUT      :1;    /* ADC12+ Reference Out */
      uint16_t SR             :1;    /* ADC12+ Sampling Rate */
      uint16_t DF             :1;    /* ADC12+ Data Format */
      uint16_t RES            :2;    /* ADC12+ Resolution  */
      uint16_t TCOFF          :1;    /* ADC12+ Temperature Sensor Off */
      uint16_t PDIV           :1;    /* ADC12+ predivider 0:/1   1:/4 */
    };
  };
  uint16_t RESERVED_1[2];
  uint16_t IFG;
  uint16_t IE;
  uint16_t IV; 
  struct
  {
      uint8_t INCH            :4;     /* ADC12 Input Channel Select*/
      uint8_t REF             :3;     /* ADC12 Select Reference  */
      uint8_t EOS             :1;     /* ADC12 End of Sequence */
  }MCTL[16];
  uint16_t MEM[16];
}volatile * ADC_MemMapPtr;

#define ADC12_SSEL_ADC12OSC       0
#define ADC12_SSEL_ACLK           1                   
#define ADC12_SSEL_MCLK           2
#define ADC12_SSEL_SMCLK          3   

#define ADC12_SREF_0            0
#define ADC12_SREF_1            1 
#define ADC12_SREF_2            2
#define ADC12_SREF_3            3
#define ADC12_SREF_4            4
#define ADC12_SREF_5            5
#define ADC12_SREF_6            6
#define ADC12_SREF_7            7

#define ADC_BASE_PTR                     ((ADC_MemMapPtr)__MSP430_BASEADDRESS_ADC12_PLUS__)

typedef struct UCS_MemMap
{
  union
  {
    uint16_t CTL0;
    struct 
    {
      uint8_t :3;
      uint8_t MOD:5;
      uint8_t DCO:5;
    };
  };
  union
  {
    uint16_t CTL1;
    struct
    {
      uint8_t DisMOD   :1;
      uint8_t :3;
      uint8_t DCORSEL  :3;
    };
  };
  union
  {
    uint16_t CTL2;
    struct
    {
      uint16_t FLLN    :10;
      uint16_t :2;
      uint16_t FLLD     :3;
    };
  };
  union
  {
    uint16_t CTL3;
    struct
    {
      uint8_t FLLREFDIV  :3;
      uint8_t :1;
      uint8_t SELREF     :3;
    };
  };
  union
  {
    uint16_t CTL4;
    struct
    {
      uint8_t SELM  :3;
      uint8_t :1;
      uint8_t SELS  :3;
      uint8_t :1;
      uint8_t SELA  :3;
      uint8_t :1;
    };
  };
  union
  {
    uint16_t CTL5;
    struct
    {
      uint8_t DIVM  :3;
      uint8_t :1;
      uint8_t DIVS  :3;
      uint8_t :1;
      uint8_t DIVA  :3;
      uint8_t :1;
      uint8_t DIVPA :3;
      uint8_t :1;
    };
  };
  union
  {
    uint16_t CTL6;
    struct
    {
      uint8_t XT1_OFF   :1; /* High Frequency Oscillator 1 (XT1) disable */
      uint8_t SMCLK_OFF :1; /* SMCLK Off */
      uint8_t XCAP      :2; /* XIN/XOUT*/
      uint8_t XT1_BYPASS:1; /* XT1 bypass mode : 0: internal 1:sourced from external pin */
      uint8_t XT_S      :1; /* 1: Selects high-freq. oscillator */
      uint8_t XT1DRIVE  :2; /* XT1 Drive Level mode */   
      uint8_t XT2_OFF   :1; /* High Frequency Oscillator 2 (XT2) disable */
      uint8_t :3;
      uint8_t XT2_BYPASS:1; /* XT2 bypass mode : 0: internal 1:sourced from external pin */
      uint8_t :1;
      uint8_t XT2DRIVE  :2; /* XT2 Drive Level mode  */
    };
  };
  union
  {
    uint16_t CTL7;
    struct
    {
      uint8_t DCO_FFG              :1;    /* DCO Fault Flag */
      uint8_t XT1_LFOFFG           :1;    /* XT1 Low Frequency Oscillator Fault Flag */
      uint8_t :1;     
      uint8_t XT2_OFFG             :1;    /* High Frequency Oscillator 2 Fault Flag */
    };
  };
  union
  {
    uint16_t CTL8;
    struct
    {
      uint8_t ACLK_REQEN           :1;    /* ACLK Clock Request Enable */
      uint8_t MCLK_REQEN           :1;    /* MCLK Clock Request Enable */
      uint8_t SMCLK_REQEN          :1;    /* SMCLK Clock Request Enable */
      uint8_t MODOSC_REQEN         :1;    /* MODOSC Clock Request Enable */
    };
  };
}volatile * UCS_MemMapPtr;

#define UCS_BASE_PTR                                  ((UCS_MemMapPtr)__MSP430_BASEADDRESS_UCS__)

extern const UCS_MemMapPtr UCS;

typedef struct COMPB_MemMap
{
  union
  {
    uint16_t CTL0;
    struct
    {
      uint16_t IPSEL              :4;  /* Comp. B Pos. Channel Input Select 3 */
      uint16_t :3;
      uint16_t IPEN               :1;  /* Comp. B Pos. Channel Input Enable */
      uint16_t IMSEL              :4;  /* Comp. B Neg. Channel Input Select 3 */
      uint16_t :3;
      uint16_t IMEN               :1;  /* Comp. B Neg. Channel Input Enable */
    };
  };
  union
  {
    uint16_t CTL1;
    struct
    {
      uint16_t CB_OUT            :1;  /* Comp. B Output */
      uint16_t OUTPOL            :1;  /* Comp. B Output Polarity */
      uint16_t F                 :1;  /* Comp. B Enable Output Filter */
      uint16_t IES               :1;  /* Comp. B Interrupt Edge Select */
      uint16_t SHORT             :1;  /* Comp. B Input Short */
      uint16_t EX                :1;  /* Comp. B Exchange Inputs */
      uint16_t FDLY              :2;  /* Comp. B Filter delay  */
      uint16_t PWRMD             :2;  /* Comp. B Power Mode */
      uint16_t ON                :1;  /* Comp. B enable */
      uint16_t MRVL              :1;  /* Comp. B MRV Level */
      uint16_t MRVS              :1;  /* Comp. B Output selects between VREF0 or VREF1*/
      uint16_t :3;
    };
  };
  union
  {
    uint16_t CTL2;
    struct
    {
      uint16_t REF0              :5;  /* Comp. B Reference 0 Resistor Select Bit : 4 */
      uint16_t RSEL              :1;  /* Comp. B Reference select */
      uint16_t RS                :2;  /* Comp. B Reference Source Bit : 1 */
      uint16_t REF1              :5;  /* Comp. B Reference 1 Resistor Select Bit : 4 */
      uint16_t REFL              :2;  /* Comp. B Reference voltage level Bit : 1 */
      uint16_t REFACC            :1;  /* Comp. B Reference Accuracy */
    };
  };
  union
  {
    uint16_t CTL3;
    struct
    {
      uint16_t PD0               :1;  /* Comp. B Disable Input Buffer of Port Register .0 */
      uint16_t PD1               :1;  /* Comp. B Disable Input Buffer of Port Register .1 */
      uint16_t PD2               :1;  /* Comp. B Disable Input Buffer of Port Register .2 */
      uint16_t PD3               :1;  /* Comp. B Disable Input Buffer of Port Register .3 */
      uint16_t PD4               :1;  /* Comp. B Disable Input Buffer of Port Register .4 */
      uint16_t PD5               :1;  /* Comp. B Disable Input Buffer of Port Register .5 */
      uint16_t PD6               :1;  /* Comp. B Disable Input Buffer of Port Register .6 */
      uint16_t PD7               :1;  /* Comp. B Disable Input Buffer of Port Register .7 */
      uint16_t PD8               :1;  /* Comp. B Disable Input Buffer of Port Register .8 */
      uint16_t PD9               :1;  /* Comp. B Disable Input Buffer of Port Register .9 */
      uint16_t PD10              :1;  /* Comp. B Disable Input Buffer of Port Register .10 */
      uint16_t PD11              :1;  /* Comp. B Disable Input Buffer of Port Register .11 */
      uint16_t PD12              :1;  /* Comp. B Disable Input Buffer of Port Register .12 */
      uint16_t PD13              :1;  /* Comp. B Disable Input Buffer of Port Register .13 */
      uint16_t PD14              :1;  /* Comp. B Disable Input Buffer of Port Register .14 */
      uint16_t PD15              :1;  /* Comp. B Disable Input Buffer of Port Register .15 */
    };
  };
  uint16_t RESERVED[2];
  union
  {
    uint16_t INT;
    struct
    {
      union
      {
      uint8_t CB_IFG;
      struct
      {
        uint8_t IFG                :1;  /* Comp. B Interrupt Flag */
        uint8_t IIFG               :1;  /* Comp. B Interrupt Flag Inverted Polarity */
        uint8_t :6;
      };
    };
    union
    {
      uint8_t CB_IE;
      struct
      {
        uint8_t IE                 :1;  /* Comp. B Interrupt Enable */
        uint8_t IIE                :1;  /* Comp. B Interrupt Enable Inverted Polarity */
        uint8_t :6;
      };
    };
    };
  };
  union
  {
    uint16_t IV;
    struct
    {
      uint16_t IV_NONE           :1;    /* No Interrupt pending */
      uint16_t IV_IFG            :1;    /* IFG */
      uint16_t IV_IIFG           :1;    /* IIFG */
    };
  };
}volatile * COMPB_MemMapPtr;

#define COMPB_BASE_PTR                  ((COMPB_MemMapPtr)__MSP430_BASEADDRESS_COMPB__)

/** GPIO - Peripheral register structure */
#include "msp430f5529_gpio.h"
typedef struct GPIO_MemMap 
{
#ifdef GPIO_PORT_SIZE_WORD
  uint16_t IN; 
  uint16_t POUT;                  
  uint16_t PDIR;                   
  uint16_t REN;                      
  uint16_t DS;                      
  uint16_t SEL;                     
  uint8_t RESERVED_0[2];
  uint16_t :16;                  
  uint8_t RESERVED_1[8];
  uint16_t IES;                      
  uint16_t IE;                       
  uint16_t IFG;                     
  uint16_t :16;
#else
  uint8_t IN;                      
  uint8_t :8;  //无名成员，占用8bit，只用来作填充或调整位置，
  uint8_t POUT;                  
  uint8_t :8;
  uint8_t PDIR;                   
  uint8_t :8;
  uint8_t REN;                      
  uint8_t :8;
  uint8_t DS;                      
  uint8_t :8;
  uint8_t SEL;                     
  uint8_t :8;
  uint8_t RESERVED_0[2];
  uint16_t :16;                  
  uint8_t RESERVED_1[8];
  uint8_t IES;                      
  uint8_t :8;
  uint8_t IE;                       
  uint8_t :8;
  uint8_t IFG;                     
  uint8_t :8;
  uint16_t :16;
#endif
} volatile * GPIO_MemMapPtr;
/* GPIO - Peripheral instance base addresses */
#define PA_BASE_PTR                             ((GPIO_MemMapPtr)&PAIN)
#define PB_BASE_PTR                             ((GPIO_MemMapPtr)&PBIN)
#define PC_BASE_PTR                             ((GPIO_MemMapPtr)&PCIN)
#define PD_BASE_PTR                             ((GPIO_MemMapPtr)&PDIN)

#define P1_BASE_PTR                             ((GPIO_MemMapPtr)&P1IN)
#define P2_BASE_PTR                             ((GPIO_MemMapPtr)&P2IN)
#define P3_BASE_PTR                             ((GPIO_MemMapPtr)&P3IN)
#define P4_BASE_PTR                             ((GPIO_MemMapPtr)&P4IN)
#define P5_BASE_PTR                             ((GPIO_MemMapPtr)&P5IN)
#define P6_BASE_PTR                             ((GPIO_MemMapPtr)&P6IN)
#define P7_BASE_PTR                             ((GPIO_MemMapPtr)&P7IN)
#define P8_BASE_PTR                             ((GPIO_MemMapPtr)&P8IN)


typedef struct DMA_MemMap
{
  uint8_t TSSEL[8];
  union
  {
    uint16_t CTL4;
    struct
    {
      uint16_t DMA_ENNMI          :1;    /* Enable NMI interruption of DMA */
      uint16_t DMA_ROUNDROBIN     :1;    /* Round-Robin DMA channel priorities */
      uint16_t RMWDIS             :1;    /* Inhibited DMA transfers during read-modify-write CPU operations */
    };
  };
  uint16_t RESERVED[2];
  uint16_t IV;
  struct
  {
    union
    {
      uint16_t CTL;
      struct
      {
        uint16_t REQ              :1;    /* Initiate DMA transfer with DMATSEL */
        uint16_t ABORT            :1;    /* DMA transfer aborted by NMI */
        uint16_t IE               :1;    /* DMA interrupt enable */
        uint16_t IFG              :1;    /* DMA interrupt flag */
        uint16_t EN               :1;    /* DMA enable */
        uint16_t LEVEL            :1;    /* DMA level sensitive trigger select */
        uint16_t SRCBYTE          :1;    /* DMA source byte */
        uint16_t DSTBYTE          :1;    /* DMA destination byte */
        uint16_t SRCINCR          :2;    /* DMA source increment bit 1 */
        uint16_t DSTINCR          :2;    /* DMA destination increment bit 1 */
        uint16_t DT               :3;    /* DMA transfer mode bit 2 */
      };
    }; 
    uint32_t SA;
    uint32_t DA;
    uint16_t SZ;
    uint16_t RESERVED[2];
  }CHX[3];
}volatile * DMA_MemMapPtr;

#define DMA_BASE_PTR                           ((DMA_MemMapPtr)__MSP430_BASEADDRESS_DMAX_3__)

typedef struct TIMER_MemMap
{
  union
  {
    uint16_t CTL;
    struct
    {
      uint16_t IFG                :1;  /* Timer A counter interrupt flag */
      uint16_t IE                 :1;  /* Timer A counter interrupt enable */
      uint16_t CLR                :1;  /* Timer A counter clear */
      uint16_t :1;
      uint16_t MC                 :2;  /* Timer A mode control 1 */
      uint16_t ID                 :2;  /* Timer A clock input divider 1 */
      uint16_t SSEL               :2;  /* Timer A clock source select 1 */    
    };
  };
  union
  {
    uint16_t CTL;
    struct
    {
      uint16_t IFG                 :1;  /* Capture/compare interrupt flag */
      uint16_t CH_COV              :1;  /* Capture/compare overflow flag */
      uint16_t CH_OUT              :1;  /* PWM Output signal if output mode 0 */
      uint16_t CH_CCI              :1;  /* Capture input signal (read) */
      uint16_t IE                  :1;  /* Capture/compare interrupt enable */
      uint16_t OUTMOD              :3;  /* Output mode  */
      uint16_t CAPMODE             :1;  /* Capture mode: 1 /Compare mode : 0 */
      //TB
      //uint16_t CLLD                :2;
      //TA
      uint16_t :1;
      uint16_t CH_SCCI             :1;  /* Latched capture signal (read) */
      uint16_t CH_SCS              :1;  /* Capture sychronize */
      uint16_t CCIS                :2;  /* Capture input select */
      uint16_t CM                  :2;  /* Capture mode  */
    };
  }CCTL[7];
  uint16_t R;
  uint16_t CCR[7];
  uint16_t EX;
  uint16_t RESERVED[3];
  uint16_t IV;
}volatile * TIMER_MemMapPtr;

#define TIMER_MC_STOP                 (0)  /* Timer A mode control: 0 - Stop */
#define TIMER_MC_UP                   (1)  /* Timer A mode control: 1 - Up to CCR0 */
#define TIMER_MC_CONTINUOUS           (2)  /* Timer A mode control: 2 - Continuous up */
#define TIMER_MC_CONTINOUS            (2)  /* Legacy define */
#define TIMER_MC_UPDOWN               (3)  /* Timer A mode control: 3 - Up/Down */

#define TIMER_SSEL_TACLK              (0) /* Timer A clock source select: 0 - TACLK */
#define TIMER_SSEL_ACLK               (1) /* Timer A clock source select: 1 - ACLK  */
#define TIMER_SSEL_SMCLK              (2) /* Timer A clock source select: 2 - SMCLK */
#define TIMER_SSEL_INCLK              (3) /* Timer A clock source select: 3 - INCLK */

#define TIMER_CCIS_0                  (0) /* Capture input select: 0 - CCIxA */
#define TIMER_CCIS_1                  (1) /* Capture input select: 1 - CCIxB */
#define TIMER_CCIS_2                  (2) /* Capture input select: 2 - GND */
#define TIMER_CCIS_3                  (3) /* Capture input select: 3 - Vcc */

#define TIMER_A0_BASE_PTR                            ((TIMER_MemMapPtr)__MSP430_BASEADDRESS_T0A5__)
#define TIMER_A1_BASE_PTR                            ((TIMER_MemMapPtr)__MSP430_BASEADDRESS_T1A3__)
#define TIMER_A2_BASE_PTR                            ((TIMER_MemMapPtr)__MSP430_BASEADDRESS_T2A3__)
#define TIMER_B0_BASE_PTR                            ((TIMER_MemMapPtr)__MSP430_BASEADDRESS_T0B7__)

extern  const TIMER_MemMapPtr      TIMERX[];

typedef struct RTC_MemMap
{
  union
  {
    uint16_t CTL01;
    struct
    {
      uint8_t  CTL0;
      uint8_t  CTL1;
    };
    struct
    {
      uint16_t IFG:3;
      /*
      uint16_t RDYIFG           :1;     // RTC Ready Interrupt Flag
      uint16_t AIFG             :1;     // RTC Alarm Interrupt Flag
      uint16_t TEVIFG           :1;     // RTC Time Event Interrupt Flag
      */ 
      uint16_t :1;
      uint16_t IE:3;
      /*
      uint16_t RDYIE            :1;     // RTC Ready Interrupt Enable Flag
      uint16_t AIE              :1;     // RTC Alarm Interrupt Enable Flag
      uint16_t TEVIE            :1;     // RTC Time Event Interrupt Enable Flag
      */  
      uint16_t :1;
      uint16_t TEV0             :1;     /* RTC Time Event 0 */  
      uint16_t TEV1             :1;     /* RTC Time Event 1 */  
      uint16_t SSEL             :2;     /* RTC Source Select */  
      uint16_t RDY              :1;     /* RTC Ready */ 
      uint16_t MODE             :1;     /* RTC Mode 0:Counter / 1: Calendar */ 
      uint16_t HOLD             :1;     /* RTC Hold */  
      uint16_t BCD              :1;     /* RTC BCD  0:Binary / 1:BCD */
    };
  };
  union
  {
    uint16_t CTL23;
    struct
    {
      uint8_t  CTL2;
      uint8_t  CTL3;
    };
    struct
    {
      uint16_t CAL              :6;     /* RTC Calibration */
      uint16_t :1;  
      uint16_t CALS             :1;     /* RTC Calibration Sign */  
      uint16_t CALF             :2;     /* RTC Calibration Frequency */  
    };
  };
  uint16_t RESERVED_1[2];
  union
  {
    uint16_t PS0CTL;
    struct
    {
      uint16_t RT0_PSIFG            :1;     /* RTC Prescale Timer 0 Interrupt Flag */
      uint16_t RT0_PSIE             :1;     /* RTC Prescale Timer 0 Interrupt Enable Flag */  
      uint16_t RT0_IP               :3;     /* RTC Prescale Timer 0 Interrupt Interval */  
      uint16_t :3;  
      uint16_t RT0_PSHOLD           :1;     /* RTC Prescale Timer 0 Hold */ 
      uint16_t :2; 
      uint16_t RT0_PSDIV            :3;     /* RTC Prescale Timer 0 Clock Divide  */ 
      uint16_t RT0_SSEL             :1;     /* RTC Prescale Timer 0 Source Select 0:ACLK / 1:SMCLK */
      uint16_t :1;
    };
  };
  union
  {
    uint16_t PS1CTL;
    struct
    {
      uint16_t RT1_PSIFG            :1;     /* RTC Prescale Timer 1 Interrupt Flag */
      uint16_t RT1_PSIE             :1;     /* RTC Prescale Timer 1 Interrupt Enable Flag */  
      uint16_t RT1_IP               :3;     /* RTC Prescale Timer 1 Interrupt Interval  */  
      uint16_t :3;  
      uint16_t RT1_PSHOLD           :1;     /* RTC Prescale Timer 1 Hold */ 
      uint16_t :2; 
      uint16_t RT1_PSDIV            :3;     /* RTC Prescale Timer 1 Clock Divide  */ 
      uint16_t RT1_SSEL             :2;     /* RTC Prescale Timer 1 Source Select */
    };
  };
  uint16_t PS;
  uint16_t IV;
  
  uint16_t TIM0;
  uint16_t TIM1;
  uint16_t DATE;
  uint16_t YEAR;
  uint16_t AMINHR;
  uint16_t DOWDAY;
}volatile * RTC_MemMapPtr;

#define RTC_BASE_PTR                    ((RTC_MemMapPtr)__MSP430_BASEADDRESS_RTC__)
extern const RTC_MemMapPtr  RTC; 

typedef struct USCI_MemMap
{
  union
  {
    uint16_t CTLW;
    struct
    {
      union
      {
        uint8_t CTL1;
        //UART
        struct
        {
          uint8_t SWRST             :1;    /* USCI Software Reset */
          uint8_t TXBRK             :1;    /* Send next Data as Break */
          uint8_t TXADDR            :1;    /* Send next Data as Address */
          uint8_t DORM              :1;    /* Dormant (Sleep) Mode */  
          uint8_t BRKIE             :1;    /* Break interrupt enable */  
          uint8_t RXEIE             :1;    /* RX Error interrupt enable */  
          //SSEL公用
          uint8_t SSEL              :2;    /* USCI 0 Clock Source Select */
        };
        //SPI
        /*
        struct
        {
          uint8_t :8;
        };
        */
        //I2C
        struct
        {
        uint8_t :1;
        uint8_t TXSTT             :1;    /* Transmit START */
        uint8_t TXSTP             :1;    /* Transmit STOP */
        uint8_t TXNACK            :1;    /* Transmit NACK */
        uint8_t TR                :1;    /* Transmit/Receive Select/Flag */
        uint8_t :1;
        };
      };
      union
      {
        uint8_t CTL0;
        //UART
        struct
        {
          uint8_t SYNC             :1;    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
          uint8_t MODE             :2;    /* Async. Mode: USCI Mode 0 */  
          uint8_t SPB              :1;    /* Async. Mode: Stop Bits  0:one / 1: two */  
          uint8_t UC_7BIT          :1;    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */  
          uint8_t MSB              :1;    /* Async. Mode: MSB first  0:LSB / 1:MSB */
          uint8_t PAR              :1;    /* Async. Mode: Parity     0:odd / 1:even */  
          uint8_t PEN              :1;    /* Async. Mode: Parity enable */
        };
        //SPI
        struct
        {
          uint8_t :3;
          uint8_t MST               :1;    /* Sync. Mode: Master Select */
          uint8_t :2;
          uint8_t CKMODE            :2;
          //uint8_t CKPL              :1;    /* Sync. Mode: Clock Polarity */
          //uint8_t CKPH              :1;    /* Sync. Mode: Clock Phase */        
        };
        //I2C
        struct
        {
        uint8_t :5;
        uint8_t MM                :1;    /* Multi-Master Environment */  
        uint8_t SLA10             :1;    /* 10-bit Slave Address Mode */
        uint8_t OA10              :1;    /* 10-bit Address Mode */
        };
      };
    };
  };
  uint8_t RESERVED_1[4];
  union
  {
    uint16_t BRW;
    struct
    {
      uint8_t BR0;
      uint8_t BR1;
    };
  };
  union
  {
    uint16_t MCTL;
    struct
    {
      uint8_t OS16              :1;    /* USCI 16-times Oversampling enable */
      uint8_t BRS               :3;    /* USCI Second Stage Modulation Select */
      uint8_t BRF               :4;    /* USCI First Stage Modulation Select*/
    };
  };
  union
  {
    uint16_t STAT;
    //AX
    struct
    {
      uint8_t UC_BUSY              :1;    /* USCI Busy Flag */
      uint8_t ADDR              :1;    /* USCI Address received Flag */
      uint8_t RXERR             :1;    /* USCI RX Error Flag */
      uint8_t BRK               :1;    /* USCI Break received */
      uint8_t PE                :1;    /* USCI Parity Error Flag */
      uint8_t OE                :1;    /* USCI Overrun Error Flag */
      uint8_t FE                :1;    /* USCI Frame Error Flag */
      uint8_t LISTEN            :1;    /* USCI Listen mode */   
    };
    //UCBX
    struct
    {
    uint8_t :4;
    uint8_t BBUSY             :1;    /* Bus Busy Flag */
    uint8_t GC                :1;    /* General Call address received Flag */
    uint8_t SCLLOW            :1;    /* SCL low */
    uint8_t :1;          
    };
  };
  
  uint8_t RXBUF;
  uint8_t RESERVED_3;
  uint8_t TXBUF;
  uint8_t RESERVED_4;
  union  //共同占用两个字
  {
    struct       
    {
      uint16_t ABCTL;
      union
      {  
        uint16_t IRCTL;
        struct
        {
          union
          {
            uint8_t IRTCTL;
            struct
            {
              uint8_t IREN              :1;    /* IRDA Encoder/Decoder enable */
              uint8_t IRTXCLK           :1;    /* IRDA Transmit Pulse Clock Select */  
              uint8_t IRTXPL            :6;    // IRDA Transmit Pulse Length 
              /*
              uint8_t IRTXPL0           :1;    // IRDA Transmit Pulse Length 0 
              uint8_t IRTXPL1           :1;    // IRDA Transmit Pulse Length 1  
              uint8_t IRTXPL2           :1;    // IRDA Transmit Pulse Length 2  
              uint8_t IRTXPL3           :1;    // IRDA Transmit Pulse Length 3   
              uint8_t IRTXPL4           :1;    // IRDA Transmit Pulse Length 4   
              uint8_t IRTXPL5           :1;    // IRDA Transmit Pulse Length 5 
              */
            };
          };
          union
          {
            uint8_t IRRCTL;
            struct
            {
              uint8_t IRRXFE            :1;    // IRDA Receive Filter enable 
              uint8_t IRRXPL            :1;    // IRDA Receive Input Polarity 
              uint8_t IRRXFL            :6;    // IRDA Receive Filter Length 
              /*
              uint8_t IRRXFL0           :1;    // IRDA Receive Filter Length 0 
              uint8_t IRRXFL1           :1;    // IRDA Receive Filter Length 1 
              uint8_t IRRXFL2           :1;    // IRDA Receive Filter Length 2 
              uint8_t IRRXFL3           :1;    // IRDA Receive Filter Length 3 
              uint8_t IRRXFL4           :1;    // IRDA Receive Filter Length 4 
              uint8_t IRRXFL5           :1;    // IRDA Receive Filter Length 5 
              */
            };
          };
        };
      };
    }; //USCIA
    struct
    {
      uint16_t I2COA;
      uint16_t I2CSA;
    }; //USCIB
  };
  uint8_t RESERVED_2[8];
  union
  {
    uint16_t ICTL;
    struct
    {
      union
      {
        uint8_t IE;
        struct
        {
          uint8_t RXIE              :1;  /* USCI Receive Interrupt Enable */
          uint8_t TXIE              :1;  /* USCI Transmit Interrupt Enable */
          //UCBX
          uint8_t STTIE             :1;  /* START Condition interrupt enable */ 
          uint8_t STPIE             :1;  /* STOP Condition interrupt enable */
          uint8_t ALIE              :1;  /* Arbitration Lost interrupt enable */
          uint8_t NACKIE            :1;  /* NACK Condition interrupt enable */
        };
      };
      union
      {
        uint8_t IFG;
        struct
        {
          uint8_t RXIFG             :1;  /* USCI Receive Interrupt Flag */
          uint8_t TXIFG             :1;  /* USCI Transmit Interrupt Flag */
          //BX
          uint8_t STTIFG            :1;  /* START Condition interrupt Flag */
          uint8_t STPIFG            :1;  /* STOP Condition interrupt Flag */
          uint8_t ALIFG             :1;  /* Arbitration Lost interrupt Flag */
          uint8_t NACKIFG           :1;  /* NAK Condition interrupt Flag */
        };
      };
    };
  };
  uint16_t IV;
}volatile * USCI_MemMapPtr;

#define USCI_SSEL__UCLK        (0)    /* USCI 0 Clock Source: UCLK */
#define USCI_SSEL__ACLK        (1)    /* USCI 0 Clock Source: ACLK */
#define USCI_SSEL__SMCLK       (2)    /* USCI 0 Clock Source: SMCLK */

#define USCI_A0_BASE_PTR    ((USCI_MemMapPtr)__MSP430_BASEADDRESS_USCI_A0__)
#define USCI_A1_BASE_PTR    ((USCI_MemMapPtr)__MSP430_BASEADDRESS_USCI_A1__)
#define USCI_B0_BASE_PTR    ((USCI_MemMapPtr)__MSP430_BASEADDRESS_USCI_B0__)
#define USCI_B1_BASE_PTR    ((USCI_MemMapPtr)__MSP430_BASEADDRESS_USCI_B1__)

extern const USCI_MemMapPtr  USCIX[];

#ifdef __MSP430_HAS_FLASH__
typedef struct FLASH_MemMap
{
/* FCTL1 Control Bits */
  union 
  {
    uint8_t FCTL1;
    struct
    {
     uint8_t :1; /* Reserved */
     uint8_t FERASE               :1;  /* Enable bit for Flash segment erase */
     uint8_t FMERAS               :1;  /* Enable bit for Flash mass erase */
     uint8_t :2;  /* Reserved */
     uint8_t FSWRT                :1;  /* Smart Write enable */
     uint8_t FWRT                 :1;  /* Enable bit for Flash write */
     uint8_t FBLKWRT              :1;  /* Enable bit for Flash segment write */
    };
  };
  union
  {
    uint8_t FCTL3;
    struct
    {
      uint8_t FBUSY                :1;  /* Flash busy: 1 */
      uint8_t FKEYV                :1;  /* Flash Key violation flag */
      uint8_t FACCVIFG             :1;  /* Flash Access violation flag */
      uint8_t FWAIT                :1;  /* Wait flag for segment write */
      uint8_t FLOCK                :1;  /* Lock bit: 1 - Flash is locked (read only) */
      uint8_t FEMEX                :1;  /* Flash Emergency Exit */
      uint8_t FLOCKA               :1;  /* Segment A Lock bit: read = 1 - Segment is locked (read only) */
    };
  };
 union
  {
    uint8_t FCTL4;
    struct
    {
      uint8_t FVPE                  :1;  /* Voltage Changed during Program Error Flag */
      uint8_t :3;
      uint8_t FMGR0                 :1;  /* Marginal read 0 mode. */
      uint8_t FMGR1                 :1;  /* Marginal read 1 mode. */
      uint8_t :1;
      uint8_t FLOCKINFO             :1;  /* Lock INFO Memory bit: read = 1 - Segment is locked (read only) */
    };
  };
} volatile * FLASH_MemMapPtr;

#define FLASH_BASE_PTR          ((FLASH_MemMapPtr)__MSP430_BASEADDRESS_FLASH__)
#endif

#ifdef __MSP430_HAS_WDT_A__
typedef struct WDT_MemMap
{
  uint8_t RESERVED[11];
  union
  {
    uint16_t CTL;
    struct
    {
      uint16_t IS               :3;  /* WDT - Timer Interval Select */
      uint16_t CNTCL            :1;  /* WDT - Timer Clear */
      uint16_t TMSEL            :1;  /* WDT - Timer Mode Select */
      uint16_t SSEL             :2;  /* WDT - Timer Clock Source Select  */
      uint16_t HOLD             :1;  /* WDT - Timer hold */
      uint16_t PW               :8;  
    };
  };
}volatile * WDT_MemMapPtr;

#define WDT_PASSWARD            (0x5Au)
#define WDT_SSEL_SMCLK          (0)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDT_SSEL_ACLK           (1)  /* WDT - Timer Clock Source Select: ACLK */
#define WDT_SSEL_VLO            (2)  /* WDT - Timer Clock Source Select: VLO_CLK */

#define WDT_BASE_PTR          ((WDT_MemMapPtr)__MSP430_BASEADDRESS_WDT_A__)
#endif

#ifdef __MSP430_HAS_SFR__
typedef struct SFR_MemMap
{
  union
  {
    uint16_t IE1;
    struct
    {
      uint16_t WDT               :1;  /* WDT Interrupt Enable */
      uint16_t OF                :1;  /* Osc Fault Enable */
      uint16_t Reserved          :1;
      uint16_t VMA               :1;  /* Vacant Memory Interrupt Enable */
      uint16_t NMI               :1;  /* NMI Interrupt Enable */
      uint16_t ACCV              :1;  /* Flash Access Violation Interrupt Enable */
      uint16_t JMBIN             :1;  /* JTAG Mail Box input Interrupt Enable */
      uint16_t JMBOUT            :1;  /* JTAG Mail Box output Interrupt Enable */
    }IE;
  };
  union 
  {
    uint16_t IFG1;
    struct
    {
      uint16_t WDT              :1;  /* WDT Interrupt Flag */
      uint16_t OF               :1;  /* Osc Fault Flag */
      uint16_t Reserved1        :1;
      uint16_t VMA              :1;  /* Vacant Memory Interrupt Flag */
      uint16_t NMI              :1;  /* NMI Interrupt Flag */
      uint16_t Reserved2        :1;
      uint16_t JMBIN            :1;  /* JTAG Mail Box input Interrupt Flag */
      uint16_t JMBOUT           :1;  /* JTAG Mail Box output Interrupt Flag */
    }IFG;
  };
  union
  {
    uint16_t RPCR;
    struct
    {
      uint16_t NMI              :1;  /* NMI select */
      uint16_t NMIIES           :1;  /* NMI edge select */
      uint16_t RSTUP            :1;  /* RESET Pin pull down/up select */
      uint16_t RSTRE            :1;  /* RESET Pin Resistor enable */
    }SYS;
  };
}volatile * SFR_MemMapPtr;

#define SFR_BASE_PTR          ((SFR_MemMapPtr)__MSP430_BASEADDRESS_SFR__)
#endif

#ifdef __MSP430_HAS_SYS__
typedef struct SYS_MemMap
{
  union
  {
    uint16_t CTL;
    struct
    {
      uint16_t RIVECT              :1;  /* SYS - RAM based interrupt vectors */
      uint16_t                     :1;  /* SYS - Reserved */
      uint16_t PMMPE               :1;  /* SYS - PMM access protect */
      uint16_t                     :1;  /* SYS - Reserved */
      uint16_t BSLIND              :1;  /* SYS - TCK/RST indication detected */
      uint16_t JTAGPIN             :1;  /* SYS - Dedicated JTAG pins enabled */
      uint16_t                     :10; /* SYS - Reserved */
    };
  };
  union
  {
    uint16_t BSLC;
    struct
    {
      uint16_t SIZE                :2;  /* SYS - BSL Protection Size 0 */
      uint16_t R                   :1;  /* SYS - RAM assigned to BSL */
      uint16_t                     :11;  /* SYS - Reserved */
      uint16_t OFF                 :1;  /* SYS - BSL Memeory disabled */
      uint16_t PE                  :1;  /* SYS - BSL Memory protection enabled */
    }BSL;
  };
  uint16_t RESERVED1;
  union
  {
    uint16_t JMBC;
    struct
    {
      uint16_t IN0FG               :1;  /* SYS - Incoming JTAG Mailbox 0 Flag */
      uint16_t IN1FG               :1;  /* SYS - Incoming JTAG Mailbox 1 Flag */
      uint16_t OUT0FG              :1;  /* SYS - Outgoing JTAG Mailbox 0 Flag */
      uint16_t OUT1FG              :1;  /* SYS - Outgoing JTAG Mailbox 1 Flag */
      uint16_t MODE                :1;  /* SYS - JMB 16/32 Bit Mode */
      uint16_t                     :1;  /* SYS - Reserved */
      uint16_t CLR0OFF             :1;  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
      uint16_t CLR1OFF             :1;  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
      uint16_t                     :8;  /* SYS - Reserved */
    }JMB;
  };
  uint16_t JMBI[2];
  uint16_t JMBO[2];
  uint16_t RESERVED2[4];
  uint16_t ERRIV;                       /* Bus Error vector generator */
  uint16_t UNIV;                        /* User NMI vector generator */
  uint16_t SNIV;                        /* System NMI vector generator */
  uint16_t RSTIV;                       /* Reset vector generator */
}volatile * SYS_MemMapPtr;
#endif

#define SYS_BASE_PTR          ((SYS_MemMapPtr)__MSP430_BASEADDRESS_SYS__)

#endif


