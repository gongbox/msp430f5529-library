/********************                                      ********************
 * 文件名       ：assert.h
 * 描述         ：断言宏定义头文件
 *
 * 实验平台     ：MSP430F5529
 * 库版本       ：
 * 嵌入系统     ：
 * 备    注     ：assert_failed() 在 assert.c 里定义,用来检测条件是否满足，条件不满足时，执行assert_failed函数，提示用户
 *
 * 作者         ：
**********************************************************************************/


#ifndef _ASSERT_H_
#define _ASSERT_H_

/********************************************************************/

void assert_failed(char *file, int line,char *funName,char *str);                  

#ifdef DEBUG
#define ASSERT(expr,funName,str) \
    if (!(expr)) \
        assert_failed(__FILE__, __LINE__,funName,str)
#else
#define ASSERT(expr)
#endif

/********************************************************************/
#endif /* _ASSERT_H_ */

