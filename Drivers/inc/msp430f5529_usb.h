#ifndef USB_H_
#define USB_H_

#include"common.h"

void    USB_Init();                         // USB ģ���ʼ��

void    USB_com_init(void);                     // USB ���⴮�� ��ʼ��
void    USB_enum_wait(void);                    // USB �ȴ�ö��
uint8   USB_com_rx(uint8 *rx_buf);              // USB ���⴮�� ����
void    USB_com_tx(uint8 *tx_buf, uint8 len);   // USB ���⴮�� ����

struct usb_driver  //��������һ��USB����
{
  const char * name;            /*����������*/
  /*��USB���ķ����˸������ܹ������USB�ӿ�ʱ�����øú���*/
  int (*probe)(struct usb_interface *intf,const struct usb_device_id * id);
  /*����Ӧ��USB�ӿڱ��Ƴ�ʱ�����øó���*/
  void (*disconnect)(struct usb_interface *intf);
  /*USB�����ܹ�������豸�б�*/ 
  const struct usb_device_id * idtable;
};
struct usb_device_id
{
  uint16 match_flags;
  
  /*Used for product specific matches;range is inclusive*/
  uint16 idVendor;   //������ID
  uint16 idProduct;  //��ƷID
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
  int devnum;      /*USB�豸��*/
  char devpath[16];  /*�豸�ɣ��ַ���*/
  
};
#endif