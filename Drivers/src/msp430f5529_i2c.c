#include "msp430f5529_i2c.h"
#include "msp430f5529_clock.h" 
#include "msp430f5529_system.h"
#include "msp430f5529_gpio.h"

const struct{
  GPIO_PIN SCL;
  GPIO_PIN SDA;
}I2C_PIN[]=
{ //        SCL         SDA
  /*I2C0*/{.SCL={P3,1},.SDA={P3,0}},           
  /*I2C1*/{.SCL={P4,2},.SDA={P4,1}}          
};
static inline void  I2C_Start     (I2Cn i2cn);          //����һ����ʼ�ź�
static inline void  I2C_Stop      (I2Cn i2cn);          //����һ��ֹͣ�ź�
static inline void  I2C_WaitBusy  (I2Cn i2cn);          //�ȴ�����
static inline void  I2C_EnterSend (I2Cn i2cn);          //���뷢��ģʽ
static inline void  I2C_EnterRead (I2Cn i2cn);          //�������ģʽ
/*******************************************************************************
*  �������ƣ�I2C_MasterInit(I2Cn i2cn,uint16_t SlaveID,uint32_t BaudRate) 
*  ����˵������ʼ��һ��ģ��ΪI2C����ģ�飬�����ôӻ���ַ�����䲨����
*  ����˵����I2Cn i2cn      :ģ���
             uint8_t SlaveID  :�ӻ���ַ
             uint32_t BaudRate:���䲨����
*  �������أ���
*  ʹ��ʾ����I2C_MasterInit   (I2C0, 0x68,100000);  //��ʼ��I2C0ģ��Ϊ�������ӻ���ַΪ0x68,���ô��䲨����Ϊ100000
********************************************************************************/
void  I2C_MasterInit(I2Cn i2cn,uint16_t SlaveID,uint32_t BaudRate)                                       
{
  WordType BR;
  BR.Word=g_sClock.SMCLK.nHZ/BaudRate;        //��ȡ����������ķ�Ƶϵ��
  GPIO_Init(I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,GPO);
  //���9��ʱ���Իָ�I2Cn����״̬  
  for(uint8_t i=0;i<9;i++)
  {
    GPIO_WriteBit  (I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,  BIT_SET);
    DELAY_US(5);
    GPIO_WriteBit  (I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,RESET);
    DELAY_US(5);
  }
  //��ʼ������
  GPIO_Init(I2C_PIN[i2cn-I2C0].SCL.Port,I2C_PIN[i2cn-I2C0].SCL.Pin,SEL);
  GPIO_Init(I2C_PIN[i2cn-I2C0].SDA.Port,I2C_PIN[i2cn-I2C0].SDA.Pin,SEL);
  //��ʼ���Ĵ���
  USCIX[i2cn]->CTL1 = UCSWRST;                       // �����λʹ�ܣ����ָ�λ״̬
  USCIX[i2cn]->CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C������ͬ��ģʽ
  if(SlaveID > 0x7F)
  {
    USCIX[i2cn]->SLA10 = BIT_SET;
  }
  else
  {
    USCIX[i2cn]->SLA10 = RESET;
  }
  USCIX[i2cn]->I2CSA = SlaveID;
  USCIX[i2cn]->CTL1 |= UCSSEL__SMCLK;                 //ʹ��SMCLK��Ϊʱ��Դ
  USCIX[i2cn]->BR0 = BR.Byte[0];
  USCIX[i2cn]->BR1 = BR.Byte[1];                        
  USCIX[i2cn]->CTL1 &=~ UCSWRST;                      //��������λ����������
  
  USCIX[i2cn]->IE = 0u;                                         //�ر������ж�
  USCIX[i2cn]->RXIFG = RESET;                                   //����������ݱ�־
  USCIX[i2cn]->TXIFG = BIT_SET;                                     //��λ���ͻ�����Ϊ�ձ�־ 
}
/*******************************************************************************
*  �������ƣ�I2C_SetSlaveID(I2Cn i2cn,uint16_t SlaveID)
*  ����˵�������ôӻ���ַ
*  ����˵���� I2Cn i2cn      :ģ���
              uint8_t SlaveID  :�ӻ���ַ
*  �������أ���
*  ʹ��ʾ����I2C_SetSlaveID(I2C1,0x68);   //����I2C1ģ��Ĵӻ���ַΪ0x68
********************************************************************************/
void I2C_SetSlaveID(I2Cn i2cn,uint16_t SlaveID)
{
  if(SlaveID > 0x7F)
  {
    USCIX[i2cn]->SLA10 = BIT_SET;
  }
  else
  {
    USCIX[i2cn]->SLA10 = RESET;
  }
  USCIX[i2cn]->I2CSA = SlaveID;
}
/*******************************************************************************
*  �������ƣ�I2C_Start(I2Cn i2cn)
*  ����˵��������һ����ʼ�ź�
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���
*  ʹ��ʾ����I2C_Start (I2C1);  //I2C1ģ�鷢��һ����ʼ�ź�
********************************************************************************/
inline void I2C_Start(I2Cn i2cn)
{
  while((USCIX[i2cn]->UC_BUSY==BIT_SET) || (USCIX[i2cn]->TXSTP==BIT_SET));//ȷ�����߿���
  USCIX[i2cn]->TXSTT = BIT_SET;
}
/*******************************************************************************
*  �������ƣ�I2C_Stop(I2Cn i2cn)
*  ����˵��������һ��ֹͣ�ź�
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���
*  ʹ��ʾ����I2C_Stop (I2C1);  //I2C1ģ�鷢��һ��ֹͣ�ź�
********************************************************************************/
inline void I2C_Stop(I2Cn i2cn)
{
  while(USCIX[i2cn]->UC_BUSY == BIT_SET);                 //�ȴ�����
  USCIX[i2cn]->TXSTP =BIT_SET;                            //����ֹͣλ
  while(USCIX[i2cn]->TXSTP == BIT_SET);                   //�ȴ�ֹͣλ�������
}
/*******************************************************************************
*  �������ƣ�I2C_EnterSend (I2Cn i2cn)
*  ����˵����I2C���뷢��ģʽ
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���
*  ʹ��ʾ����I2C_EnterSend (I2C1);  //I2C1ģ�����дģʽ
********************************************************************************/
inline void I2C_EnterSend (I2Cn i2cn)
{
  USCIX[i2cn]->TR = BIT_SET;                              //���뷢��ģʽ
}
/*******************************************************************************
*  �������ƣ�I2C_EnterRead (I2Cn i2cn)
*  ����˵����I2C�������ģʽ
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���
*  ʹ��ʾ����I2C_EnterRead (I2C1);  //I2C1ģ������ģʽ
********************************************************************************/
inline void I2C_EnterRead (I2Cn i2cn)
{
  USCIX[i2cn]->TR = RESET;                             //�������ģʽ
}
/*******************************************************************************
*  �������ƣ�I2C_SendByte (I2Cn i2cn,uint8_t data)
*  ����˵����I2C����һ�ֽ�����
*  ����˵����I2Cn i2cn      :ģ���
             uint8_t data     :Ҫ���͵�����
*  �������أ���
*  ʹ��ʾ����I2C_SendByte (I2C1,0x01);  //I2C1ģ�鷢��һ�ֽ�����0x01
********************************************************************************/
inline void I2C_SendByte (I2Cn i2cn,uint8_t data)
{
  while(USCIX[i2cn]->TXIFG == RESET);                   //�ȴ�
  USCIX[i2cn]->TXBUF = data;                            //����Ҫд�������
}
/*******************************************************************************
*  �������ƣ�I2C_ReadByte  (I2Cn i2cn)
*  ����˵����IIC��ȡһ���ֽ�����
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���ȡ��������
*  ʹ��ʾ����uint8 data = I2C_ReadByte  (I2C1);   //��ȡ����
********************************************************************************/
inline uint8_t I2C_ReadByte  (I2Cn i2cn)
{
  while(USCIX[i2cn]->RXIFG == RESET);                 //�ȴ����յ�����
  return USCIX[i2cn]->RXBUF;
}
/*******************************************************************************
*  �������ƣ�I2C_WaitBusy(I2Cn i2cn)
*  ����˵����I2C�ȴ�����
*  ����˵����I2Cn i2cn      :ģ���
*  �������أ���
*  ʹ��ʾ����I2C_WaitBusy(I2C1);  //�ȴ�I2C1ģ�鲻æ
********************************************************************************/
inline void I2C_WaitBusy(I2Cn i2cn)
{
  while(USCIX[i2cn]->UC_BUSY == BIT_SET);                    //�ȴ����ͻ�������
}
/*******************************************************************************
*  �������ƣ� I2C_WriteReg(I2Cn i2cn, uint8_t address, char data)
*  ����˵���� ��ĳһ����ַд��һ�ֽ�����
*  ����˵���� I2Cn i2cn      :ģ���
              uint8_t address :�Ĵ�����ַ
              uint8_t data    :�Ըõ�ַҪд�����������
*  �������أ���
*  ʹ��ʾ����I2C_WriteReg(I2C1, 0x20, 0x12);   //�Ե�ַΪ0x20��д������0x12
********************************************************************************/
void  I2C_WriteReg(I2Cn i2cn, uint8_t address, uint8_t data)     
{
  I2C_EnterSend(i2cn);
  I2C_Start    (i2cn);                        //����һ����ʼ�ź�
  I2C_SendByte (i2cn,address);                //����Ҫд��ĵ�ַ
  I2C_SendByte (i2cn,data);                   //����Ҫд�������
  I2C_Stop     (i2cn);                        //����ֹͣλ
  I2C_WaitBusy (i2cn);
}
/*******************************************************************************
*  �������ƣ� I2C_ReadReg(I2Cn i2cn, uint8_t address) 
*  ����˵���� ���ⲿоƬ��ȡĳһ��ַ������
*  ����˵���� I2Cn i2cn      :ģ���
              uint8_t address :�Ĵ�����ַ
*  �������أ� ��ȡ��������
*  ʹ��ʾ���� uint8_t data = I2C_ReadReg(I2C1, 0x20);  //��ȡ�Ĵ�����ַΪ0x20��������
********************************************************************************/
uint8_t    I2C_ReadReg(I2Cn i2cn, uint8_t address)             
{
  I2C_EnterSend (i2cn);                       //���뷢��ģʽ
  I2C_Start     (i2cn);                       //����һ����ʼ�ź�
  I2C_SendByte  (i2cn,address);               //����һ�ֽ�����
  I2C_WaitBusy  (i2cn);                       //�ȴ��������
  I2C_EnterRead (i2cn);                       //�������ģʽ
  I2C_Start     (i2cn);                       //����һ����ʼ�ź�
  I2C_WaitBusy  (i2cn);                       //�ȴ�����
  I2C_Stop      (i2cn);                       //����һ��ֹͣ�ź�,���Ļ�Ҫ�ȷ���ֹͣλ
  return I2C_ReadByte(i2cn);                  //��ȡ����
}
/*******************************************************************************
*  �������ƣ� I2C_ITConfig (I2Cn i2cn,I2C_IRQn irqn,STATUS ITState)
*  ����˵���� ����ʹ�ܻ��ֹI2Cn��ĳһ���ж�
*  ����˵���� I2Cn i2cn      :ģ���
              I2C_IRQn irqn :�ж�����
*  �������أ���
*  ʹ��ʾ����I2C_ITConfig (I2C1,I2C_RX_IRQn,ENABLE);  //ʹ��I2C1�Ľ����ж�
********************************************************************************/
void  I2C_ITConfig (I2Cn i2cn,I2C_IRQn irqn,STATUS ITState)
{
  if(ITState != DISABLE)
  {
    USCIX[i2cn]->IE |= irqn;
  }
  else
  {
    USCIX[i2cn]->IE &=~irqn;
  }
}
/*******************************************************************************
*  �������ƣ� I2C_GetITStatus(I2Cn i2cn,I2C_IRQn irqn)
*  ����˵���� ��ȡI2C��ĳһ���жϱ�־
*  ����˵���� I2Cn i2cn      :ģ���
              I2C_IRQn irqn :�ж�����
*  �������أ� STATUS : TRUE �ж��¼�������FALSE �ж��¼�δ����
*  ʹ��ʾ���� if(TRUE == I2C_GetITStatus(I2C0,I2C_RX_IRQn)){...} //�ж�I2C0ģ���Ƿ��������¼��жϷ���
********************************************************************************/
STATUS I2C_GetITStatus(I2Cn i2cn,I2C_IRQn irqn)
{
  return ((USCIX[i2cn]->IFG & irqn) ? TRUE :FALSE);
}
/*******************************************************************************
*  �������ƣ� I2C_ClearITPendingBit(I2Cn i2cn,I2C_IRQn irqn)
*  ����˵���� ���I2Cn��ĳһ���жϱ�־
*  ����˵���� I2Cn i2cn      :ģ���
              I2C_IRQn irqn :�ж�����
*  �������أ� ��
*  ʹ��ʾ���� I2C_ClearITPendingBit(I2C0,I2C_RX_IRQn);   //���I2C1ģ������жϱ�־λ
********************************************************************************/
void  I2C_ClearITPendingBit(I2Cn i2cn,I2C_IRQn irqn)
{
  USCIX[i2cn]->IFG &=~ irqn;
}