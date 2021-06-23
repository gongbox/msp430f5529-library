#ifndef FLASH_H_
#define FLASH_H_

#include "common.h"

#define   MIN_SEGMENT    (34)
#define   MAX_SEGMENT    (290-1)

__ramfunc void Flash_Erase_Segment(uint16_t segmentNo);                         //����һ����λ��д��ǰ�����Ȳ���������һ����λ														//����ָ��flash����
__ramfunc void Flash_Erase_Addr   (uint32_t addr);                              //����ĳһ�̶�λ�ÿ�ʼ�Ķ�λ
__ramfunc void Flash_Write_Byte   (uint16_t segmentNo, uint16_t offset, uint8_t data);		  //д��flash����
__ramfunc void Flash_Write_Word   (uint16_t segmentNo, uint16_t offset, uint16_t data);		  //д��flash����
__ramfunc void Flash_Write_DWord  (uint16_t segmentNo, uint16_t offset, uint32_t data);		  //д��flash����
__ramfunc void Flash_Write_buf	  (uint16_t segmentNo, uint16_t offset, uint16_t cnt,uint8_t buf[]); //�ӻ�����д��flash����

#define	Flash_Read(segmentNo,offset,type)	(*(type *)((uint32_t)segmentNo*512 + offset))  //��ȡflash���ݣ�typeΪ����ȡ�����ͣ�char,int�ȣ�

#endif