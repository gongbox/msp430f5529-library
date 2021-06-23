#include "msp430f5529_flash.h"
#include "msp430f5529_system.h"

//ע�⣺ʹ��flashʱӦʹ�ÿ���ĵ�ַ��������ܻ��ƻ�����
/*******************************************************************************
*  �������ƣ�����flash�е�ĳһ��
*  ����˵������Ӧ�����ڣ�MIN_SEGMENT - MAX_SEGMENT����Ϊ�����������ȡ��󼸸��Σ�ע�ⲻҪ�����ж�������
*  ����˵��������һ�β���512�ֽڣ�Flash������128KB����ӦsegmentNo������128*1024/512=256����ʹ��ʱ����ȡ������Ķ�λ
*  �������أ���
*  ʹ��ʾ����Flash_Erase_Segment(MAX_SEGMENT);                  //������MAX_SEGMENT��
********************************************************************************/
__ramfunc void Flash_Erase_Segment(uint16_t segmentNo)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Erase_Segment","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����
  
  uint16_t * Addr = (uint16_t *)((uint32_t)segmentNo*512);   //�����ַ
  
  FCTL3 = FWKEY;                              // �������
  FCTL1 = FWKEY+ERASE;                        // ����һ��
  *Addr = 0;                                  //�Բ����ĵ�ַ��Χ��������һ�ο�д�룬����������������
  while (FCTL3 & BUSY);                       // �ȴ��������
  FCTL1 = FWKEY;                              //�������λ  
  FCTL3 = FWKEY+LOCK;                         //��FLASH�������ɶ�������д�����ɲ���
}
/*******************************************************************************
*  �������ƣ�����flash�е�ĳһ�̶�λ��
*  ����˵����
*  ����˵��������һ�β���512�ֽ�
*  �������أ���
*  ʹ��ʾ����Flash_Erase_Addr(0x1800);
********************************************************************************/
__ramfunc void Flash_Erase_Addr   (uint32_t addr)
{
  uint16_t * Addr = (uint16_t *)addr;
  FCTL3 = FWKEY;                              // �������
  FCTL1 = FWKEY+ERASE;                        // ����һ��
  *Addr = 0;                                  //�Բ����ĵ�ַ��Χ��������һ�ο�д�룬����������������
  while (FCTL3 & BUSY);                       // �ȴ��������
  FCTL1 = FWKEY;                              //�������λ  
  FCTL3 = FWKEY+LOCK;                         //��FLASH�������ɶ�������д�����ɲ���
}
/*******************************************************************************
*  �������ƣ�
*  ����˵����д��һ���ֽ����ݵ�Flash
*  ����˵����uint16 segmentNo ����λ uint16_t offset ��ƫ������uint16 data ��Ҫд�������
*  �������أ���
*  ʹ��ʾ����Flash_Write(200,0,0x10);    //�ڵ�200����λ��ƫ��0���ֽڣ�д������0x10
********************************************************************************/
__ramfunc void Flash_Write_Byte(uint16_t segmentNo,uint16_t offset,uint8_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_Byte","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����
  
  uint8_t * Addr = (uint8_t *)((uint32_t)segmentNo*512 + offset);        //�����ַ
  
  FCTL3 = FWKEY;                            //������� 
  FCTL1 = FWKEY+WRT;                        //��λ���λ
  *Addr = data;                             //д��һ����
  while (FCTL3 & BUSY);                     //�ȴ��������
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
}
/*******************************************************************************
*  �������ƣ�
*  ����˵����д��һ�������ݵ�Flash
*  ����˵����uint16 segmentNo ����λ uint16_t offset ��ƫ������uint16 data ��Ҫд�������
*  �������أ���
*  ʹ��ʾ����Flash_Write(200,0,0xf010);    //�ڵ�200����λ��ƫ��0���ֽڣ�д������0xf010
********************************************************************************/
__ramfunc void Flash_Write_Word(uint16_t segmentNo,uint16_t offset,uint16_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_Word","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����
  
  uint16_t * Addr=(uint16_t *)((uint32_t)segmentNo*512 + offset);        //�����ַ
  
  FCTL3 = FWKEY;                            //������� 
  FCTL1 = FWKEY+BLKWRT;                     //��λ���λ
  *Addr = data;                             //д��һ����
  while (FCTL3 & BUSY);                     //�ȴ��������
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
}
/*******************************************************************************
*  �������ƣ�
*  ����˵����д���ĸ��ֽ����ݵ�Flash
*  ����˵����uint16 segmentNo ����λ uint16_t offset ��ƫ������int32 data ��Ҫд�������
*  �������أ���
*  ʹ��ʾ����Flash_Write(200,0,0xf0e0d010);    //�ڵ�200����λ��ƫ��0���ֽڣ�д������0xf0e0d010
********************************************************************************/
__ramfunc void Flash_Write_DWold(uint16_t segmentNo,uint16_t offset,uint32_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_DWold","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����
  
  uint32_t * Addr = (uint32_t *)((uint32_t)segmentNo*512 + offset);        //�����ַ
  
  FCTL3 = FWKEY;                            //������� 
  FCTL1 = FWKEY+BLKWRT;                     //��λ���λ
  *Addr = data;                             //д��һ����
  while (FCTL3 & BUSY);                     //�ȴ��������
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
}
/*******************************************************************************
*  �������ƣ�Flash_Write_buf(uint16_t segmentNo,uint16_t offset,uint16_t cnt,uint8_t buf[])
*  ����˵����д�����ֽ����ݵ�Flash
*  ����˵����uint16 segmentNo ����λ uint16_t offset ����ʼ����ƫ������uint16 cnt ��д������������uint8 buf[]Ҫд�������
*  �������أ���
*  ʹ��ʾ����uint8 data[10];Flash_Write_buf(200,0,10,data);            //�ӵ�200����λ��ƫ�Ƶ�ַΪ0��ʼ������д��10���ֽ�
********************************************************************************/
__ramfunc void Flash_Write_buf(uint16_t segmentNo,uint16_t offset,uint16_t cnt,uint8_t buf[])
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_buf","segmentNo����������Χ��");  //���Լ��η�Χ�Ƿ���FLASH����
   
  uint8_t * Addr = (uint8_t *)((uint32_t)segmentNo*512 + offset);        //�����ַ
   
  FCTL3 = FWKEY;                            //������� 
  FCTL1 = FWKEY + WRT ;                     //��λ���λ
  while(cnt--)                              
  {
    while (FCTL3 & BUSY);                   //�ȴ��������
    *Addr++ = *buf++;                       //д������ʱ����ʱ������
  }
  FCTL1 = FWKEY;                            //���д��λ
  FCTL3 = FWKEY+LOCK;                       //��FLASH�������ɶ�������д�����ɲ�������������
}