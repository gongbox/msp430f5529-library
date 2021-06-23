#include "msp430f5529_flash.h"
#include "msp430f5529_system.h"

//注意：使用flash时应使用靠后的地址，否则可能会破坏程序
/*******************************************************************************
*  函数名称：擦除flash中的某一段
*  功能说明：段应该是在（MIN_SEGMENT - MAX_SEGMENT），为保护程序，最好取最后几个段，注意不要擦除中断向量表
*  参数说明：必须一次擦除512字节，Flash容量（128KB）对应segmentNo数量（128*1024/512=256），使用时建议取靠后面的段位
*  函数返回：无
*  使用示例：Flash_Erase_Segment(MAX_SEGMENT);                  //擦除第MAX_SEGMENT段
********************************************************************************/
__ramfunc void Flash_Erase_Segment(uint16_t segmentNo)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Erase_Segment","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间
  
  uint16_t * Addr = (uint16_t *)((uint32_t)segmentNo*512);   //计算地址
  
  FCTL3 = FWKEY;                              // 清除锁定
  FCTL1 = FWKEY+ERASE;                        // 擦除一段
  *Addr = 0;                                  //对擦除的地址范围内任意做一次空写入，用以启动擦除操作
  while (FCTL3 & BUSY);                       // 等待允许操作
  FCTL1 = FWKEY;                              //清除擦除位  
  FCTL3 = FWKEY+LOCK;                         //对FLASH加锁，可读，不可写，不可擦除
}
/*******************************************************************************
*  函数名称：擦除flash中的某一固定位置
*  功能说明：
*  参数说明：必须一次擦除512字节
*  函数返回：无
*  使用示例：Flash_Erase_Addr(0x1800);
********************************************************************************/
__ramfunc void Flash_Erase_Addr   (uint32_t addr)
{
  uint16_t * Addr = (uint16_t *)addr;
  FCTL3 = FWKEY;                              // 清除锁定
  FCTL1 = FWKEY+ERASE;                        // 擦除一段
  *Addr = 0;                                  //对擦除的地址范围内任意做一次空写入，用以启动擦除操作
  while (FCTL3 & BUSY);                       // 等待允许操作
  FCTL1 = FWKEY;                              //清除擦除位  
  FCTL3 = FWKEY+LOCK;                         //对FLASH加锁，可读，不可写，不可擦除
}
/*******************************************************************************
*  函数名称：
*  功能说明：写入一个字节数据到Flash
*  参数说明：uint16 segmentNo ：段位 uint16_t offset ：偏移量，uint16 data ：要写入的数据
*  函数返回：无
*  使用示例：Flash_Write(200,0,0x10);    //在第200个段位，偏移0个字节，写入数据0x10
********************************************************************************/
__ramfunc void Flash_Write_Byte(uint16_t segmentNo,uint16_t offset,uint8_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_Byte","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间
  
  uint8_t * Addr = (uint8_t *)((uint32_t)segmentNo*512 + offset);        //计算地址
  
  FCTL3 = FWKEY;                            //清除锁定 
  FCTL1 = FWKEY+WRT;                        //置位编程位
  *Addr = data;                             //写入一个字
  while (FCTL3 & BUSY);                     //等待操作完成
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}
/*******************************************************************************
*  函数名称：
*  功能说明：写入一个字数据到Flash
*  参数说明：uint16 segmentNo ：段位 uint16_t offset ：偏移量，uint16 data ：要写入的数据
*  函数返回：无
*  使用示例：Flash_Write(200,0,0xf010);    //在第200个段位，偏移0个字节，写入数据0xf010
********************************************************************************/
__ramfunc void Flash_Write_Word(uint16_t segmentNo,uint16_t offset,uint16_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_Word","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间
  
  uint16_t * Addr=(uint16_t *)((uint32_t)segmentNo*512 + offset);        //计算地址
  
  FCTL3 = FWKEY;                            //清除锁定 
  FCTL1 = FWKEY+BLKWRT;                     //置位编程位
  *Addr = data;                             //写入一个字
  while (FCTL3 & BUSY);                     //等待操作完成
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}
/*******************************************************************************
*  函数名称：
*  功能说明：写入四个字节数据到Flash
*  参数说明：uint16 segmentNo ：段位 uint16_t offset ：偏移量，int32 data ：要写入的数据
*  函数返回：无
*  使用示例：Flash_Write(200,0,0xf0e0d010);    //在第200个段位，偏移0个字节，写入数据0xf0e0d010
********************************************************************************/
__ramfunc void Flash_Write_DWold(uint16_t segmentNo,uint16_t offset,uint32_t data)
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_DWold","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间
  
  uint32_t * Addr = (uint32_t *)((uint32_t)segmentNo*512 + offset);        //计算地址
  
  FCTL3 = FWKEY;                            //清除锁定 
  FCTL1 = FWKEY+BLKWRT;                     //置位编程位
  *Addr = data;                             //写入一个字
  while (FCTL3 & BUSY);                     //等待操作完成
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}
/*******************************************************************************
*  函数名称：Flash_Write_buf(uint16_t segmentNo,uint16_t offset,uint16_t cnt,uint8_t buf[])
*  功能说明：写入多个字节数据到Flash
*  参数说明：uint16 segmentNo ：段位 uint16_t offset ：起始数据偏移量，uint16 cnt ：写入数据数量，uint8 buf[]要写入的数据
*  函数返回：无
*  使用示例：uint8 data[10];Flash_Write_buf(200,0,10,data);            //从第200个段位，偏移地址为0开始，连续写入10个字节
********************************************************************************/
__ramfunc void Flash_Write_buf(uint16_t segmentNo,uint16_t offset,uint16_t cnt,uint8_t buf[])
{
  ASSERT(segmentNo >= MIN_SEGMENT && segmentNo <= MAX_SEGMENT,"Flash_Write_buf","segmentNo参数超出范围！");  //断言检测段范围是否在FLASH区间
   
  uint8_t * Addr = (uint8_t *)((uint32_t)segmentNo*512 + offset);        //计算地址
   
  FCTL3 = FWKEY;                            //清除锁定 
  FCTL1 = FWKEY + WRT ;                     //置位编程位
  while(cnt--)                              
  {
    while (FCTL3 & BUSY);                   //等待允许操作
    *Addr++ = *buf++;                       //写入数据时启动时序发生器
  }
  FCTL1 = FWKEY;                            //清除写入位
  FCTL3 = FWKEY+LOCK;                       //对FLASH加锁，可读，不可写，不可擦除，保护数据
}