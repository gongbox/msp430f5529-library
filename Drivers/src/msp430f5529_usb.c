#include "msp430f5529_usb.h"
#include "msp430f5529.h"
#include "msp430f5529_clock.h"
#include "system.h"
/*
参考资料：http://www.doc88.com/p-7098075309005.html 
http://www.doc88.com/p-5384122814633.html

*/
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/  
void    USB_Init(void)
{
    uint16 bGIE  = __get_SR_register() &GIE;  //save interrupt status
    // atomic operation - disable interrupts
    __disable_interrupt();               //禁止全局中断
    
    // configuration of USB module
    USBKEYPID   =     USBKEY;            // 允许寄存器修改
    //引脚配置
    USBPHYCTL   =     PUSEL;             // use DP and DM as USB terminals (not needed because an external PHY is connected to port 9)

    USBPWRCTL   =  VUSBEN + SLDOAON ; // enable primary and secondary LDO (3.3 and 1.8 V)
    //时钟配置
    //存在XT2时必须使用XT2作为时钟源，使用PLL产生48Mhz的时钟，不存在XT2时使用XT1作为时钟源
    DELAY_MS(2);    //延时2MS
    USBPLLCTL = UPCS0 + UCLKSEL__PLLCLK + UPLLEN ;   
    //使用XT2作为时钟源，使能PLL，并使用PLL作为时钟
    USBPLLDIVB = USBPLL_SETCLK_4_0;            //XT2输入时钟为4Mhz
    
    USBPWRCTL   =   VUSBEN + SLDOAON + VBONIE;  // enable interrupt VBUSon
    USBKEYPID   =    0x9600;            // 禁止寄存器修改

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

    __bis_SR_register(bGIE); //恢复全局中断使能位
    //return kUSB_succeed;
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/  
 // USB 虚拟串口 初始化
void    USB_com_init(void)
{
  
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/  
// USB 等待枚举
void    USB_enum_wait(void)
{
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/  
// USB 虚拟串口 接收
uint8   USB_com_rx(uint8 *rx_buf)
{
}
/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：
*  函数返回：
*  使用示例：
*************************************************************************/  
// USB 虚拟串口 发送
void    USB_com_tx(uint8 *tx_buf, uint8 len)
{
}
