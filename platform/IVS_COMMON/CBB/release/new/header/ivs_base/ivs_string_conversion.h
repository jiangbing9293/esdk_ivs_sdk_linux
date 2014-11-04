/********************************************************************
filename    :    ivs_string_conversion.h
author      :    yWX150774
created     :    2012/11/22
description :    �ַ�ת������С���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/22 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_STRING_CONVERSION_H
#define CBB_BASE_STRING_CONVERSION_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/***********************************************************************
 * �����
 * pData        : ����������������, Ҳ�����������ݵĻ�����
 * iDataLen     : �������ݳ���
 * pSecretKey   : ������������Կ
 * iSecretKeyLen: ��Կ����
 * ����ֵ        : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_XOR(
    void * pData, int iDataLen, 
    const void * pSecretKey, int iSecretKeyLen);

/************************************************************************
 * ���������ַ�����ת��Ϊ�ɴ�ӡ���ַ����� ('0'-'9', 'A'-'Z', 'a'-'z', '+', '-')
 * ע��: ���ת���ǲ������
 * pszData  : ��������, Ҳ���������(�����Զ���'\0')
 * iDataLen : �������ݳ��� (������'\0')
 * ����ֵ    : ��
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_BinaryToChars(
    unsigned char * pszData, int iDataLen);

/************************************************************************
 * ���������ַ�(unsigned char) ת��Ϊ �ɴ�ӡ��ʮ�������ַ����� ('0'-'9', 'a'-'f')
 * ���ת���ǿ����, ��IVS_HexesToUChar�ǻ����
 * ucIn  : �������� (�����������ַ�)
 * szOut : ������� (������������2, �����Զ���'\0')
 * ����ֵ : ��
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_UCharToHexes(
    unsigned char ucIn, char szOut[2]);

/************************************************************************
 * ���ɴ�ӡ��ʮ�������ַ����� ('0'-'9', 'a'-'f') ת��Ϊ �������ַ�(unsigned char)
 * ���ת���ǿ����, ��IVS_UCharToHexes�ǻ����
 * szIn   : �������� (������������2)
 * pszOut : ������� (�����������ַ�, �����Զ���'\0')
 * ����ֵ  : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_HexesToUChar(
    const char szIn[2], unsigned char * pszOut);

/************************************************************************
 * ���������ַ�����ת��Ϊ�ɴ�ӡ��ʮ�������ַ����� ('0'-'9', 'a'-'f')
 * ���ת���ǿ����, ��IVS_HexesToBinary�ǻ����
 * pszInData  : ��������
 * iInDataLen : �������ݳ��� (������'\0')
 * pszOutData : ������� (������������iInDataLen*2, �����Զ���'\0')
 * ����ֵ      : ��
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_BinaryToHexes(
    const unsigned char * pszInData, int iInDataLen, char * pszOutData);

/************************************************************************
 * ���ɴ�ӡ��ʮ�������ַ����� ('0'-'9', 'a'-'f') ת��Ϊ �������ַ�����
 * ���ת���ǿ����, ��IVS_BinaryToHexes�ǻ����
 * pszInData  : ��������
 * iInDataLen : �������ݳ��� (iInDataLen������ż��, ������'\0')
 * pszOutData : ������� (������������iInDataLen/2, �����Զ���'\0')
 * ����ֵ      : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_HexesToBinary(
    const char * pszInData, int iInDataLen, unsigned char * pszOutData);

NAMESPACE_CBB_END


#endif // CBB_BASE_STRING_CONVERSION_H
