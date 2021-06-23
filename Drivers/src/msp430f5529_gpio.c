#include "msp430f5529_gpio.h"

//port��pin,BITs��ȡ�궨��,�ɴ˿�֪����ʹ��16λIO����ʱ���ڼ���port,pinֵ�ή��ִ��Ч��
#ifdef GPIO_PORT_SIZE_WORD
#define GET_PORT(portn)        ((((portn) >= PORTA)&&((portn) <= PORTD)) ? ((PORTn)((portn) - PORTA)) : ((PORTn)((portn - PORT1) / 2)))
#define GET_PIN(portn,pin)     ((((portn) >= PORTA)&&((portn) <= PORTD)) ? (pin)  : ((((portn - PORT1)%2) == 0) ? (pin)  : ((pin)+8u)))
#define GET_BITs(portn,BITs)   ((((portn) >= PORTA)&&((portn) <= PORTD)) ? (BITs) : ((((portn - PORT1)%2) == 0) ? (BITs) : (((BITs)<<8u))))
#else
#define GET_PORT(portn)        (portn)
#define GET_PIN(portn,pin)     (pin)
#define GET_BITs(portn,BITs)   (BITs)
#endif

#ifdef GPIO_PORT_SIZE_WORD
const GPIO_MemMapPtr GPIOX[] ={PA_BASE_PTR,PB_BASE_PTR,PC_BASE_PTR,PD_BASE_PTR}; 
#else
const GPIO_MemMapPtr GPIOX[] ={P1_BASE_PTR,P2_BASE_PTR,P3_BASE_PTR,P4_BASE_PTR,P5_BASE_PTR,P6_BASE_PTR,P7_BASE_PTR,P8_BASE_PTR}; 
#endif
/*********************************************************************************
��������IO�ڳ�ʼ��
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint16_t pin   ��ӦIO����λ��
          uint16_t config     
          ��ӦIO������ {
                            GPI                   ,// ���뷽��
                            GPO                   ,// �������
                            LDS                   ,// ����������
                            HDS                   ,// ����������
                            PULL_DOWN             ,// ����
                            PULL_UP               ,// ����
                            IRQ_RISING            ,// �����ش����ж�
                            IRQ_FALLING           ,// �½��ش����ж�
                            SEL                   ,// �ڶ����ù���
                        }  
ʹ������:GPIO_MultiBits_Init(P2,BIT3,GPO|HDS);   //��ʼ��P2.3Ϊ������򣬸���������
**********************************************************************************/
void GPIO_MultiBits_Init(PORTn portn,uint16_t BITs,uint16_t config)
{
  //��ȡport,pin
  PORTn    port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  
  //����IO��
  if((config & SEL) == SEL)
  {
    GPIOX[port]->SEL |= BITs;                    //ѡ��Ϊ�ڶ�����
  }
  else
  {
    GPIOX[port]->SEL &=~BITs;                    //ѡ��ΪIO�ڹ���
  }

  if((config & GPO) == GPO)              //�жϷ���
  {
    GPIOX[port]->PDIR |= BITs;       //����Ϊ�������
    
    if((config&HDS) == HDS)            //�ж���������
    {
      GPIOX[port]->DS |= BITs;          //��Ϊ����������
    }
    else
    {
      GPIOX[port]->DS &=~BITs;          //��Ϊ����������
    }
  }
  else   //��������뷽��
  {
    GPIOX[port]->PDIR &=~BITs;      //����Ϊ���뷽��
    
    if((config & PULL_DOWN) == PULL_DOWN) //�ж��Ƿ�������
    {
      GPIOX[port]->REN  |= BITs;             //������\����
      GPIOX[port]->POUT &=~BITs;             //����Ϊ����
    }
    else if((config & PULL_UP) == PULL_UP)//�ж��Ƿ�������
    {
      GPIOX[port]->REN  |= BITs;             //������\����
      GPIOX[port]->POUT |= BITs;             //����Ϊ����
    }
    else
    {
      GPIOX[port]->REN &=~BITs;              //�ر���\����
    }
    
    if((portn == PORT1)||(portn == PORT2)
#ifdef  GPIO_PORT_SIZE_WORD
       ||(portn == PORTA)
#endif       
       );                 //С�ڵ���P2����ܻ�����IO���ж�
    {
      GPIOX[port]->IE &=~BITs;                  //�ر��ж�
      if((config & IRQ_RISING) == IRQ_RISING)       //�����������
      {
        GPIOX[port]->IES |= BITs;                  //��Ϊ������
      }
      else 
      {
        GPIOX[port]->IES &=~BITs ;                 //��Ϊ�½���
      } 
    }
  }
}
/*******************************************************************************
*  �������ƣ� GPIO_SetBits  (PORTn portn, uint16_t BITs)
*  ����˵��������IO�����Ϊ�ߵ�ƽ
*  ����˵���� PORTn portn     ��ӦIO�˿�
              uint16_t BITs   ��ӦIO����λ��
*  �������أ���
*  ʹ��ʾ���� GPIO_SetBits  (PORT1, GPIO_Pin_0|GPIO_Pin_1); //P1.0,P1.1����ߵ�ƽ
********************************************************************************/
void     GPIO_SetBits  (PORTn portn, uint16_t BITs)
{
  if((portn <= PORT8)) 
    BITs = BITs & 0xFF;
  //��ȡport,pin
  PORTn port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  //IO�ڲ���
  GPIOX[port]->POUT |= BITs;            //
}
/*******************************************************************************
*  �������ƣ� GPIO_ResetBits(PORTn portn, uint16_t BITs)
*  ����˵��������IO�����Ϊ�͵�ƽ
*  ����˵���� PORTn portn     ��ӦIO�˿�
              uint16_t BITs   ��ӦIO����λ��
*  �������أ���
*  ʹ��ʾ����GPIO_ResetBits  (PORT1, GPIO_Pin_0|GPIO_Pin_1); //P1.0,P1.1����͵�ƽ
********************************************************************************/
void     GPIO_ResetBits(PORTn portn, uint16_t BITs)
{
  //��ȡport,pin
  PORTn port = GET_PORT(portn);
  BITs = GET_BITs(portn,BITs);
  //IO�ڲ���
  GPIOX[port]->POUT &=~BITs;    
}
/*******************************************************************************
�������ƣ�IO��һλ�������
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin     ��ӦIO����λ��
          STATUS level    ���������ƽ��SET ���ߵ�ƽ  RESET���͵�ƽ��
*  ʹ��ʾ����
********************************************************************************/
void GPIO_WriteBit(PORTn portn,uint8_t pin,STATUS level)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO�ڲ���
  if(level != RESET)
  {
    GPIOX[port]->POUT |= BIT;            //
  }
  else 
  {
    GPIOX[port]->POUT &=~BIT;            //
  }
}
/*******************************************************************************
�������ƣ�IO��һλ�����ȡ
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin    ��ӦIO����λ��
���ز�����STATUS         �����ƽ��SET ���ߵ�ƽ  RESET���͵�ƽ��
*  ʹ��ʾ����
********************************************************************************/
STATUS GPIO_ReadBit(PORTn portn,uint8_t pin)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO�ڲ���
  return ((GPIOX[port]->IN & BIT) ? BIT_SET : RESET);
}
/*******************************************************************************
�������ƣ�IO��һλ�����ת
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin     ��ӦIO����λ��
*  ʹ��ʾ����
********************************************************************************/
void GPIO_TurnBit  (PORTn portn,uint8_t pin)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u << GET_PIN(portn,pin));
  //IO�ڲ���
  GPIOX[port]->POUT ^= BIT;
}
/*******************************************************************************
�������ƣ�IO��8λ�������
��ڲ�����PORTn portn    ��ӦIO�˿�,����ȡPORT1-PORT8
          uint8_t level   ����8λ�����ƽ
*  ʹ��ʾ����
********************************************************************************/
void GPIO_WriteByte(PORTn portn,uint8_t value)
{
#ifdef GPIO_PORT_SIZE_WORD
  
  ASSERT((portn <= PORT8),
         "GPIO_WriteByte",
         "�˿����ò�����"); //���Լ���������
  
  PORTn port = GET_PORT(portn);
  
  if((portn%2) == 0) //���portn�ǻ���ʱ
  {
    GPIOX[port]->POUT = GPIOX[port]->POUT & 0xFF00;
    GPIOX[port]->POUT |= value;
  }
  else
  {
    GPIOX[port]->POUT = GPIOX[port]->POUT & 0x00FF;
    GPIOX[port]->POUT |= ((uint16_t)value<<8u);
  }
#else
  GPIOX[portn]->POUT = value;            //
#endif
}
/*******************************************************************************
�������ƣ�IO��8λ�����ȡ
��ڲ�����PORTn portn    ��ӦIO�˿�,����ȡPORT1-PORT8
���ز�����uint8 8λ�����ƽ
*  ʹ��ʾ����
********************************************************************************/
uint8_t GPIO_ReadByte(PORTn portn)
{
#ifdef GPIO_PORT_SIZE_WORD
  //����������ʱP1-P8ʱ
  ASSERT((portn <= PORT8),
         "GPIO_ReadByte",
         "�˿����ò�����"); //���Լ���������
  
  PORTn port = GET_PORT(portn);
  
  if((portn%2) == 0) //���portn�ǻ���ʱ
  {
    return ((GPIOX[port]->IN) & 0x00FF);
  }
  else
  {
    return ((GPIOX[port]->IN) >> 8u);
  }
#else
  return (GPIOX[portn]->IN);            //
#endif
}
#ifdef GPIO_PORT_SIZE_WORD
/*******************************************************************************
�������ƣ�IO��16λ�������
��ڲ�����PORTn portn     ��ӦIO�˿�,����ȡPORTA-PORTD
          uint16_t level   ����16λ�����ƽ
*  ʹ��ʾ����
********************************************************************************/
void    GPIO_WriteWord(PORTn portn, uint16_t value) 
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  
  GPIOX[port]->POUT = value;            //
}
/*******************************************************************************
�������ƣ�IO��16λ�����ȡ
��ڲ�����PORTn portn    ��ӦIO�˿�,����ȡPORTA-PORTD
���ز�����uint16 16λ�����ƽ
*  ʹ��ʾ����
********************************************************************************/
uint16_t GPIO_ReadWord(PORTn portn)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  
  return (GPIOX[port]->IN);
}
#endif
/*******************************************************************************
�������ƣ������Ƿ�����IO���ж�
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin     ��ӦIO����λ��
*  ʹ��ʾ����
********************************************************************************/
void GPIO_ITConfig(PORTn portn,uint8_t pin,STATUS ITState)
{
  ASSERT(
#ifdef  GPIO_PORT_SIZE_WORD
         (portn == PORTA)||
#endif
         (portn == PORT1)||(portn == PORT2),
         "GPIO_ITConfig",
         "�˿����ò�����");                 //���Լ���������

  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO�ڲ���
  
  if(ITState != DISABLE)
  {
    GPIOX[port]->IFG &=~BIT;                  //���жϱ�־
    GPIOX[port]->IE |= BIT;
  }
  else
  {
    GPIOX[port]->IE &=~BIT;
  }
}
/*******************************************************************************
�������ƣ���ȡĳһλ�˿��жϱ�־
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin     ��ӦIO����λ��
*  ʹ��ʾ����
********************************************************************************/
STATUS GPIO_GetITStatus(PORTn portn,uint8_t pin)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO�ڲ���
  return ((GPIOX[port]->IFG & BIT) ? TRUE : FALSE);
}
/*******************************************************************************
�������ƣ����8λ�жϱ�־
��ڲ�����PORTn portn    ��ӦIO�˿�
          uint8_t pin     ��ӦIO����λ��
*  ʹ��ʾ����
********************************************************************************/
void  GPIO_ClearITPendingBit(PORTn portn,uint8_t pin)
{
  //��ȡport,pin
  PORTn   port = GET_PORT(portn);
  uint16_t BIT = (1u<<GET_PIN(portn,pin));
  //IO�ڲ���
  GPIOX[port]->IFG &=~BIT;
}