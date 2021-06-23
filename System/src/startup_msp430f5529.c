#include "msp430f5529_system.h"
#include "include.h"
#include "intrinsics.h"

static void RAM_data_Init(void);
static void SystemInit(void);
#ifdef DEBUG
static void Start_Show(void);
#endif
#if 0
/*******************************************************************************
*  函数名称：Reset_Handler(void)
*  功能说明：复位中断服务函数
*  参数说明：无
*  函数返回：无
*  使用示例：无
*  注意事项：需要修改工程配置，勾选linker目录下的override default program,并选择Defined by Applicat选项
********************************************************************************/
extern void main();
#pragma vector=RESET_VECTOR
__interrupt void Reset_Handler(void)
{
  SystemInit();
  main();
  while(1);
}
#else
/*******************************************************************************
*  函数名称：__low_level_init(void)
*  功能说明：启动函数，在主函数之前调用
*  参数说明：无
*  函数返回：无
*  使用示例：无
********************************************************************************/
__intrinsic int __low_level_init(void)
{
    SystemInit();
    return 0;//0-不初始化参数,1-初始化参数
}
#endif
/*******************************************************************************
*  函数名称：SystemInit (void)
*  功能说明：启动函数，在主函数之前调用
*  参数说明：无
*  函数返回：无
*  使用示例：无
********************************************************************************/
void SystemInit (void)
{   
    DisableWatchDog();            //关闭看门狗
#ifdef RAM_VECTOR
    Set_VectorTable_To_RAM();     //设置中断向量映射到RAM区域
#else
    Set_VectorTable_To_FLASH();   //设置中断向量映射到FLASH区域
#endif
    RAM_data_Init();              //初始化数据  
    /*******************************时钟初始化*********************************/
    CLOCK_DCO_PLLConfig(FLL_REF,FLLREF_DIV,DCO_CLK_HZ);             //初始化DCO频率
    CLOCK_MCLK_Config (MCLK_SOURCE , MCLK_DIV);               //设置时钟源及时钟分频
    CLOCK_SMCLK_Config(SMCLK_SOURCE, SMCLK_DIV);              //设置时钟源及时钟分频
    CLOCK_ACLK_Config (ACLK_SOURCE , ACLK_DIV);               //设置时钟源及时钟分频
    /**************************************************************************/
#if(defined(DEBUG))
#if(defined(DEBUG_UART_PRINT))
    UART_Init(Print_UART, Print_Baud);          //调试模式下，如果选择使用串口作为信息打印则初始化UART模块
#endif
    Start_Show();
#endif
}
//启动后打印提示信息
#ifdef DEBUG
void Start_Show()
{
    DEBUG_PRINTF("\nMSP430F5529 测试程序!\n");  //调试模式下打印信息
    DEBUG_PRINTF("\n MCLK 频率:%ld HZ\n",g_sClock.MCLK.nHZ);
    DEBUG_PRINTF("SMCLK 频率:%ld HZ\n",  g_sClock.SMCLK.nHZ);
    DEBUG_PRINTF(" ACLK 频率:%ld HZ\n",  g_sClock.ACLK.nHZ);  
#ifdef DELAY_TIMER
    DEBUG_PRINTF("\n使用定时器延时,定时器:%s\n",TOSTRING(DELAY_TIMER));
#else
    DEBUG_PRINTF("\n使用软件模拟延时\n%s\n",TOSTRING(DELAY_MS(ms)));
#endif
}
#endif
/*******************************************************************************
*  函数名称：RAM_data_Init(void)
*  功能说明：复制中断向量表 和 必要的数据到 RAM里
*  参数说明：无
*  函数返回：无
*  使用示例：无
********************************************************************************/
#pragma section = "DATA16_I"
#pragma section = "DATA20_I"
#pragma section = "DATA16_ID"
#pragma section = "DATA20_ID"
#pragma section = "DATA16_Z"
#pragma section = "DATA20_Z"
#pragma section = "TLS16_I"
#pragma section = "TLS16_ID"
#pragma section = "CODE_I"
#pragma section = "CODE_ID"
//#pragma section = "INTVEC"
void RAM_data_Init(void)
{
    uint32_t n;
    //初始化中断向量表
    //ISR_TYPE *vector_rom = __section_begin("INTVEC");
    VectorTableCopyToRAM();                       //从FLASH中复制中断向量表到RAM
    /* 把已赋初值的变量从ROM里复制数据到RAM里 */
    uint8_t *data_ram = __section_begin("DATA16_I");        //已赋初值的变量的地址在RAM里
    uint8_t *data_rom = __section_begin("DATA16_ID");       //已赋初值的变量的数据存放在ROM里，需要赋值到RAM里
    uint8_t *data_rom_end = __section_end("DATA16_ID");
    n = data_rom_end - data_rom;

    /* 复制初始化数据到RAM里 */
    while (n--)
    {
        *data_ram++ = *data_rom++; 
    }
    /* 把已赋初值的变量从ROM里复制数据到RAM里 */
    data_ram = __section_begin("DATA20_I");                //已赋初值的变量的地址在RAM里
    data_rom = __section_begin("DATA20_ID");               //已赋初值的变量的数据存放在ROM里，需要赋值到RAM里
    data_rom_end = __section_end("DATA20_ID");
    n = data_rom_end - data_rom;

    /* 复制初始化数据到RAM里 */
    while (n--)
    {
        *data_ram++ = *data_rom++;
    }
    
    /* 没赋初值或者初值为0的变量，需要清除其RAM里的数据，确保值为0 */
    uint8_t *bss_start = __section_begin("DATA16_Z");
    uint8_t *bss_end = __section_end("DATA16_Z");

    /* 清除没赋初值或者初值为0的变量数据值 */
    n = bss_end - bss_start;
    while(n--)
    {
        *bss_start++ = 0;
    }
    /* 没赋初值或者初值为0的变量，需要清除其RAM里的数据，确保值为0 */
    bss_start = __section_begin("DATA20_Z");
    bss_end = __section_end("DATA20_Z");

    /* 清除没赋初值或者初值为0的变量数据值 */
    n = bss_end - bss_start;
    while(n--)
    {
        *bss_start++ = 0;
    }
   
     /* 赋值用 __ramfunc 声明的函数的的代码段到 RAM，可以加快代码的运行        */
    uint8_t *code_relocate_ram = __section_begin("CODE_I");
    uint8_t *code_relocate = __section_begin("CODE_ID");
    uint8_t *code_relocate_end = __section_end("CODE_ID");

    /* 从ROM里复制函数代码到RAM里 */
    n = code_relocate_end - code_relocate;
    while (n--)
    {
        *code_relocate_ram++ = *code_relocate++;
    }
    
    /* 赋值 Thread-local storage for main thread(require custom runtime library)到RAM*/
    uint8_t *tls_ram = __section_begin("TLS16_I");
    uint8_t *tls_start = __section_begin("TLS16_ID");
    uint8_t *tls_end   = __section_end("TLS16_ID");

    /* 从ROM里复制函数代码到RAM里 */
    n = tls_end - tls_start;
    while (n--)
    {
        *tls_ram++ = *tls_start++;
    }
}