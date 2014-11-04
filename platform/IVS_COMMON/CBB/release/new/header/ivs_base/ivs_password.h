/********************************************************************
filename    :    ivs_password.h
author      :    yWX150774
created     :    2012.11.20
description :    ����У��ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.20 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_PASSWORD_H
#define CBB_BASE_PASSWORD_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

enum KEY_RELIABILITY
{
    KEY_WEAK,
    KEY_MIDDLE,
    KEY_STRONG,
    KEY_STRONGER
};

/************************************************************************
 * ��֪�����ǿ��
 * pszPassword : ����
 * ����ֵ       : �����ǿ��
 ***********************************************************************/
extern "C" KEY_RELIABILITY EXPORT_DLL GetPasswordReliability(
    const char * pszPassword);

/************************************************************************
 * �����Զ����ɺ��� (�Զ�����(iBufferLength - 1)λ�Ŀ���)
 * pszPasswordBuffer : �������Ļ�����
 * iBufferLength     : �������Ĵ�С (���볤�ȱ���>=6, ���Ի����С����>=7)
 * iSymbolOption     : �ַ�ѡ��
 *     (0 != (iSymbolOption & 0x08)) : �����д�д
 *     (0 != (iSymbolOption & 0x04)) : ������Сд
 *     (0 != (iSymbolOption & 0x02)) : ����������
 *     (0 != (iSymbolOption & 0x01)) : �����������ַ�
 * ����ֵ             : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GeneratePassword(
    char * pszPasswordBuffer, int iBufferLength, int iSymbolOption);

/************************************************************************
 * ����У�麯��
 *                 ��У��Ŀ����������iSymbolOption��Ҫ��, 
 *                 ��������к��в��������������͵��ַ�, Ҳ�᷵��ʧ��
 * pszPassword   : ��У��Ŀ���
 * iSymbolOption : �ַ�ѡ��
 *     (0 != (iSymbolOption & 0x08)) : �����д�д
 *     (0 != (iSymbolOption & 0x04)) : ������Сд
 *     (0 != (iSymbolOption & 0x02)) : ����������
 *     (0 != (iSymbolOption & 0x01)) : �����������ַ�
 * iMinLength    : �������С���� (Ĭ��Ϊ8)
 * iMaxLength    : �������󳤶� (Ĭ��Ϊ-1, -1������Ҫ��)
 * ����ֵ         : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckPassword(
    const char * pszPassword, int iSymbolOption, 
    int iMinLength = 8, int iMaxLength = -1);

NAMESPACE_CBB_END


#endif // CBB_BASE_PASSWORD_H
