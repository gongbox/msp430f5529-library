#ifndef USB_H_
#define USB_H_

#include"common.h"

void    USB_Init();                         // USB 模块初始化

void    USB_com_init(void);                     // USB 虚拟串口 初始化
void    USB_enum_wait(void);                    // USB 等待枚举
uint8   USB_com_rx(uint8 *rx_buf);              // USB 虚拟串口 接收
void    USB_com_tx(uint8 *tx_buf, uint8 len);   // USB 虚拟串口 发送

struct usb_driver  //用来描述一个USB驱动
{
  const char * name;            /*驱动程序名*/
  /*当USB核心发现了该驱动能够处理的USB接口时，调用该函数*/
  int (*probe)(struct usb_interface *intf,const struct usb_device_id * id);
  /*当相应的USB接口被移除时，调用该程序*/
  void (*disconnect)(struct usb_interface *intf);
  /*USB驱动能够处理的设备列表*/ 
  const struct usb_device_id * idtable;
};
struct usb_device_id
{
  uint16 match_flags;
  
  /*Used for product specific matches;range is inclusive*/
  uint16 idVendor;   //制造商ID
  uint16 idProduct;  //产品ID
  uint16 bcdDevice_lo;
  uint16 bcdDevice_hi;
  
  /*Used for device class matches*/
  uint8 bDeviceClass;
  uint8 bDeviceSubClass;
  uint8 bDeviceProtocol;
  
  /*Used for interface class matches*/
  uint8 bInterfaceClass;
  uint8 bInterfaceSubClass;
  uint8 bInterfaceProtocol;
  
  /*not matched against*/
  Kernel_ulong_t driver_info;
};
struct usb_device
{
  int devnum;      /*USB设备号*/
  char devpath[16];  /*设备ＩＤ字符串*/
  
};
#endif