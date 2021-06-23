#include "msp430f5529_it.h"
#include "msp430f5529_vectors.h"

//�����ж�������RAM���ض����򣬺���Ҫ
__no_init ISR_TYPE __VECTOR_RAM[VECTOR_SIZE]@VECTOR_RAM_START_ADDR;
#define            __VECTOR_ROM ((ISR_TYPE *)VECTOR_ROM_START_ADDR)
extern const SFR_MemMapPtr SFRX;
const SYS_MemMapPtr SYSX = SYS_BASE_PTR;
/*******************************************************************************
*  �������ƣ�Set_Vector_Handler(VECTORn vector,__interrupt void pfunc_handler(void))
*  ����˵���������ж�������ַ
*  ����˵����
            VECTORn vector  ���ж�������ö�ٱ�������.h�ļ���
            __interrupt void pfunc_handler(void)  �� �жϷ�����
*  �������أ���
*  ʹ��ʾ����Set_Vector_Handler(VECTOR_PORT1,GPIO_IRQ_Handler);    //����PPORT1�жϷ�����ΪGPIO_IRQ_Handler����Ҫ�ȶ����жϷ���������������������ж��������ٿ����ж�
********************************************************************************/
void Set_Vector_Handler(VECTORn vector, __interrupt void pfunc_handler(void))
{
  if(SYSX->RIVECT == DISABLE)        //���û�������ж�������RAM��������
  {
    Set_VectorTable_To_RAM();                       //�����ж�������RAM
  }
  //�˴����о���ɺ���
  __VECTOR_RAM[vector] = (ISR_TYPE)pfunc_handler;     //���ø��ж�������ַ 
}
/*******************************************************************************
*  �������ƣ�Set_VectorTable_To_RAM()
*  ����˵���������ж�������ӳ�䵽RAM����
*  ����˵������
*  �������أ���
*  ʹ��ʾ����Set_VectorTable_To_RAM();  //�����ж�������ӳ�䵽RAM
********************************************************************************/
void Set_VectorTable_To_RAM(void)
{
  SYSX->RIVECT = ENABLE;                             //�����ж�����ӳ�䵽RAM���� 
}
/*******************************************************************************
*  �������ƣ�Set_VectorTable_To_FLASH()
*  ����˵���������ж�������ӳ�䵽ROM����
*  ����˵������
*  �������أ���
*  ʹ��ʾ����Set_VectorTable_To_FLASH();  //�����ж�������ӳ�䵽FLASH
********************************************************************************/
void Set_VectorTable_To_FLASH(void)
{
  SYSX->RIVECT = DISABLE;                            //�����ж�����ӳ�䵽ROM���� 
}
/*******************************************************************************
*  �������ƣ�VectorTableCopyToRAM()
*  ����˵������FLASH�и����ж�������RAM
*  ����˵������
*  �������أ���
*  ʹ��ʾ����VectorTableCopyToRAM();              //�����ж�������RAM
********************************************************************************/
void VectorTableCopyToRAM(void)
{
  for(int i=0;i < VECTOR_SIZE;i++)
  { //�����ж���������û�ж�����ж�������ֵΪdefault_isr��Ĭ���жϷ�������
    //�˴����о���ɺ���
    __VECTOR_RAM[i] = (0 == __VECTOR_ROM[i] || (ISR_TYPE)0xFFFF == __VECTOR_ROM[i]) ? (ISR_TYPE)default_isr : __VECTOR_ROM[i]; 
  }
}