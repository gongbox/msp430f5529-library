#include "easyWin.h"

typedef char * STRING; 
typedef int16_t SIZE;
class SHAPE
{
private:
  SITE start,end;
  COLOR color;
};
class eButton:SHAPE
{
private:
  SITE start,end;
  COLOR color;
public:
  eButton();
  eButton(void * click);
  void drew(void);
  void (*click)(void);
};
class easyWin
{
private:
  SIZE width,height;
public:
  easyWin();
  void (*DrawPoint)(SITE site,COLOR color);
  void (*DrawLine)(SITE start,SITE end,COLOR color);
  void (*DrawString)(SITE start,SIZE size,STRING text);
  void (*DrawChinese)(SITE start,SIZE size,STRING text);
  void (*FillRet)(SITE start,SITE end,COLOR color);
};
easyWin::easyWin()
{
  this->DrawPoint = NULL;
  this->DrawLine = NULL;
  this->DrawString = NULL;
  this->DrawChinese = NULL;
  this->FillRet = NULL;
}
void easyWin_Draw(SHAPE shape)
{
  /*
  switch(shape.type)
  {
  case shape.RECTANGLE:
      easyWin.DrawLine(shape.start,shape.end,BLACK);
      break;
  default:break;
      
  }
  */
}
void easyWin_Init()
{
  eButton button;
  button.drew();
  button.click = easyWin_Init;
}