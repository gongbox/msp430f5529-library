#include "msp430f5529_it.h"
#include "msp430f5529_vectors.h"

//定义中断向量表到RAM的特定区域，很重要
__no_init ISR_TYPE __VECTOR_RAM[VECTOR_SIZE]@VECTOR_RAM_START_ADDR;
#define            __VECTOR_ROM ((ISR_TYPE *)VECTOR_ROM_START_ADDR)
extern const SFR_MemMapPtr SFRX;
const SYS_MemMapPtr SYSX = SYS_BASE_PTR;
/*******************************************************************************
*  函数名称：Set_Vector_Handler(VECTORn vector,__interrupt void pfunc_handler(void))
*  功能说明：设置中断向量地址
*  参数说明：
            VECTORn vector  ：中断向量（枚举变量，见.h文件）
            __interrupt void pfunc_handler(void)  ： 中断服务函数
*  函数返回：无
*  使用示例：Set_Vector_Handler(VECTOR_PORT1,GPIO_IRQ_Handler);    //设置PPORT1中断服务函数为GPIO_IRQ_Handler，需要先定义中断服务函数，另外最好先设置中断向量，再开启中断
********************************************************************************/
void Set_Vector_Handler(VECTORn vector, __interrupt void pfunc_handler(void))
{
  if(SYSX->RIVECT == DISABLE)        //如果没有设置中断向量表到RAM，则设置
  {
    Set_VectorTable_To_RAM();                       //设置中断向量表到RAM
  }
  //此处若有警告可忽略
  __VECTOR_RAM[vector] = (ISR_TYPE)pfunc_handler;     //设置该中断向量地址 
}
/*******************************************************************************
*  函数名称：Set_VectorTable_To_RAM()
*  功能说明：设置中断向量表映射到RAM区间
*  参数说明：无
*  函数返回：无
*  使用示例：Set_VectorTable_To_RAM();  //设置中断向量表映射到RAM
********************************************************************************/
void Set_VectorTable_To_RAM(void)
{
  SYSX->RIVECT = ENABLE;                             //设置中断向量映射到RAM区间 
}
/*******************************************************************************
*  函数名称：Set_VectorTable_To_FLASH()
*  功能说明：设置中断向量表映射到ROM区间
*  参数说明：无
*  函数返回：无
*  使用示例：Set_VectorTable_To_FLASH();  //设置中断向量表映射到FLASH
********************************************************************************/
void Set_VectorTable_To_FLASH(void)
{
  SYSX->RIVECT = DISABLE;                            //设置中断向量映射到ROM区间 
}
/*******************************************************************************
*  函数名称：VectorTableCopyToRAM()
*  功能说明：从FLASH中复制中断向量表到RAM
*  参数说明：无
*  函数返回：无
*  使用示例：VectorTableCopyToRAM();              //复制中断向量表到RAM
********************************************************************************/
void VectorTableCopyToRAM(void)
{
  for(int i=0;i < VECTOR_SIZE;i++)
  { //复制中断向量表，对没有定义的中断向量赋值为default_isr（默认中断服务函数）
    //此处若有警告可忽略
    __VECTOR_RAM[i] = (0 == __VECTOR_ROM[i] || (ISR_TYPE)0xFFFF == __VECTOR_ROM[i]) ? (ISR_TYPE)default_isr : __VECTOR_ROM[i]; 
  }
}