#ifndef _COMMON_H_
#define _COMMON_H_
/************************************************************
* STANDARD BITS
************************************************************/
#ifndef BIT0
#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)
#endif

#define Hz   *1u
#define KHz  *1E3
#define MHz  *1E6
#define HZ   *1u
#define KHZ  *1E3
#define MHZ  *1E6

//���ڿɱ��������ȡ����������Ҫ���������С��8��
#define __ARG_NUM(ARG0,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,...)        ARG8 //ȡ����9�����ʽ
#define ARG_NUM(...)            __ARG_NUM(,##__VA_ARGS__,7,6,5,4,3,2,1,0)       //��ȡ��������(��9�����ʽ��Ϊ��������)
//ת��Ϊ�ַ���
#define _TOSTRING(str)          #str
#define TOSTRING(str)           _TOSTRING(str)  

#ifndef NULL
  #define NULL 0
#endif
/*
 * ������������
 */
#include "stdint.h"

typedef void (*FUNCPTR)();

typedef enum
{
  FALSE = 0u,   //��
  TRUE  = 1u,   //��
  
  ERROR = 0u,   //��
  SUCCESS = 1u, //��
  
  RESET = 0u,//��
  
  BIT_RESET = 0u,
  BIT_SET = 1u,  //��
  /*
  ALL_RESET = 0u,   //��
  ALL_SET = ~0,     //��
  */
  /*
  RESET = 0u,//��
  SET = 1u,  //��
  */
  
  OFF = 0u, //��
  ON  = 1u, //��
  
  DISABLE = 0u, //��
  ENABLE  = 1u, //��
}BOOL,STATUS,FLAG;          //״ָ̬ʾ����

typedef union
{
    uint8_t   Byte;
    struct
    {
        uint8_t b0: 1;
        uint8_t b1: 1;
        uint8_t b2: 1;
        uint8_t b3: 1;
        uint8_t b4: 1;
        uint8_t b5: 1;
        uint8_t b6: 1;
        uint8_t b7: 1;
    };
}ByteType;    //sizeof(ByteType) Ϊ 1
/*
 * �����λ�������������
 */
typedef union
{
    uint16_t  Word;
    uint8_t   Byte[2];
    struct
    {
        uint16_t b0: 1;
        uint16_t b1: 1;
        uint16_t b2: 1;
        uint16_t b3: 1;
        uint16_t b4: 1;
        uint16_t b5: 1;
        uint16_t b6: 1;
        uint16_t b7: 1;
        uint16_t b8: 1;
        uint16_t b9: 1;
        uint16_t b10: 1;
        uint16_t b11: 1;
        uint16_t b12: 1;
        uint16_t b13: 1;
        uint16_t b14: 1;
        uint16_t b15: 1;
    };
}WordType;    //sizeof(Wordtype) Ϊ 2
typedef union
{
    uint32_t  DWord;
    uint16_t  Word[2];
    uint8_t   Byte[4];
    struct
    {
        uint32_t b0: 1;
        uint32_t b1: 1;
        uint32_t b2: 1;
        uint32_t b3: 1;
        uint32_t b4: 1;
        uint32_t b5: 1;
        uint32_t b6: 1;
        uint32_t b7: 1;
        uint32_t b8: 1;
        uint32_t b9: 1;
        uint32_t b10: 1;
        uint32_t b11: 1;
        uint32_t b12: 1;
        uint32_t b13: 1;
        uint32_t b14: 1;
        uint32_t b15: 1;
        uint32_t b16: 1;
        uint32_t b17: 1;
        uint32_t b18: 1;
        uint32_t b19: 1;
        uint32_t b20: 1;
        uint32_t b21: 1;
        uint32_t b22: 1;
        uint32_t b23: 1;
        uint32_t b24: 1;
        uint32_t b25: 1;
        uint32_t b26: 1;
        uint32_t b27: 1;
        uint32_t b28: 1;
        uint32_t b29: 1;
        uint32_t b30: 1;
        uint32_t b31: 1;
    };
}DWordType;    //sizeof(Wordtype) Ϊ 4

//ʵ��λ���������û��ȡĳһ�Ĵ���ĳһλ��ֵ
#define BIT_VALUE_IN_BYTE(byteaddr,bit)       (((ByteType *)(&(byteaddr)))->b##bit)  //�ԼĴ���Ϊ1���ֽڵ����ݲ���ʱʹ��
#define BIT_VALUE_IN_WORD(wordaddr,bit)       (((WordType *)(&(wordaddr)))->b##bit)  //�ԼĴ���Ϊ1����  �����ݲ���ʱʹ��

#include "assert.h"
#include "msp430f5529_system.h"
#include "msp430f5529.h"
#include "msp430f5529_MemMap.h"

#endif