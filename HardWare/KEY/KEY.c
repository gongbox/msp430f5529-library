#include "key.h"
#include "msp430f5529_gpio.h"
//��������׶�
typedef enum
{
  KEY_PRESSED           ,//��������
  KEY_REALSE            ,//�����ɿ�
  KEY_REALSE_WOBBLE     ,//ȷ���ɿ�������״̬
  KEY_PRESS_WOBBLE      ,//ȷ�ϰ��µ�����״̬
}KEY_STAGEn;
//�˿����ò���
const struct
{
  PORTn         Port;             //GPIO�˿�
  uint16_t      Pin;              //����
  uint16_t      config;           //
  BOOL          PressedLevel;     //���¶˿ڵ�ƽ״̬
} KEY_InitStruct[KEY_NUM] =
{
  {KEY1_GPIO_PORT,KEY1_GPIO_PIN,KEY1_GPIO_CONFIG,KEY1_PRESSED_LEVEL}, //����1�������ò���
  {KEY2_GPIO_PORT,KEY2_GPIO_PIN,KEY2_GPIO_CONFIG,KEY2_PRESSED_LEVEL}, //����2�������ò���
  //������Ӹ��ఴ�������ȶ԰������к궨����ڲο������޸�
  //{KEY3_GPIO_PORT,KEY3_GPIO_PIN,KEY2_GPIO_CONFIG,KEY3_PRESSED_LEVEL}, //����3�������ò���
};
//����������Ϣ����
volatile struct
{
  void (*ClickedCallBack)(KEY_EVENT);  //�����¼��ص�����
  KEY_STAGEn    Stage;         //��������׶�
  BOOL          isClicked;     //����״̬�Ƿ����ı�                     
  BOOL          isLongClick;   //�Ƿ��ǳ���
  uint16_t      ClickTimes;    //�����������
  uint16_t      DealTime;      //��������ʱ��
  uint16_t      PressedTime;   //��������ʱ��
  uint8_t     	WobbleTime;    //����ʱ�����	
}KeyStruct[KEY_NUM];
/*******************************************************************************
*  �������ƣ�KEY_GPIO_Config(KEYn key)
*  ����˵����KEY�˿ڳ�ʼ������
*  ����˵����KEYn key: ����
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
static void KEY_GPIO_Config(KEYn key)
{
  GPIO_Init(KEY_InitStruct[key].Port, KEY_InitStruct[key].Pin, KEY_InitStruct[key].config);
}
/*******************************************************************************
*  �������ƣ�KEY_Init(KEYn key,void (*ClickedCallBack)(KEY_EVENT))
*  ����˵����KEY��ʼ������
*  ����˵����
             KEYn key: Ҫ��ʼ���İ���
             void (*ClickedCallBack)(KEY_EVENT): �����¼����õĻص�����
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void KEY_Init(KEYn key,void (*ClickedCallBack)(KEY_EVENT))
{
  KEY_GPIO_Config(key);

  KeyStruct[key].ClickedCallBack = ClickedCallBack; //��ᰴ�������¼��ص�����
  KeyStruct[key].ClickTimes = 0;        //�����������
  KeyStruct[key].Stage = KEY_REALSE;    //���������ɿ�״̬
  KeyStruct[key].isClicked = FALSE;     //����״̬δ�ı�
  KeyStruct[key].isLongClick = FALSE;   //���ǳ���
  KeyStruct[key].WobbleTime = 0;        //����ʱ���ʼ��Ϊ0
}
/*******************************************************************************
*  �������ƣ�KEY_isPressed(KEYn key)
*  ����˵�����жϰ����Ƿ���
*  ����˵����
             KEYn key: ����
*  �������أ�BOOL ��������ǰ��״̬
*  ʹ��ʾ����
********************************************************************************/
BOOL KEY_isPressed(KEYn key)
{
  if(KEY_InitStruct[key].PressedLevel == GPIO_ReadBit(KEY_InitStruct[key].Port, KEY_InitStruct[key].Pin))
    return TRUE;
  else 
    return FALSE;
}
/*******************************************************************************
*  �������ƣ�KEY_20msTimingProcess(KEYn key)
*  ����˵��������20ms���ڴ�����
*  ����˵����
             KEYn key: ����
*  �������أ���
*  ʹ��ʾ����
********************************************************************************/
void KEY_20msTimingProcess(KEYn key)
{
  switch(KeyStruct[key].Stage)
  {
  case KEY_REALSE:
    if(KEY_isPressed(key)) //�����������
    {
      KeyStruct[key].Stage = KEY_PRESS_WOBBLE;    //���밴�������׶�
      KeyStruct[key].WobbleTime = 0;              //����ʱ������
      KeyStruct[key].PressedTime = 0;             //��������ʱ������
    }
    break;
  case KEY_PRESS_WOBBLE:
    KeyStruct[key].PressedTime += KEY_TIMING_PROCESS_TIME;								//��������ʱ�俪ʼ��ʱ
    KeyStruct[key].WobbleTime += KEY_TIMING_PROCESS_TIME; 								//����ʱ�俪ʼ��ʱ
    if(KEY_isPressed(key)) //�����������
    {
      if(KeyStruct[key].WobbleTime >= KEY_PRESS_WOBBLE_TIME)  //����ʱ��ﵽ
      {
        KeyStruct[key].Stage = KEY_PRESSED;         //����ȷ�ϰ��£������Ѱ��½׶�
        KeyStruct[key].ClickTimes ++;             //���´�����1
      }
    }
    else
    {
      KeyStruct[key].Stage = KEY_REALSE;          //�������ʱ�����ɿ�����Ϊ�ôΰ���������Ч
    }
    break;
  case KEY_PRESSED:
    KeyStruct[key].PressedTime += KEY_TIMING_PROCESS_TIME;                //������¼��������ʱ��
    if( (KeyStruct[key].isLongClick == FALSE)
        &&(KeyStruct[key].PressedTime >= KEY_LONG_CLICK_PERIOD)) //�����ʱ�䰴�£�����Ϊ�ǳ���״̬
    {
      KeyStruct[key].ClickedCallBack(KEY_LONG_CLICK); //��������ʱ�����������Ϊ�ǳ���
      KeyStruct[key].isLongClick = TRUE;          //���Ϊ����
    }
    else
    {
      if(KEY_isPressed(key) == FALSE) //��������ɿ�
      {
        KeyStruct[key].Stage = KEY_REALSE_WOBBLE; //���밴���ɿ��������׶�
        KeyStruct[key].WobbleTime = 0;            //����ʱ�����㣬һ�ߺ����׶�����
      }
    }
    break;
  case KEY_REALSE_WOBBLE:
    KeyStruct[key].WobbleTime += KEY_TIMING_PROCESS_TIME;                 //�����ɿ�����ʱ�俪ʼ��ʱ
    if(KEY_isPressed(key) == FALSE) //��������ɿ�
    {
      if(KeyStruct[key].WobbleTime >= KEY_REALSE_WOBBLE_TIME)  //����ʱ��ﵽ
      {
        KeyStruct[key].Stage = KEY_REALSE;              //���밴����ȫ�ɿ��׶�
        KeyStruct[key].isClicked = TRUE;             //��ǰ���״̬�����ı�
      }
    }
    break;	
    default:break;
  }
  if(KeyStruct[key].isClicked == TRUE)  //�������״̬�ı���
  {
    //�ﵽ˫��ʱ�����ֵ���Ѿ�˫��
    KeyStruct[key].DealTime += KEY_TIMING_PROCESS_TIME;
    if((KeyStruct[key].DealTime >= KEY_DOUBLE_CLICK_PERIOD)
      ||KeyStruct[key].ClickTimes >= 2)
    {
      if(KeyStruct[key].ClickedCallBack != NULL)
      {
        //�������ʱ��ܳ�
        if((KeyStruct[key].isLongClick == FALSE))
        {
          if(KeyStruct[key].ClickTimes >= 2)  //���¶������Ϊ��˫��
            KeyStruct[key].ClickedCallBack(KEY_DOUBLE_CLICK);
          else if(KeyStruct[key].ClickTimes == 1)                               //������Ϊ�ǵ���
            KeyStruct[key].ClickedCallBack(KEY_CLICK);
        }
      }
      KeyStruct[key].PressedTime = 0;
      KeyStruct[key].ClickTimes = 0;
      KeyStruct[key].DealTime = 0;
      KeyStruct[key].isClicked = FALSE;
      KeyStruct[key].isLongClick = FALSE;
    }
  }
}
