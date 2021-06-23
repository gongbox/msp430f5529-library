#include "nrf2401.H"
//********************************************************************************************************************// 
// SPI(nRF24L01) ָ��
#define READ_REG    0x00   // �����üĴ���
#define WRITE_REG   0x20   // д���üĴ���
#define RD_RX_PLOAD 0x61   // ��ȡRX FIFO�е�����
#define WR_TX_PLOAD 0xA0   // ��TX FIFO��д������
#define FLUSH_TX    0xE1   // ���TX FIFO�е����� Ӧ���ڷ���ģʽ��
#define FLUSH_RX    0xE2   // ���RX FIFO�е����� Ӧ���ڽ���ģʽ��
#define REUSE_TX_PL 0xE3   // ����ʹ����һ����Ч����
#define NOP         0xFF   // ����
//********************************************************************************************************************// 
// SPI(nRF24L01) �Ĵ���(��ַ)
#define CONFIG      0x00  //���÷���״̬��CRCУ��ģʽ�Լ����շ�״̬��Ӧ��ʽ

#define MASK_RX_DR  BIT6  //�������ж�-�����ж� 1�������ж�
#define MASK_TX_DS  BIT5  //�������ж�-��������ж�
#define MASK_MAX_RT BIT4  //�������ж�-�ﵽ����ط������ж�
#define EN_CRC      BIT3  //ʹ��CRC
#define CRCO        BIT2  //0��CRC8λ: 1��16λ
#define PWR_UP      BIT1  //1���ϵ� 0������
#define PRIM_RX     BIT0  //1������ģʽ 0������ģʽ

#define EN_AA       0x01  //�Զ�Ӧ��������
#define EN_RXADDR   0x02  //�����ŵ�����
#define SETUP_AW    0x03  //�շ���ַ�������
#define SETUP_RETR  0x04  //�Զ��ط�����������
#define RF_CH       0x05  //����Ƶ���趨
#define RF_SETUP    0x06  //
#define STATUS      0x07  //
#define OBSERVE_TX  0x08  //
#define CD          0x09  //
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17

//������Ҫ��״̬���
#define STATUS_TX_FULL     0x01        //TX FIFO �Ĵ�������־�� 1 Ϊ ����0Ϊ ����
#define STATUS_MAX_RT      0x10        //�ﵽ����ط������жϱ�־λ
#define STATUS_TX_DS       0x20        //��������жϱ�־λ
#define STATUS_RX_DR       0x40        //���յ������жϱ�־λ

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //���յ�ַ

static s_NRF24L01 s_nrf24l01;
/***********************************************************************************************
 ���ܣ�NRF24L01 ��дһ���ֽ�
 �βΣ����͵�����
 ���أ�����������
 ��⣺����SPIģ��
************************************************************************************************/
static uint8_t NRF2401_ReadWriteByte(uint8_t data)
{
#if NRF24L01HardWareSPI
  return NRF2401HardWareSPI_SendByte(data);   //���Ͳ���ȡһ�ֽ�����
#else
  for(uint8_t i=0;i < 8;i++)          // ѭ��8��
  {
    NRF2401_SIMO = (data&0x80) ? 1 : 0;//�����ط�������// byte���λ�����MOSI
    data <<= 1;             // ��һλ��λ�����λ
    NRF2401_SCL = 1; 
    if(NRF2401_SOMI)            // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
      data |= 0x01;       	// ��MISO��byte���λ
    NRF2401_SCL = 0;            	// SCK�õ�
  }
  return(data);           	// ���ض�����һ�ֽ�
#endif
}
/***********************************************************************************************
 ���ܣ�NRF24L01 д�Ĵ���
 �βΣ�reg: �Ĵ���
       value:д���ֵ
 ���أ�����������
 ��⣺��
************************************************************************************************/
static uint8_t NRF2401_ReadWriteReg(uint8_t reg,uint8_t value)
{
	NRF2401_CSN = 0;                   // CSN low, init SPI transaction
	uint8_t status = NRF2401_ReadWriteByte(reg);      // select register
	NRF2401_ReadWriteByte(value);             // ..and write value to it..
	NRF2401_CSN = 1;                   // CSN high again
	return(status);            // return nRF24L01 status byte
}
/***********************************************************************************************
 ���ܣ�NRF24L01 д�Ĵ���
 �βΣ�reg: �Ĵ���
       value:д���ֵ
 ���أ�����������
 ��⣺��
************************************************************************************************/
static uint8_t NRF2401_WriteBuffer(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
	NRF2401_CSN = 0; 
	uint8_t status = NRF2401_ReadWriteByte(reg);    // Select register to write to and read status byte
	for(uint8_t i=0; i<bytes; i++) // then write all byte in buffer(*pBuf)
	{
		NRF2401_ReadWriteByte(*pBuf++);
	}
	NRF2401_CSN = 1;                 // Set CSN high again
	return(status);          // return nRF24L01 status byte
}
/***********************************************************************************************
 ���ܣ�NRF24L01 ��Buffer
 �βΣ�reg: �Ĵ���
       pBuf:����ָ��
		 	 bytes: д����ֽ���
 ���أ���״̬
 ��⣺��
************************************************************************************************/
static uint8_t NRF2401_ReadBuffer(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
	NRF2401_CSN = 0;                    		// Set CSN low, init SPI tranaction
	uint8_t status = NRF2401_ReadWriteByte(reg);       		// Select register to write to and read status byte
	for(uint8_t i=0;i<bytes;i++)
	{
		pBuf[i] = NRF2401_ReadWriteByte(0);    // Perform SPI_RW to read byte from nRF24L01
	}
	NRF2401_CSN = 1;                           // Set CSN high again
	return(status);                    // return nRF24L01 status byte
}

