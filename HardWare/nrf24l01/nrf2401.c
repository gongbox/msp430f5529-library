#include "nrf2401.H"
//********************************************************************************************************************// 
// SPI(nRF24L01) 指令
#define READ_REG    0x00   // 读配置寄存器
#define WRITE_REG   0x20   // 写配置寄存器
#define RD_RX_PLOAD 0x61   // 读取RX FIFO中的数据
#define WR_TX_PLOAD 0xA0   // 向TX FIFO中写入数据
#define FLUSH_TX    0xE1   // 清除TX FIFO中的数据 应用于发射模式下
#define FLUSH_RX    0xE2   // 清除RX FIFO中的数据 应用于接收模式下
#define REUSE_TX_PL 0xE3   // 重新使用上一包有效数据
#define NOP         0xFF   // 保留
//********************************************************************************************************************// 
// SPI(nRF24L01) 寄存器(地址)
#define CONFIG      0x00  //配置发送状态，CRC校验模式以及发收发状态响应方式

#define MASK_RX_DR  BIT6  //可屏蔽中断-接收中断 1：屏蔽中断
#define MASK_TX_DS  BIT5  //可屏蔽中断-发送完成中断
#define MASK_MAX_RT BIT4  //可屏蔽中断-达到最大重发次数中断
#define EN_CRC      BIT3  //使能CRC
#define CRCO        BIT2  //0：CRC8位: 1：16位
#define PWR_UP      BIT1  //1：上电 0：掉电
#define PRIM_RX     BIT0  //1：接收模式 0：发射模式

#define EN_AA       0x01  //自动应答功能设置
#define EN_RXADDR   0x02  //可用信道设置
#define SETUP_AW    0x03  //收发地址宽度设置
#define SETUP_RETR  0x04  //自动重发功能设设置
#define RF_CH       0x05  //工作频率设定
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

//几个重要的状态标记
#define STATUS_TX_FULL     0x01        //TX FIFO 寄存器满标志。 1 为 满，0为 不满
#define STATUS_MAX_RT      0x10        //达到最大重发次数中断标志位
#define STATUS_TX_DS       0x20        //发送完成中断标志位
#define STATUS_RX_DR       0x40        //接收到数据中断标志位

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xE1,0xE2,0xE3,0xE4,0xE5}; //接收地址

