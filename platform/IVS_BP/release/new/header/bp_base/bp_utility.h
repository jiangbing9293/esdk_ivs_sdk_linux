/********************************************************************
    filename    :    bp_utility.h
    author      :    yWX150774
    created     :    2013/03/11
    description :    ����С���
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/11 ��ʼ�汾
*********************************************************************/

#ifndef BP_UTILITY_H
#define BP_UTILITY_H


#include "bp_def.h"

NAMESPACE_BP_BEGIN

/* ��ȡ����·�����ļ��� */
extern "C" EXPORT_DLL const char * GetFileNameWithoutPath(const char * pszFilenameWithPath);

/* ���������(IP)�Ƿ�Ϸ�, ֻУ��ÿ���ָ�λ�Ƿ�����[0, 255] */
extern "C" EXPORT_DLL bool CheckHostName(const char * pszHostName);

/* ���˿ں��Ƿ�Ϸ�, ֻУ���Ƿ�����[1, 65535] */
extern "C" EXPORT_DLL bool CheckHostPort(const char * pszHostPort);

/* �ж��Ƿ�Ϊ�������ŵĺϷ������ַ��� */
extern "C" EXPORT_DLL bool IsUnsignedInteger(const char * pszValue);

enum enPathCheckMode
{
    PATH_CHECK_DEFAULT_MODE, // ��Ĭ�ϵķ�ʽУ��·����
    PATH_CHECK_UNIX_MODE,    // ��ȷָ��ΪUNIX��Ӧ�Ĺ���
    PATH_CHECK_WINDOWS_MODE  // ��ȷָ��ΪWINDOWS��Ӧ�Ĺ���
};

/******************************************************************
 * ����������ж��Ƿ�Ϊ�Ϸ���·����, �����ж�·���Ƿ����
 * Unix:
 *          1. ·��:
 *             1>  �����ǿմ�
 *             2>  ���ܰ��� <>\"?:|*
 *             3>  ���ܰ��� \t\r\n
 *             4>  ���ܰ��� \\
 *          2. ÿһ����·��(�ļ���, �ļ�����):
 *             1>  �����Կո�ʼ
 *             2>  �����Կո����
 * Windows:
 *          1. ·��:
 *             1>  �����ǿմ�
 *             2>  ���ܰ��� <>\"?|*
 *             3>  ���ܰ��� \t\r\n
 *             4>  ':'ֻ�ܳ����ڸ�Ŀ¼��
 *          2. ÿһ����·��(�ļ���, �ļ�����):
 *             1>  �����Կո�ʼ
 *             2>  �����Կո����
 *             3>  ������'.'��ʼ(.��..����)
 *             4>  ������\\��ʼ
 *             5>  ������/��ʼ
 ******************************************************************/
extern "C" EXPORT_DLL bool IsValidPathName(
    const char * pszPathName, 
    enPathCheckMode enMode = PATH_CHECK_DEFAULT_MODE);

NAMESPACE_BP_END


#endif // BP_UTILITY_H
