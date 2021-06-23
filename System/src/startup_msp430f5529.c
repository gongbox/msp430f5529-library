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
*  �������ƣ�Reset_Handler(void)
*  ����˵������λ�жϷ�����
*  ����˵������
*  �������أ���
*  ʹ��ʾ������
*  ע�������Ҫ�޸Ĺ������ã���ѡlinkerĿ¼�µ�override default program,��ѡ��Defined by Applicatѡ��
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
*  �������ƣ�__low_level_init(void)
*  ����˵����������������������֮ǰ����
*  ����˵������
*  �������أ���
*  ʹ��ʾ������
********************************************************************************/
__intrinsic int __low_level_init(void)
{
    SystemInit();
    return 0;//0-����ʼ������,1-��ʼ������
}
#endif
/*******************************************************************************
*  �������ƣ�SystemInit (void)
*  ����˵����������������������֮ǰ����
*  ����˵������
*  �������أ���
*  ʹ��ʾ������
********************************************************************************/
void SystemInit (void)
{   
    DisableWatchDog();            //�رտ��Ź�
#ifdef RAM_VECTOR
    Set_VectorTable_To_RAM();     //�����ж�����ӳ�䵽RAM����
#else
    Set_VectorTable_To_FLASH();   //�����ж�����ӳ�䵽FLASH����
#endif
    RAM_data_Init();              //��ʼ������  
    /*******************************ʱ�ӳ�ʼ��*********************************/
    CLOCK_DCO_PLLConfig(FLL_REF,FLLREF_DIV,DCO_CLK_HZ);             //��ʼ��DCOƵ��
    CLOCK_MCLK_Config (MCLK_SOURCE , MCLK_DIV);               //����ʱ��Դ��ʱ�ӷ�Ƶ
    CLOCK_SMCLK_Config(SMCLK_SOURCE, SMCLK_DIV);              //����ʱ��Դ��ʱ�ӷ�Ƶ
    CLOCK_ACLK_Config (ACLK_SOURCE , ACLK_DIV);               //����ʱ��Դ��ʱ�ӷ�Ƶ
    /**************************************************************************/
#if(defined(DEBUG))
#if(defined(DEBUG_UART_PRINT))
    UART_Init(Print_UART, Print_Baud);          //����ģʽ�£����ѡ��ʹ�ô�����Ϊ��Ϣ��ӡ���ʼ��UARTģ��
#endif
    Start_Show();
#endif
}
//�������ӡ��ʾ��Ϣ
#ifdef DEBUG
void Start_Show()
{
    DEBUG_PRINTF("\nMSP430F5529 ���Գ���!\n");  //����ģʽ�´�ӡ��Ϣ
    DEBUG_PRINTF("\n MCLK Ƶ��:%ld HZ\n",g_sClock.MCLK.nHZ);
    DEBUG_PRINTF("SMCLK Ƶ��:%ld HZ\n",  g_sClock.SMCLK.nHZ);
    DEBUG_PRINTF(" ACLK Ƶ��:%ld HZ\n",  g_sClock.ACLK.nHZ);  
#ifdef DELAY_TIMER
    DEBUG_PRINTF("\nʹ�ö�ʱ����ʱ,��ʱ��:%s\n",TOSTRING(DELAY_TIMER));
#else
    DEBUG_PRINTF("\nʹ�����ģ����ʱ\n%s\n",TOSTRING(DELAY_MS(ms)));
#endif
}
#endif
/*******************************************************************************
*  �������ƣ�RAM_data_Init(void)
*  ����˵���������ж������� �� ��Ҫ�����ݵ� RAM��
*  ����˵������
*  �������أ���
*  ʹ��ʾ������
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
    //��ʼ���ж�������
    //ISR_TYPE *vector_rom = __section_begin("INTVEC");
    VectorTableCopyToRAM();                       //��FLASH�и����ж�������RAM
    /* ���Ѹ���ֵ�ı�����ROM�︴�����ݵ�RAM�� */
    uint8_t *data_ram = __section_begin("DATA16_I");        //�Ѹ���ֵ�ı����ĵ�ַ��RAM��
    uint8_t *data_rom = __section_begin("DATA16_ID");       //�Ѹ���ֵ�ı��������ݴ����ROM���Ҫ��ֵ��RAM��
    uint8_t *data_rom_end = __section_end("DATA16_ID");
    n = data_rom_end - data_rom;

    /* ���Ƴ�ʼ�����ݵ�RAM�� */
    while (n--)
    {
        *data_ram++ = *data_rom++; 
    }
    /* ���Ѹ���ֵ�ı�����ROM�︴�����ݵ�RAM�� */
    data_ram = __section_begin("DATA20_I");                //�Ѹ���ֵ�ı����ĵ�ַ��RAM��
    data_rom = __section_begin("DATA20_ID");               //�Ѹ���ֵ�ı��������ݴ����ROM���Ҫ��ֵ��RAM��
    data_rom_end = __section_end("DATA20_ID");
    n = data_rom_end - data_rom;

    /* ���Ƴ�ʼ�����ݵ�RAM�� */
    while (n--)
    {
        *data_ram++ = *data_rom++;
    }
    
    /* û����ֵ���߳�ֵΪ0�ı�������Ҫ�����RAM������ݣ�ȷ��ֵΪ0 */
    uint8_t *bss_start = __section_begin("DATA16_Z");
    uint8_t *bss_end = __section_end("DATA16_Z");

    /* ���û����ֵ���߳�ֵΪ0�ı�������ֵ */
    n = bss_end - bss_start;
    while(n--)
    {
        *bss_start++ = 0;
    }
    /* û����ֵ���߳�ֵΪ0�ı�������Ҫ�����RAM������ݣ�ȷ��ֵΪ0 */
    bss_start = __section_begin("DATA20_Z");
    bss_end = __section_end("DATA20_Z");

    /* ���û����ֵ���߳�ֵΪ0�ı�������ֵ */
    n = bss_end - bss_start;
    while(n--)
    {
        *bss_start++ = 0;
    }
   
     /* ��ֵ�� __ramfunc �����ĺ����ĵĴ���ε� RAM�����Լӿ���������        */
    uint8_t *code_relocate_ram = __section_begin("CODE_I");
    uint8_t *code_relocate = __section_begin("CODE_ID");
    uint8_t *code_relocate_end = __section_end("CODE_ID");

    /* ��ROM�︴�ƺ������뵽RAM�� */
    n = code_relocate_end - code_relocate;
    while (n--)
    {
        *code_relocate_ram++ = *code_relocate++;
    }
    
    /* ��ֵ Thread-local storage for main thread(require custom runtime library)��RAM*/
    uint8_t *tls_ram = __section_begin("TLS16_I");
    uint8_t *tls_start = __section_begin("TLS16_ID");
    uint8_t *tls_end   = __section_end("TLS16_ID");

    /* ��ROM�︴�ƺ������뵽RAM�� */
    n = tls_end - tls_start;
    while (n--)
    {
        *tls_ram++ = *tls_start++;
    }
}