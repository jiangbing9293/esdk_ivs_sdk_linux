/********************************************************************
filename    :    ivs_check_input_string.h
author      :    yWX150774
created     :    2012.11.20
description :    ����У��ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.20 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_CHECK_INPUT_STRING_H
#define CBB_BASE_CHECK_INPUT_STRING_H


#include "ivs_namespace.h"

#include <vector>
#include <string>

/************************************************************************
 * Ĭ�ϵ������ַ�������: (���ܰ����������ַ���)
 *    01. ������ ( ' )         02. ��б��+������ ( \' )
 *    03. ˫���� ( " )         04. ��б��+˫���� ( \" )
 *    05. ��Բ���� ( ) )       06. �ֺ� ( ; )
 *    07. ǰ������ ( < )       08. ������� ( > )
 *    09. ��Ԫ���� ( $ )       10. �ٷַ��� ( % )
 *    11. at���� ( @ )        12. ˫�л��� ( -- )
 ***********************************************************************/
static const std::string s_aDefaultInvalidStringArray[] = 
{
    std::string("\'"), 
    std::string("\\\'"), 
    std::string("\""), 
    std::string("\\\""), 
    std::string(")"), 
    std::string(";"), 
    std::string("<"), 
    std::string(">"), 
    std::string("$"), 
    std::string("%"), 
    std::string("@"), 
    std::string("--")
};

/************************************************************************
 * Ĭ�ϵ������ַ������� (���ܰ����������ַ���)
 ***********************************************************************/
static const std::vector<std::string> s_aDefaultInvalidStringSet
(
    s_aDefaultInvalidStringArray, 
    s_aDefaultInvalidStringArray + 
        sizeof(s_aDefaultInvalidStringArray) / sizeof(s_aDefaultInvalidStringArray[0])
);

/************************************************************************
 * Ĭ�ϵ������ַ������� (���Լ��������ַ���), ���ڵ����������Ĺ���
 ***********************************************************************/
static const std::vector<std::string> s_aDefaultIgnoreStringSet;

NAMESPACE_CBB_BEGIN

/************************************************************************
 * У�������ַ�
 * pszString        : ��У����ַ���
 * rIgnoreStringSet : ��Ҫ���Լ��������ַ��� (�����ַ�������) ���ڵ����������Ĺ���
 * rInvalidStringSet: ���ܰ����������ַ���    (�����ַ�������)
 * ����ֵ            : �ַ����Ϸ� ���� IVS_SUCCEED, ���򷵻� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckSpecialString(
    const char * pszString, 
    const std::vector<std::string> & rIgnoreStringSet = s_aDefaultIgnoreStringSet, 
    const std::vector<std::string> & rInvalidStringSet = s_aDefaultInvalidStringSet);

/************************************************************************
 * У�������ַ�
 * pszString    : ��У����ַ���
 *                �Ϸ��ַ���Ϊ { pszIgnoreSet + pszValidSet }
 *                ��:
 *                ���pszString�к��в���pszIgnoreSet��pszValidSet�ַ����е��ַ�
 *                ����ΪpszString���Ϸ�
 *                ��pszIgnoreSet��pszValidSet��ΪNULL, pszStringҲ���Ϸ�
 *                ��pszStringΪNULL��"", Ҳ���Ϸ�
 * pszIgnoreSet : ��Ҫ���Լ����ַ��� (Ĭ�Ϻ����ַ���Ϊǧλ�ָ���(����: ",") )
 *                pszIgnoreSet����ΪNULL, ��ʱ�൱��""
 * pszValidSet  : ��Ϊ��Ч���ַ��� (Ĭ��Ϊ: ".0123456789+E-e")
 *                pszValidSet����ΪNULL, ��ʱ�൱��""
 * ����ֵ        : �ַ����Ϸ� ���� IVS_SUCCEED, ���򷵻� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckNumberString(
    const char * pszString, 
    const char * pszIgnoreSet = ",", 
    const char * pszValidSet = ".0123456789+E-e");

/************************************************************************
 * У��һ���ַ�
 * pszString    : ��У����ַ���
 *                ����64���ַ���һ���Ϸ���:
 *                   ('a'-'z') + ('A'-'Z') + ('0'-'9') + ('_') + ('-')
 * pszIgnoreSet : ��Ҫ���Լ����ַ��� (Ĭ�Ϻ����ַ���Ϊ��)
 *                pszIgnoreSet����ΪNULL, ��ʱ�൱��""
 * ����ֵ        : �ַ����Ϸ� ���� IVS_SUCCEED, ���򷵻� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckNormalString(
    const char * pszString, 
    const char * pszIgnoreSet = NULL);

/************************************************************************
 * У���ַ�������
 * pszString  : ��У����ַ���
 * uiMaxLength: �ַ������ȵ����� (�ַ������ȱ��� <= uiMaxLength)
 * uiMinLength: �ַ������ȵ����� (�ַ������ȱ��� >= uiMinLength), Ĭ��Ϊ0
 * ����ֵ        : �ַ����Ϸ� ���� IVS_SUCCEED, ���򷵻� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckStringLength(
    const char * pszString, 
    unsigned int uiMaxLength, 
    unsigned int uiMinLength = 0);

NAMESPACE_CBB_END


#endif // CBB_BASE_CHECK_INPUT_STRING_H
