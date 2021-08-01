#include "include.h"
void main()
{
    DisableInterrupts();          //禁止总中断
    PIN_DIR(P1,1) = 0;                 //该位置0，P1.1设为输入方向
    PIN_DIR(P1,0) = 1;                 //该位置1，P1.0，P4.7设为输出方向
    PIN_DIR(P4,7) = 1;
    while (1)
    {
        DELAY_MS(500);                 //延时500MS
        PIN_OUT(P1,0) = !PIN_OUT(P1, 0);  //P1.0翻转
        PIN_OUT(P4,7) = PIN_IN(P1, 1);    //检测P1.1引脚输入状态，并用P4.7显示出来
    }
}
