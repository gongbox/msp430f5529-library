#include "msp430f5529_spi.h"
#include "msp430f5529_clock.h"
#include "msp430f5529_gpio.h"

const USCI_MemMapPtr USCIX[]={USCI_A0_BASE_PTR,USCI_A1_BASE_PTR,USCI_B0_BASE_PTR,USCI_B1_BASE_PTR};

const struct
{
  GPIO_PIN STE_PIN ; //
  GPIO_PIN SIMO_PIN; //
  GPIO_PIN SOMI_PIN; //
  GPIO_PIN CLK_PIN ; //
}SPI_PIN[]=
{ //        STE             SIMO             SOMI             CLK
  /*SPI0*/{.STE_PIN={P3,2},.SIMO_PIN={P3,3},.SOMI_PIN={P3,4},.CLK_PIN={P2,7}},    
  /*SPI1*/{.STE_PIN={P4,3},.SIMO_PIN={P4,4},.SOMI_PIN={P4,5},.CLK_PIN={P4,0}},       
  /*SPI2*/{.STE_PIN={P2,1},.SIMO_PIN={P3,0},.SOMI_PIN={P3,1},.CLK_PIN={P3,2}},    
  /*SPI3*/{.STE_PIN={P4,0},.SIMO_PIN={P4,1},.SOMI_PIN={P4,2},.CLK_PIN={P4,3}},  
};//���ų�ʼ������
/*******************************************************************************
*  �������ƣ�SPI_Master_Init (SPIn spin,uint32_t BaudRate,FirstBit firstbit, SPI_STE_Mode spistemode,SPI_CLK_Mode spiclkmode)
*  ����˵����SPI��ʼ��
*  ����˵����SPIn spin                  :SPIģ���
             uint32_t BaudRate          :���䲨����
             SPI_FirstBit firstbit      :�ȷ��͸�λ���λ����
             SPI_STE_Mode spistemode    :STEģʽ
             SPI_CLK_Mode spiclkmode    :SPIʱ��ģʽ
*  �������أ���
*  ʹ�����̣�SPI_Master_Init(SPI2,10000000,MSB_First,SPI_STE_None,SPI_CLK_Mode_4); //��ʼ��SPIģ�飬������Ϊ10000000���ȷ��͸�λ���ݣ�STE�ź����Σ�ʱ����ģʽ4
********************************************************************************/
void SPI_Master_Init (SPIn spin,uint32_t BaudRate,SPI_FirstBit firstbit, SPI_STE_Mode spistemode,SPI_CLK_Mode spiclkmode)
{
  WordType BR;
  BR.Word = g_sClock.SMCLK.nHZ/BaudRate;        //��ȡ����������ķ�Ƶϵ��
  if(spistemode != SPI_STE_None)
  {
    GPIO_Init(SPI_PIN[spin].STE_PIN.Port,SPI_PIN[spin].STE_PIN.Pin,SEL);     //Ƭѡ�ź�������Ϊ�ڶ�����
  }
  GPIO_Init(SPI_PIN[spin].SIMO_PIN.Port,SPI_PIN[spin].SIMO_PIN.Pin,SEL);     //������������ź�������Ϊ�ڶ�����
  GPIO_Init(SPI_PIN[spin].SOMI_PIN.Port,SPI_PIN[spin].SOMI_PIN.Pin,SEL);     //�����������ź�������Ϊ�ڶ�����
  GPIO_Init(SPI_PIN[spin].CLK_PIN.Port,SPI_PIN[spin].CLK_PIN.Pin,SEL);       //ʱ���ź�������Ϊ�ڶ�����
  
  USCIX[spin]->CTL1 = UCSWRST; 
  USCIX[spin]->SYNC = BIT_SET;                             //ͬ��ģʽ
  USCIX[spin]->MST = BIT_SET;                              //����ģʽ
  USCIX[spin]->MODE = spistemode;                      //Ƭѡ�ź�����
  USCIX[spin]->CKMODE = spiclkmode;                    //ʱ����ģʽ����
  USCIX[spin]->MSB = firstbit;                         //������λ����
  
  USCIX[spin]->SSEL= USCI_SSEL__SMCLK;                 //ʱ��Դ����
  
  USCIX[spin]->BRW = BR.Word;                          //BRWд1��д0���ֲ�����û������
  USCIX[spin]->CTL1 &=~ UCSWRST; 
  
  USCIX[spin]->IE = 0u;                                         //�ر������ж�
  USCIX[spin]->RXIFG = RESET;                                   //����������ݱ�־
  USCIX[spin]->TXIFG = BIT_SET;                                     //��λ���ͻ�����Ϊ�ձ�־
}
/*******************************************************************************
�������ƣ�SPI_SetBaudRate (SPIn spin, uint32_t BaudRate)     
����˵��������SPI������
����˵����SPIn spin                  :SPIģ���
          uint32_t BaudRate            :���䲨����
�������أ���
*******************************************************************************/
void  SPI_SetBaudRate (SPIn spin, uint32_t BaudRate)                  
{
  USCIX[spin]->CTL1 = UCSSEL__SMCLK + UCSWRST;                //����ʱ��Դ
 
  USCIX[spin]->BRW =(uint16_t)g_sClock.SMCLK.nHZ/BaudRate;                                 //���ò�����

  USCIX[spin]->CTL1 &= ~UCSWRST;      
}
/*******************************************************************************
*  �������ƣ�SPI_SendReadByte(SPIn spin,uint8_t data)
*  ����˵����SPI���Ͳ�����һ�ֽ����ݣ����͵�ͬʱһ�����������
*  ����˵����SPIn spin :SPIģ���
             uint8_t data:Ҫ���͵�����
*  �������أ���ȡ��������
*  ʹ��ʾ����SPI_SendReadByte(SPIn spin,uint8_t data)
*  ע�⣺���ṩƬѡ�źţ���Ҫ�Լ�ʹ��IO��ģ��CSƬѡ�ź�
********************************************************************************/
uint8_t SPI_SendReadByte(SPIn spin,uint8_t data)
{
  while(USCIX[spin]->UC_BUSY == BIT_SET);                   //�ж�SPInӲ���Ƿ�æ
  USCIX[spin]->TXBUF = data;                            // д��Ҫ���͵�����
  while(USCIX[spin]->UC_BUSY == BIT_SET);                   //�ж�SPInӲ���Ƿ�æ
  return USCIX[spin]->RXBUF;                            //���ؽ��յ�������,��ȡRXBUF��RXIFGλ�Զ����
}
/*******************************************************************************
*  �������ƣ�SPI_SendReadString(SPIn spin,char * SendData,char * ReceiveData,uint16_t Length)
*  ����˵����SPIn���Ͳ����ն��ֽ�����
*  ����˵����SPIn spin          :SPIģ���
             char * SendData    :�������ݵ�ַ
             char * ReceiveData :�������ݻ����ַ
             uint16_t Length      :Ҫ���������ֽ���
*  �������أ���
*  ʹ��ʾ����char SendData[20] = "Hello!";
             char ReceiveData[20];
             SPI_SendReadString(SPI0,SendData,ReceiveData,strlen("Hello!")); //SPI0�����ַ���
*  ע�⣺���ṩƬѡ�źţ���Ҫ�Լ�ʹ��IO��ģ��CSƬѡ�ź�
********************************************************************************/
void SPI_SendReadString(SPIn spin,char * SendData,char * ReceiveData,uint16_t Length)
{
  while(Length--)
  {
    *ReceiveData++=SPI_SendReadByte(spin, *SendData++);
  }
}
/*******************************************************************************
*  �������ƣ�SPI_ITConfig    (SPIn spin,SPI_IRQn irqn,STATUS ITState)
*  ����˵��������SPIĳһ�ж��Ƿ�ʹ��
*  ����˵����SPIn spin          :SPIģ���
             SPI_IRQn irqn      :�ж��¼�ѡ��
             STATUS ITState     :�ж��Ƿ�ʹ��
*  �������أ���
*  ʹ��ʾ����SPI_ITConfig    (SPI1,SPI_RX_IRQn,ENABLE);  //ʹ��SPI1�Ľ����ж�
********************************************************************************/
void  SPI_ITConfig    (SPIn spin,SPI_IRQn irqn,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    USCIX[spin]->IE |= irqn;
  }
  else
  {
    USCIX[spin]->IE &=~irqn;
  }
}
/*******************************************************************************
*  �������ƣ�SPI_GetITStatus(SPIn spin,SPI_IRQn irqn)
*  ����˵������ȡSPI�жϱ�־λ״̬
*  ����˵����SPIn spin          :SPIģ���
             SPI_IRQn irqn      :�ж��¼�ѡ��
*  �������أ�STATUS :TRUE-�ж��¼����� FALSE-�ж��¼�δ����
*  ʹ��ʾ����if(TRUE == SPI_GetITStatus(SPI1,SPI_RX_IRQn)){...}  //�ж�SPI1�����ж��¼��Ƿ���
********************************************************************************/
STATUS  SPI_GetITStatus(SPIn spin,SPI_IRQn irqn)
{
  return ((USCIX[spin]->IFG & irqn) ? TRUE : FALSE);
}
/*******************************************************************************
*  �������ƣ�SPI_ClearITPendingBit(SPIn spin,SPI_IRQn irqn)
*  ����˵����SPIn����жϱ�־λ״̬
*  ����˵����SPIn spin          :SPIģ���
             SPI_IRQn irqn      :�ж��¼�ѡ��
*  �������أ���
*  ʹ��ʾ����SPI_ClearITPendingBit(SPI1,SPI_RX_IRQn);  //���SPI1�����жϱ�־
********************************************************************************/
void   SPI_ClearITPendingBit(SPIn spin,SPI_IRQn irqn)
{
   USCIX[spin]->IFG &=~ irqn;
}