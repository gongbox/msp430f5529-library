/********************                                      ********************
 * �ļ���       ��assert.h
 * ����         �����Ժ궨��ͷ�ļ�
 *
 * ʵ��ƽ̨     ��MSP430F5529
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 * ��    ע     ��assert_failed() �� assert.c �ﶨ��,������������Ƿ����㣬����������ʱ��ִ��assert_failed��������ʾ�û�
 *
 * ����         ��
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