static s_NRF24L01 s_nrf24l01;
/***********************************************************************************************
 功能：NRF24L01 读写一个字节
 形参：发送的数据
 返回：读到的数据
 详解：调用SPI模块
************************************************************************************************/
static uint8_t NRF2401_ReadWriteByte(uint8_t data)
{
#if NRF24L01HardWareSPI
  return NRF2401HardWareSPI_SendByte(data);   //发送并读取一字节数据
#else
  for(uint8_t i=0;i < 8;i++)          // 循环8次
  {
    NRF2401_SIMO = (data&0x80) ? 1 : 0;//上升沿发送数据// byte最高位输出到MOSI
    data <<= 1;             // 低一位移位到最高位
    NRF2401_SCL = 1; 
    if(NRF2401_SOMI)            // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
      data |= 0x01;       	// 读MISO到byte最低位
    NRF2401_SCL = 0;            	// SCK置低
  }
  return(data);           	// 返回读出的一字节
#endif
}
/***********************************************************************************************
 功能：NRF24L01 写寄存器
 形参：reg: 寄存器
       value:写入的值
 返回：读到的数据
 详解：无
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
 功能：NRF24L01 写寄存器
 形参：reg: 寄存器
       value:写入的值
 返回：读到的数据
 详解：无
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
 功能：NRF24L01 读Buffer
 形参：reg: 寄存器
       pBuf:数据指针
		 	 bytes: 写入的字节数
 返回：读状态
 详解：无
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
 功能：检测NRF24L01 是否存在
 形参：
 返回：NRF_OK : 成功   NRF_ERR:失败
 详解：无
************************************************************************************************/
static uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	NRF2401_WriteBuffer(WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF2401_ReadBuffer(TX_ADDR,buf,5); //读出写入的地址  
	for(uint8_t i=0;i < 5;i++)
	{
		if(buf[i] != 0xA5) return NRF_ERR;
	}
	return NRF_OK;
/*******************************************************************************
*  函数名称：
*  功能说明：NRF24L01初始化
*  参数说明：
*  函数返回：
*  使用示例：
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
        //检测NRF2401 是否存在
        uint8_t checkStatus = NRF24L01_Check();
        if(checkStatus == NRF_OK)
        {
          NRF2401_CE=0;
          
          NRF2401_ReadWriteReg(WRITE_REG+SETUP_AW, ADR_WIDTH - 2);          //设置地址长度为 TX_ADR_WIDTH   	    
          NRF2401_WriteBuffer(WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
          NRF2401_WriteBuffer(WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
          NRF2401_ReadWriteReg(WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
          NRF2401_ReadWriteReg(WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
          NRF2401_ReadWriteReg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
          NRF2401_ReadWriteReg(WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
          NRF2401_ReadWriteReg(WRITE_REG+RF_CH,40);       //设置RF通道为40
          NRF2401_ReadWriteReg(WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
          NRF2401_ReadWriteReg(WRITE_REG+CONFIG,0x0e);     //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断

          NRF2401_CE=1;//CE为高,10us后启动发送
        }
	return checkStatus;
}
/*******************************************************************************
*  函数名称：
*  功能说明：该函数初始化NRF24L01到RX模式
             设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
             当CE变高后,即进入RX模式,并可以接收数据了
*  参数说明：
*  函数返回：
*  使用示例：
********************************************************************************/
void NRF2401_SetRXMode(void)
{
	NRF2401_CE=0;
   
	NRF2401_ReadWriteReg(WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
        
	NRF2401_CE = 1; //CE为高,进入接收模式
}	
/*******************************************************************************
*  函数名称：
*  功能说明：该函数初始化NRF24L01到TX模式
             设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
             PWR_UP,CRC使能
             当CE变高后,即进入RX模式,并可以接收数据了		   
             CE为高大于10us,则启动发送.	
*  参数说明：
*  函数返回：
*  使用示例：
********************************************************************************/
void NRF2401_SetTXMode(void)
{				
	NRF2401_CE=0;
 
	NRF2401_ReadWriteReg(WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	
        NRF2401_CE=1;//CE为高,10us后启动发送
}
/*******************************************************************************
*  函数名称：
*  功能说明：启动NRF24L01发送一次数据
*  参数说明：txbuf:待发送数据首地址
*  函数返回：返回值:发送完成状况
*  使用示例：
********************************************************************************/
void NRF2401_SendData(uint8_t *txbuf)
{
	/*uint8_t sta;
	NRF2401_CE=0;
	NRF2401_WriteBuffer(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF2401_CE=1;//启动发送	  
	while(NRF2401_IRQ!=0);//等待发送完成
	sta = NRF2401_ReadWriteReg(STATUS,0xFF);  //读取状态寄存器的值	
	NRF2401_ReadWriteReg(WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	
	if(sta & MAX_TX)//达到最大重发次数
	{
		NRF2401_ReadWriteReg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta & TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
        */
        NRF2401_CE = 0;			//StandBy I模式	
        
	NRF2401_WriteBuffer(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); 			 // 装载数据	
        NRF2401_ReadWriteReg(WRITE_REG+STATUS,STATUS_TX_DS|STATUS_MAX_RT); //清除TX_DS或MAX_RT中断标志
        
        NRF2401_CE = 1;		 //置高CE，激发数据发送
}
/*******************************************************************************
*  函数名称：
*  功能说明：启动NRF24L01发送一次数据
*  参数说明：txbuf:待发送数据首地址
*  函数返回：返回值:0，接收完成；其他，错误代码
*  使用示例：
********************************************************************************/
uint8_t NRF2401_RecData(uint8_t *rxbuf)
{
	uint8_t status = NRF2401_ReadWriteReg(STATUS,0xFF);  //读取状态寄存器的值    	 
	NRF2401_ReadWriteReg(WRITE_REG+STATUS,status); //清除TX_DS或MAX_RT中断标志
        if(status & RX_OK)//接收到数据
	{
		NRF2401_ReadBuffer(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF2401_ReadWriteReg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return RX_OK; 
	}   
	return 0xff;//没收到任何数据
}
/*******************************************************************************
*  函数名称：
*  功能说明：NRF24L01中断处理函数，请在中断中添加该函数
*  参数说明：
*  函数返回：
*  使用示例：
********************************************************************************/
void  NRF2401_IRQ_Handler()
{
    /*读取status寄存器的值  */
    uint8_t state = NRF2401_ReadWriteReg(STATUS,0xFF);  //读取状态寄存器的值   
    if(state & STATUS_RX_DR) //接收到数据
    {
       NRF2401_CE = 0;			//StandBy I模式	
       if(s_nrf24l01.CallBack_RecevieData != NULL)
       {
          NRF2401_ReadBuffer(RD_RX_PLOAD,(uint8_t *)s_nrf24l01.pReceiveData,RX_PLOAD_WIDTH);//读取数据
          s_nrf24l01.CallBack_RecevieData();
       }
       NRF2401_ReadWriteReg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
       NRF2401_CE = 1;		 //置高CE，激发数据发送
    }

    if(state & STATUS_TX_DS) //发送完数据
    {
        if(s_nrf24l01.CallBack_SendDataOk != NULL)
        {
          s_nrf24l01.CallBack_SendDataOk();
        }
    }

    if(state & STATUS_MAX_RT)      //发送超时
    {
        if(s_nrf24l01.CallBack_SendOutTime != NULL)
        {
           s_nrf24l01.CallBack_SendOutTime();
        }
        NRF2401_ReadWriteReg(FLUSH_TX,0xff);//清除TX FIFO寄存器
    }

    if(state & STATUS_TX_FULL) //TX FIFO 满
    {
        if(s_nrf24l01.CallBack_TxFull != NULL)
        {
           s_nrf24l01.CallBack_TxFull();
        }
    }
    NRF2401_ReadWriteReg(WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
}


