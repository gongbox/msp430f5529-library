#ifndef EASYWIN_H_
#define EASYWIN_H_

#include "common.h"

//������
typedef struct SITE
{
  int16_t x;
  int16_t y;
}SITE;  
//��ɫ
typedef enum COLOR
{
  BLACK,
  WHITE,
  RED,
  BLUE,
  GREEN
}COLOR; 
//����
//������
typedef struct RECTANGLE
{
  SITE start;
  SITE end;
  COLOR color;
}RECTANGLE;
//��״
/*
typedef struct eButton
{
  
}eButton;
*/
#endif