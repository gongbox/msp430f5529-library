/********************************************************************
创建人：陈志强
时间：2010.5.17
功能：LCD12864头文件
******************************************************************/
#ifndef _LCD12864_H_
#define _LCD12864_H_
/***************************************************/
void LcdCS(unchar num);
void LcdWriteCmd(unchar num,unchar cmd);
void LcdWriteDate(unchar num,unchar date);
unchar LcdReadDate(unchar num);
void PageSet(unchar num,unchar page);
void YSet(unchar num,unchar y);
void LcdWriteToSet(unchar x,unchar y,unchar date);
unchar LcdReadFromSet(unchar x,unchar y);
void LcdWriteDot(unchar x,unchar y,unchar attr);
void LcdWriteCh12(unchar x,unchar y,unchar *string);
void LcdWriteAsc58(unchar x,unchar y,unchar string);
void LcdWriteAsc816(unchar x,unchar y,unchar string);
extern void LcdWriteChStr(unchar x,unchar y,unchar *string);
extern void LcdClear();
extern void LcdInt();
/***************************************************/
#endif