/***********************************************************************************************
 ���ܣ����NRF24L01 �Ƿ����
 �βΣ�
 ���أ�NRF_OK : �ɹ�   NRF_ERR:ʧ��
 ��⣺��
************************************************************************************************/
static uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	NRF2401_WriteBuffer(WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF2401_ReadBuffer(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(uint8_t i=0;i < 5;i++)
	{
		if(buf[i] != 0xA5) return NRF_ERR;
	}
	return NRF_OK;
/*******************************************************************************
*  �������ƣ�
*  ����˵����NRF24L01��ʼ��
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
********************************************************************************/}
uint8_t NRF2401_Init(s_NRF24L01 nrf24l01)
{
        s_nrf24l01 = nrf24l01;
        
        GPIO_Init(NRF2401_CE_PORT,NRF2401_CE_PIN,GPO);
        GPIO_Init(NRF2401_CSN_PORT,NRF2401_CSN_PIN,GPO);
        //GPIO_Init(NRF2401_IRQ_PORT,NRF2401_IRQ_PIN,GPI);
        
        nrf24l01.CallBack_IRQ_Init();
#if NRF24L01HardWareSPI
        NRF2401HardWareSPI_Init();
#else
        GPIO_Init(NRF2401_SCL_PORT,NRF2401_SCL_PIN,GPO);
        GPIO_Init(NRF2401_SIMO_PORT,NRF2401_SIMO_PIN,GPO);
        GPIO_Init(NRF2401_SOMI_PORT,NRF2401_SOMI_PIN,GPI);
#endif
        
	NRF2401_CSN = 1;			// Spi disable	
        NRF2401_CE = 0;			        // chip enable
        //���NRF2401 �Ƿ����
        uint8_t checkStatus = NRF24L01_Check();
        if(checkStatus == NRF_OK)
        {
          NRF2401_CE=0;
          
          NRF2401_ReadWriteReg(WRITE_REG+SETUP_AW, ADR_WIDTH - 2);          //���õ�ַ����Ϊ TX_ADR_WIDTH   	    
          NRF2401_WriteBuffer(WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
          NRF2401_WriteBuffer(WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
          NRF2401_ReadWriteReg(WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
          NRF2401_ReadWriteReg(WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
          NRF2401_ReadWriteReg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
          NRF2401_ReadWriteReg(WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
          NRF2401_ReadWriteReg(WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
          NRF2401_ReadWriteReg(WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��
          NRF2401_ReadWriteReg(WRITE_REG+CONFIG,0x0e);     //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

          NRF2401_CE=1;//CEΪ��,10us����������
        }
	return checkStatus;
}
/*******************************************************************************
*  �������ƣ�
*  ����˵�����ú�����ʼ��NRF24L01��RXģʽ
             ����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
             ��CE��ߺ�,������RXģʽ,�����Խ���������
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
********************************************************************************/
void NRF2401_SetRXMode(void)
{
	NRF2401_CE=0;
   
	NRF2401_ReadWriteReg(WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
        
	NRF2401_CE = 1; //CEΪ��,�������ģʽ
}	
/*******************************************************************************
*  �������ƣ�
*  ����˵�����ú�����ʼ��NRF24L01��TXģʽ
             ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
             PWR_UP,CRCʹ��
             ��CE��ߺ�,������RXģʽ,�����Խ���������		   
             CEΪ�ߴ���10us,����������.	
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
********************************************************************************/
void NRF2401_SetTXMode(void)
{				
	NRF2401_CE=0;
 
	NRF2401_ReadWriteReg(WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
        NRF2401_CE=1;//CEΪ��,10us����������
}
/*******************************************************************************
*  �������ƣ�
*  ����˵��������NRF24L01����һ������
*  ����˵����txbuf:�����������׵�ַ
*  �������أ�����ֵ:�������״��
*  ʹ��ʾ����
********************************************************************************/
void NRF2401_SendData(uint8_t *txbuf)
{
	/*uint8_t sta;
	NRF2401_CE=0;
	NRF2401_WriteBuffer(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF2401_CE=1;//��������	  
	while(NRF2401_IRQ!=0);//�ȴ��������
	sta = NRF2401_ReadWriteReg(STATUS,0xFF);  //��ȡ״̬�Ĵ�����ֵ	
	NRF2401_ReadWriteReg(WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	
	if(sta & MAX_TX)//�ﵽ����ط�����
	{
		NRF2401_ReadWriteReg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta & TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
        */
        NRF2401_CE = 0;			//StandBy Iģʽ	
        
	NRF2401_WriteBuffer(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); 			 // װ������	
        NRF2401_ReadWriteReg(WRITE_REG+STATUS,STATUS_TX_DS|STATUS_MAX_RT); //���TX_DS��MAX_RT�жϱ�־
        
        NRF2401_CE = 1;		 //�ø�CE���������ݷ���
}
/*******************************************************************************
*  �������ƣ�
*  ����˵��������NRF24L01����һ������
*  ����˵����txbuf:�����������׵�ַ
*  �������أ�����ֵ:0��������ɣ��������������
*  ʹ��ʾ����
********************************************************************************/
uint8_t NRF2401_RecData(uint8_t *rxbuf)
{
	uint8_t status = NRF2401_ReadWriteReg(STATUS,0xFF);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF2401_ReadWriteReg(WRITE_REG+STATUS,status); //���TX_DS��MAX_RT�жϱ�־
        if(status & RX_OK)//���յ�����
	{
		NRF2401_ReadBuffer(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF2401_ReadWriteReg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return RX_OK; 
	}   
	return 0xff;//û�յ��κ�����
}
/*******************************************************************************
*  �������ƣ�
*  ����˵����NRF24L01�жϴ������������ж�����Ӹú���
*  ����˵����
*  �������أ�
*  ʹ��ʾ����
********************************************************************************/
void  NRF2401_IRQ_Handler()
{
    /*��ȡstatus�Ĵ�����ֵ  */
    uint8_t state = NRF2401_ReadWriteReg(STATUS,0xFF);  //��ȡ״̬�Ĵ�����ֵ   
    if(state & STATUS_RX_DR) //���յ�����
    {
       NRF2401_CE = 0;			//StandBy Iģʽ	
       if(s_nrf24l01.CallBack_RecevieData != NULL)
       {
          NRF2401_ReadBuffer(RD_RX_PLOAD,(uint8_t *)s_nrf24l01.pReceiveData,RX_PLOAD_WIDTH);//��ȡ����
          s_nrf24l01.CallBack_RecevieData();
       }
       NRF2401_ReadWriteReg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
       NRF2401_CE = 1;		 //�ø�CE���������ݷ���
    }

    if(state & STATUS_TX_DS) //����������
    {
        if(s_nrf24l01.CallBack_SendDataOk != NULL)
        {
          s_nrf24l01.CallBack_SendDataOk();
        }
    }

    if(state & STATUS_MAX_RT)      //���ͳ�ʱ
    {
        if(s_nrf24l01.CallBack_SendOutTime != NULL)
        {
           s_nrf24l01.CallBack_SendOutTime();
        }
        NRF2401_ReadWriteReg(FLUSH_TX,0xff);//���TX FIFO�Ĵ���
    }

    if(state & STATUS_TX_FULL) //TX FIFO ��
    {
        if(s_nrf24l01.CallBack_TxFull != NULL)
        {
           s_nrf24l01.CallBack_TxFull();
        }
    }
    NRF2401_ReadWriteReg(WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
}


