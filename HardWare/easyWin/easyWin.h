#ifndef EASYWIN_H_
#define EASYWIN_H_

#include "common.h"

//点坐标
typedef struct SITE
{
  int16_t x;
  int16_t y;
}SITE;  
//颜色
typedef enum COLOR
{
  BLACK,
  WHITE,
  RED,
  BLUE,
  GREEN
}COLOR; 
//边形
//长方形
typedef struct RECTANGLE
{
  SITE start;
  SITE end;
  COLOR color;
}RECTANGLE;
//形状
/*
typedef struct eButton
{
  
}eButton;
*/
#endif