#include "msp430f5529_usb.h"
#include "msp430f5529.h"
#include "msp430f5529_clock.h"
#include "system.h"
/*
�ο����ϣ�http://www.doc88.com/p-7098075309005.html 
http://www.doc88.com/p-5384122814633.html

*/
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/  
void    USB_Init(void)
{
    uint16 bGIE  = __get_SR_register() &GIE;  //save interrupt status
    // atomic operation - disable interrupts
    __disable_interrupt();               //��ֹȫ���ж�
    
    // configuration of USB module
    USBKEYPID   =     USBKEY;            // ����Ĵ����޸�
    //��������
    USBPHYCTL   =     PUSEL;             // use DP and DM as USB terminals (not needed because an external PHY is connected to port 9)

    USBPWRCTL   =  VUSBEN + SLDOAON ; // enable primary and secondary LDO (3.3 and 1.8 V)
    //ʱ������
    //����XT2ʱ����ʹ��XT2��Ϊʱ��Դ��ʹ��PLL����48Mhz��ʱ�ӣ�������XT2ʱʹ��XT1��Ϊʱ��Դ
    DELAY_MS(2);    //��ʱ2MS
    USBPLLCTL = UPCS0 + UCLKSEL__PLLCLK + UPLLEN ;   
    //ʹ��XT2��Ϊʱ��Դ��ʹ��PLL����ʹ��PLL��Ϊʱ��
    USBPLLDIVB = USBPLL_SETCLK_4_0;            //XT2����ʱ��Ϊ4Mhz
    
    USBPWRCTL   =   VUSBEN + SLDOAON + VBONIE;  // enable interrupt VBUSon
    USBKEYPID   =    0x9600;            // ��ֹ�Ĵ����޸�

    //reset events mask
    //wUsbEventMask = 0;

    //init Serial Number
#if (USB_STR_INDEX_SERNUM != 0)
    USB_InitSerialStringDescriptor();
#endif

    // init memcpy() function: DMA or non-DMA
    USB_initMemcpy();
#ifdef _MSC_
    MscResetCtrlLun();
#endif

    __bis_SR_register(bGIE); //�ָ�ȫ���ж�ʹ��λ
    //return kUSB_succeed;
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/  
 // USB ���⴮�� ��ʼ��
void    USB_com_init(void)
{
  
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/  
// USB �ȴ�ö��
void    USB_enum_wait(void)
{
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/  
// USB ���⴮�� ����
uint8   USB_com_rx(uint8 *rx_buf)
{
}
/*************************************************************************
*  �������ƣ�
*  ����˵����
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
*************************************************************************/  
// USB ���⴮�� ����
void    USB_com_tx(uint8 *tx_buf, uint8 len)
{
}
