#include "include.h"
void main()
{
    DisableInterrupts();          //��ֹ���ж�
    PIN_DIR(P1,1) = 0;                 //��λ��0��P1.1��Ϊ���뷽��
    PIN_DIR(P1,0) = 1;                 //��λ��1��P1.0��P4.7��Ϊ�������
    PIN_DIR(P4,7) = 1;
    while (1)
    {
        DELAY_MS(500);                 //��ʱ500MS
        PIN_OUT(P1,0) = !PIN_OUT(P1, 0);  //P1.0��ת
        PIN_OUT(P4,7) = PIN_IN(P1, 1);    //���P1.1��������״̬������P4.7��ʾ����
    }
}
