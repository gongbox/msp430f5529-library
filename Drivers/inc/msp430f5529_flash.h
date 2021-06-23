#ifndef FLASH_H_
#define FLASH_H_

#include "common.h"

#define   MIN_SEGMENT    (34)
#define   MAX_SEGMENT    (290-1)

__ramfunc void Flash_Erase_Segment(uint16_t segmentNo);                         //擦除一个段位，写入前必须先擦除，擦出一个段位														//擦除指定flash扇区
__ramfunc void Flash_Erase_Addr   (uint32_t addr);                              //擦除某一固定位置开始的段位
__ramfunc void Flash_Write_Byte   (uint16_t segmentNo, uint16_t offset, uint8_t data);		  //写入flash操作
__ramfunc void Flash_Write_Word   (uint16_t segmentNo, uint16_t offset, uint16_t data);		  //写入flash操作
__ramfunc void Flash_Write_DWord  (uint16_t segmentNo, uint16_t offset, uint32_t data);		  //写入flash操作
__ramfunc void Flash_Write_buf	  (uint16_t segmentNo, uint16_t offset, uint16_t cnt,uint8_t buf[]); //从缓存区写入flash操作

#define	Flash_Read(segmentNo,offset,type)	(*(type *)((uint32_t)segmentNo*512 + offset))  //读取flash内容，type为读读取的类型（char,int等）

#endif