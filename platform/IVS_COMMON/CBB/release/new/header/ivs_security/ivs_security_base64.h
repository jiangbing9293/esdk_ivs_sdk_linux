/********************************************************************
filename    :    ivs_security_base64.h
author      :    yWX150774
created     :    2012/11/10
description :    Base64�����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_BASE64_H
#define CBB_SECURITY_BASE64_H


#include "ivs_uncopy.hpp"

#include "openssl/evp.h"

NAMESPACE_CBB_BEGIN

/*********************************************************************
 * Base64�����
 ********************************************************************/
class EXPORT_DLL CBase64
{
public:
    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������, ���Զ���ʽ��(����֮��)�������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     ****************************************************************/
    static bool EncodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ����Base64�������ĸ�ʽ��(�л���֮��)��������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������ (�����Զ���ʽ��)
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     ****************************************************************/
    static bool EncodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  ��������������
     * iInLen:     �������ݵĳ���
     * pszOutBuff: ����Base64������������
     * iOutLen:    �����������ݵĳ���
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /******************************************************************
     * �ַ������ݵ�Base64����
     * pszInBuff:  �ַ�����������
     * pszOutBuff: ����Base64������������ (�ַ���)
     * iOutLen:    ����������ݵĻ����С
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool EncodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * ���������ݵ�Base64����
     * pszInBuff:  �ַ�����������
     * pszOutBuff: ����Base64������������ (�ַ���)
     * iOutLen:    ����������ݵĻ����С
     * ����ֵ:      �����Ƿ�ɹ�
     *****************************************************************/
    static bool DecodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * ����ΪinLen�Ķ�������������, ����Base64�����������������ռ�õ���󳤶�
     *****************************************************************/
    static int GetEncodeBinarySize(int iInLen);

    /******************************************************************
     * ����ΪinLen�Ķ�������������, ����Base64�����������������ռ�õ���󳤶�
     *****************************************************************/
    static int GetDecodeBinarySize(int iInLen);

    /******************************************************************
     * �ַ�����������, ����Base64�����������������ռ�õ���󳤶�(����'\0')
     *****************************************************************/
    static int GetEncodeStringSize(const char * pszInBuff);

    /******************************************************************
     * �ַ�����������, ����Base64�����������������ռ�õ���󳤶�(����'\0')
     *****************************************************************/
    static int GetDecodeStringSize(const char * pszInBuff);
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_BASE64_H
